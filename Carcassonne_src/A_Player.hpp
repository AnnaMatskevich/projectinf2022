// ----------------------------------------------------------------------------
#ifndef _A_PLAYER_HPP_
#define _A_PLAYER_HPP_
// ----------------------------------------------------------------------------
#include "A_Chip.hpp"
#include <ustring.h>
// ----------------------------------------------------------------------------
// Исходные данные об игроках игры Каркассон:
// в игре 5 игроков (красный, желтый, зеленый, синий, серый)
// у каждого игрока по 4 фишки
// ----------------------------------------------------------------------------
#define CHIPS_MAX   4
// ----------------------------------------------------------------------------
class A_Player
{
public:
	// конструктор (задаем начальные значения всех переменных класса)
	A_Player() { clear(); }

	// игрок играет?
	bool				Is_playing;

	// идентификатор игрока (он нужен для того, чтобы потом можно было определить кто захватил дорогу или сушу)
	unsigned long long 	UserID;

	// имя игрока
	UnicodeString		Name;

	// цвет игрока
	TColor				Color;

	// получено бананов
	int             	Bananas_count;

	// получено ракушек
	int             	Seashells_count;

	// закрыто рынков
	int                 Maketplaces_count;

	// Фишки игрока
	A_Chip              Chips[CHIPS_MAX];

	// графический интерфейс (панель) для отображения очков и других характеристик игрока

	// панель на которой распологаются все характеристики игрока
	TPanel *    		panel_main;

	// разделитель между панелями (соседних игроков
	TPanel *    		panel_vert;

	// на главной панеле есть panel_top, она предназначена для отображения
	// имени игрока и цвета (зеленый - ход текущего игрока, обычный цвет - этот игрок не ходит)
	TPanel *  			panel_top;

	// Label для отображения количества бананов
	TLabel *    		label_bananas_count;

	// Label для отображения количества ракушек
	TLabel *    		label_shells_count;

	// Label для отображения количества захваченных рынков
	TLabel *    		label_markets_count;

	// Label для отображения количества заработанных очков
	TLabel *    		label_points_count;

	// Label для отображения количества свободных фишек
	TLabel *    		label_tuples_count;

	// картинка с изображением фишки для данного игрока
	TImage *    		image_tuple;

	// очистить значения переменных
	void clear()
	{
		int i;

		Is_playing        = false;
		UserID            = 0;
		Name              = "";
		Color             = 0;
		Bananas_count     = 0;
		Seashells_count   = 0;
		Maketplaces_count = 0;

		// если хотя бы одна карточка на игровом поле - убрать ее
		for (i = 0; i < CHIPS_MAX; i++)
		{
			Chips[i].hide();
		}

		// графический интерфейс изначатьно не задан
		panel_main          = NULL;
		panel_vert          = NULL;
		panel_top           = NULL;
		label_bananas_count = NULL;
		label_shells_count  = NULL;
		label_markets_count = NULL;
		label_points_count  = NULL;
		label_tuples_count  = NULL;
		image_tuple         = NULL;
	}

	// получить указатель на первую "свободную" фишку
	A_Chip * get_free_chip()
	{
		int i;

		for (i = 0; i < CHIPS_MAX; i++)
		{
			if (Chips[i].IsUsed == false)
				return &Chips[i];
		}
		// ни одной "свободной" игровой фишки не нашли
		return NULL;
	}

	// получить количество свободных фишек
	int get_free_chipa_count()
	{
		int i, n;

		n = 0;
		for (i = 0; i < CHIPS_MAX; i++)
		{
			if (Chips[i].IsUsed == false)
				n= n + 1;
		}

		return n;
	}

	// инициализация графического интерфейса с характеристиками игрока
	void init_gui(TPanel * _panel_main, TPanel * _panel_vert, TPanel * _panel_top,
				TLabel * _label_bananas_count,
				TLabel * _label_shells_count,
				TLabel * _label_markets_count,
				TLabel * _label_points_count,
				TLabel * _label_tuples_count, TImage * _image_tuple)
	{
		panel_main          = _panel_main;
		panel_vert          = _panel_vert;
		panel_top           = _panel_top;
		label_bananas_count = _label_bananas_count;
		label_shells_count  = _label_shells_count;
		label_markets_count = _label_markets_count;
		label_points_count  = _label_points_count;
		label_tuples_count  = _label_tuples_count;
		image_tuple         = _image_tuple;
		// в процессе инициализации надо сделать неактивным игрока
		set_player_active_state(false);
		// обновить значения характеристик
		update_gui();
	}

	// обновить на панеле все характеристики игрока
	void update_gui()
	{
		// бананы
		if (label_bananas_count != NULL)
			label_bananas_count->Caption = Bananas_count;
		// ракушки
		if (label_shells_count != NULL)
			label_shells_count->Caption  = Seashells_count;
		// рынки
		if (label_markets_count != NULL)
			label_markets_count->Caption = Maketplaces_count;
		// фишки
		if (label_tuples_count != NULL)
			label_tuples_count->Caption  = get_free_chipa_count();
		// общее количество очков (можно задать любой способ расчета итоговой суммы)
		if (label_points_count != NULL)
			label_points_count->Caption  = calc_points();
	}

	// установить активность игрока (ходит он или нет? Это делается за счет цвета панели)
	void set_player_active_state(bool is_active)
	{
		if (panel_top != NULL)
			if (is_active == true)
				panel_top->Color = clLime; //0x0053FF53;
			else
				panel_top->Color = clBtnFace;
	}

	// отобразить имя игрока на панеле
	void set_name()
	{
		if (panel_top != NULL) panel_top->Caption = Name;
	}

	// рсчитать количество очков игрока
	int calc_points()
	{
		// расчет производится по следующей формуле:
		// количество очков = целое число троек бананов + троек ракушек + 2 * число захваченных рынков
		int points;

		points = Bananas_count / 3 + Seashells_count / 3 + Maketplaces_count * 2;

		return points;
	}

};
// ----------------------------------------------------------------------------
#endif
