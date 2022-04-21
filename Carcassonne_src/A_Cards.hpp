// ----------------------------------------------------------------------------
#ifndef _A_CARDS_HPP_
#define _A_CARDS_HPP_
// ----------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>

#include <stdlib.h>         // в этом модуле есть генератор случайных числе

// ----------------------------------------------------------------------------
// число карточек сейчас 74: 0 - рубашка, 29-ая всегда начальная карточка
// ----------------------------------------------------------------------------

#define CARDS_COUNT_MAX		74	// счет начинается с 1, 0-ая карточка это обратная сторона игровой карточки (рубашка игровой карточки)
#define CARD_WIDTH			86
#define CARD_HEIGHT 		86
#define CARD_SIZE   		87
// ----------------------------------------------------------------------------
// Стороны игровой карточки
//      0
//    -----
//   |     |
//  3|  4  |1
//	 |     |
//	  -----
//      2
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// структура данных - игровой элемент карточки: дорога, суша, рынок
// ----------------------------------------------------------------------------
// для одной стороны карточки(из 5)
struct A_CardItem
{
	// конструктор (начальная инициализация описания стороны карточки)
	A_CardItem() { clear(); }

	// очистить карточку - ничего на ней нет
	void clear() { item_type  = 0; item_connection = 0; item_price = 0; UserID = 0; x = 0; y = 0;}

	// тип игрового элемента:
	// 0 - ничего нет, 1 - дорога, 2 - суша, 3 - рынок
	char   			item_type;

	// тип соединения
	// для дорог:                                     для суши:
	// 0 - нет,                                       0 - нет
	// 1 - тупик в сушу (не используется)             1 - тупик
	// 2 - тупик,                                     2 - соединено с противоположной стороной
	// 3 - соединено c противоложной стороной,        3 - соединено с правой стороной
	// 4 - соединено с правой стороной,               4 - соединено с левой стороной
	// 5 - соединено с левой стороной                 5 - соединено с правой и противоположной
	//                								  6 - соединено с левой и противоположной
	//                                                7 - соединено с боковыми
	char 			item_connection;

	// стоимость (число ракушек, бананов, у рынка ни чего нет - его просто можно захватить)
	char 			item_price;

	// владелец (id пользователя), 0 - нет владельца
	unsigned int	UserID;

	// координаты куда можно выставлять фишку: (0,0) - значит фишку ставить нельзя
	int				x;
	int				y;

	// установить значения
	void set_type(char _item_type, char _item_connection, char _item_price)
	{
		item_type       = _item_type;
		item_connection = _item_connection;
		item_price      = _item_price;
	}

	// установить координаты для фишки
	void set_xy(int _x, int _y) { x = _x; y = _y; }

	// вращение координат по часовой стрелки на 90'
	void rotate()
	{
		int tmp;
		if ((x == 0) && (y == 0))
			return;

		tmp = x;
		x = CARD_WIDTH - 1 - y;
		y = tmp;
	}
};


// ----------------------------------------------------------------------------
// структура для карточки с сторонами
// ----------------------------------------------------------------------------
struct A_Card
{
	TPanel *	Panel;				// панель
	TImage *	Image;				// картинка

	// стороны карточки 0-3, 4-центр
	A_CardItem  card_items[5];

	// карта на поле?
	bool		on_field;

	// конструктор (начальная инициализация игровой карточки)
	A_Card()
	{
		Panel    = NULL;
		Image    = NULL;
		// очистить игровую карточку
		clear();
	}

	// очистить игровую карточку, просто нужно
	void 		clear()
	{
		// сначала удаляем картинку
		if (Image != NULL) delete Image;
		// потом удаляем панель
		if (Panel != NULL) delete Panel;

		Panel    = NULL;
		Image    = NULL;
		on_field = false;
		card_items[0].clear();
		card_items[1].clear();
		card_items[2].clear();
		card_items[3].clear();
		card_items[4].clear();
	}

	// инициализировать карточку (без сторон задания сторон): создать панель + загрузить картинку
	void 		init_paint(TComponent* Owner, TImageList * image_list, int index, TImage * image_target_enter, TImage * image_target_leave)
	{
		Panel = new TPanel(Owner);
		Image = new TImage(Panel);

		Panel->Parent   = (TWinControl *)Owner;
		Panel->Left     = 0;
		Panel->Top      = 0;
		Panel->Width    = CARD_WIDTH;
		Panel->Height   = CARD_HEIGHT;

		Image->Parent   = Panel;
		Image->Top      = 0;
		Image->Left     = 0;
		Image->AutoSize = true;

		image_list->GetBitmap(index, Image->Picture->Bitmap);

		Panel->Visible  = false;
	}

	// переместить игровую карточку в нужное место
	void 		move_to(int x, int y, bool is_visible)
	{
		Panel->Left     = x;
		Panel->Top      = y;
		Panel->Visible  = is_visible;
	}

	// повернуть карточку на 90 градусов по часовой стрелке
	void 		rotate()
	{
		A_CardItem		tmp;

		Graphics::TBitmap *SrcBitmap  = new Graphics::TBitmap();
		Graphics::TBitmap *DestBitmap = new Graphics::TBitmap();

		SrcBitmap->Assign(Image->Picture->Bitmap);

		DestBitmap->Width  = SrcBitmap->Height;
		DestBitmap->Height = SrcBitmap->Width;

		// вращение по часовой стрелке

		for (int x=0;x<SrcBitmap->Width;x++)
		{
			for(int y=0;y<SrcBitmap->Height;y++)
			{
				DestBitmap->Canvas->Pixels[SrcBitmap->Width - 1 - y][x] = SrcBitmap->Canvas->Pixels[x][y];
			}
		}

		// новое изображение

		Image->Picture->Bitmap = DestBitmap;

		delete DestBitmap;
		delete SrcBitmap;

		// вращаем координаты фишек

		card_items[0].rotate(); // сверху
		card_items[1].rotate(); // справа
		card_items[2].rotate(); // снизу
		card_items[3].rotate(); // слева

		// вращаем игровые элементы

		tmp = card_items[0];
		card_items[0] = card_items[3];
		card_items[3] = card_items[2];
		card_items[2] = card_items[1];
		card_items[1] = tmp;
	}
};
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// КЛАСС набора карт
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
class A_Cards
{
public:
	// конструктор (начальная инициализация игровой колоды карточек)
	A_Cards();

	// деструктор
	~A_Cards();

	// очистить все игровые карточки
	void    clear();

	// инициализация игровых карточек
	bool	init(TComponent* Owner, TImageList * image_list, int ccards_count, TImage * image_target_enter, TImage * image_target_leave);

	// перемешать карточки
	void    shuffle_cards();

	// количество карточек в игре
	int		cards_count;

	// игровые карточки
	A_Card	card[CARDS_COUNT_MAX];

	// индексы игровых карт: [1; 73]
	// введены индексы для того чтобы можно было перемешивать карточки
	// 0 и 29 карточки не перемешивают (0-рубашка карт, 29 - начальная игровая карточка)
	int     indexes[74];
};
// ----------------------------------------------------------------------------
inline
A_Cards::A_Cards()
{
	// инициализируем генератор случайных числе
	randomize();

	// очистить все карточки
	clear();
}
// ----------------------------------------------------------------------------
inline
A_Cards::~A_Cards()
{
	// ничего не надо делать, т.к. главная форма сама удалит все графические элементы
}
// ----------------------------------------------------------------------------
// очистить все игровые карточки
// ----------------------------------------------------------------------------
void
A_Cards::clear()
{
	int i;
	// очищаем каждую игровую карточку
	for (i = 0; i < CARDS_COUNT_MAX; i++)
	{
		card[i].clear();
	}
	// теперь ни одна карточка не задана, нужна инициализация
	cards_count = 0;
}
// ----------------------------------------------------------------------------
// инициализация игровых карточек
// ----------------------------------------------------------------------------
inline bool
A_Cards::init(TComponent* Owner, TImageList * image_list, int ccards_count, TImage * image_target_enter, TImage * image_target_leave)
{
	int	index;

	cards_count = 0;

	if (image_list->Count != ccards_count)
		return false;

	cards_count = ccards_count;
	// инициализация картинок на карточках
	for (index = 0; index < cards_count; index++)
	{
		card[index].clear();
		card[index].init_paint(Owner, image_list, index, image_target_enter, image_target_leave);
	}

	// 0 карта - это оборотная сторона

	// 1 тип карточки: с [1 по 8]
	// нет суши, нет рынка
	// у всех карточек на дороге ракушка, кроме 1 карточки
	for (index = 1; index < 9; ++index)
	{
		card[index].card_items[0].set_type(1, 3, 1); // сверху вниз дорога
		card[index].card_items[1].set_type(0, 0, 0); // справа ничего нет
		card[index].card_items[2].set_type(1, 3, 1); // снизу вверх дорога
		card[index].card_items[3].set_type(0, 0, 0); // слева ничего нет

		card[index].card_items[0].set_xy(42,25);
	}
	//
	card[1].card_items[0].item_price = 0;
	card[1].card_items[2].item_price = 0;

	card[3].card_items[0].set_xy(28,52);	// на 3,4,5 карточке координаты меняются
	card[4].card_items[0].set_xy(28,52);	// на 3,4,5 карточке координаты меняются
	card[5].card_items[0].set_xy(28,52);	// на 3,4,5 карточке координаты меняются

	// + 2 тип карточек c [9 по 13]
	// нет суши, нет рынка
	// у всех карточках на дороге по 1 ракушке
	for (index = 9; index < 14; ++index)
	{
		card[index].card_items[0].set_type(1, 4, 1);	// сверху вправо дорога
		card[index].card_items[1].set_type(1, 5, 1);	// справа вверх дорога
		card[index].card_items[2].set_type(0, 0, 0);	// снизу ничего нет
		card[index].card_items[3].set_type(0, 0, 0);	// слева ничего нет

		card[index].card_items[0].set_xy(40,20);
	}

	// + 3 тип карточек с [14 по 22]
	// нет суши, нет рынка
	for (index = 14; index < 23; ++index)
	{
		card[index].card_items[0].set_type(1, 2, 0);	// сверху
		card[index].card_items[1].set_type(1, 2, 0);	// справа
		card[index].card_items[2].set_type(0, 0, 0);	// снизу ничего нет
		card[index].card_items[3].set_type(1, 2, 0);	// слева

		card[index].card_items[0].set_xy(44, 8);
		card[index].card_items[1].set_xy(78, 43);
		card[index].card_items[3].set_xy(8, 43);
	}

	card[17].card_items[1].item_price = 1;	// справа ракушка
	card[17].card_items[3].item_price = 1;	// слева ракушка

	card[19].card_items[3].item_price = 1;	// слева ракушка

	card[20].card_items[1].item_price = 1;	// справа ракушка

	card[22].card_items[0].item_price = 1;	// сверху ракушка

	// + 4 тип карточек [23 - 1 штука]
	// нет суши, нет рынка, ловля рыбы в 6-7 секторе
	index = 23;
	card[index].card_items[0].set_type(1, 2, 0);	// сверху
	card[index].card_items[1].set_type(1, 2, 0);	// справа
	card[index].card_items[2].set_type(1, 2, 1);	// снизу ракушка
	card[index].card_items[3].set_type(1, 2, 0);	// слева

	card[index].card_items[0].set_xy(43,9);
	card[index].card_items[1].set_xy(76,42);
	card[index].card_items[2].set_xy(43,76);
	card[index].card_items[3].set_xy(9,42);

	// + 5 тип карточек с [24 по 28]
	// нет дорог, нет рынка
	// везде есть банан, кроме 24 карточки
	// море занимает все пространство слева, вверху и справо
	for (index = 24; index < 29; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// сверху ничего нет
		card[index].card_items[1].set_type(0, 0, 0);	// справа ничего нет
		card[index].card_items[2].set_type(2, 1, 1);	// снизу суша с бананом
		card[index].card_items[3].set_type(0, 0, 0);	// слева ничего нет

		card[index].card_items[2].set_xy(32, 49);		// снизу
	}
	card[24].card_items[2].item_price = 0;	// внизу банана нет

	// + 6 тип карточек c [29 по 31]
	// нет рынка
	// везде есть на дороге ракушка,
	// везде есть банан, кроме 31 карточки
	for (index = 29; index < 32; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// сверху ни чего нет
		card[index].card_items[1].set_type(1, 3, 1);	// справа дорога на противополоную сторону с ракушкой
		card[index].card_items[2].set_type(2, 1, 1);	// снизу суша с бананом
		card[index].card_items[3].set_type(1, 3, 1);	// слева дорога на противоположную сторону с ракушкой

		card[index].card_items[1].set_xy(13, 38);	// справа можно захватить дорогу
		card[index].card_items[2].set_xy(22, 74);	// снизу можно захватить сушу
	}

	card[31].card_items[2].item_price = 0;	// снизу на суши банана нет

	// + 7 тип карточек с [32 по 33]
	// нет рынка
	// везде есть ракушка
	// ни где нет ни ловли ни рыбок
	for (index = 32; index < 34; ++index)
	{
		card[index].card_items[0].set_type(1, 5, 1);	// сверху налево дорога с ракушкой
		card[index].card_items[1].set_type(2, 1, 0);	// справа суша
		card[index].card_items[2].set_type(0, 0, 0);	// снизу ничего нет
		card[index].card_items[3].set_type(1, 4, 1);	// слева наверх дорога с ракушкой

		card[index].card_items[0].set_xy(9, 42);	// слева на право можно захватить
		card[index].card_items[1].set_xy(76, 63);	// cправа суша, ее можно захватить
	}

	// + 8 тип карточек с [34 по 35]
	// нет рынка
	// везде есть ракушка
	// банан на 34 карточке
	// ни где нет ловли и рыбок
	for (index = 34; index < 36; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// сверху ничего нет
		card[index].card_items[1].set_type(2, 1, 0);	// справа суша тупик
		card[index].card_items[2].set_type(1, 4, 1);	// снизу налево дорога с ракушкой
		card[index].card_items[3].set_type(1, 5, 1);	// слева вниз дорога с ракушкой

		card[index].card_items[3].set_xy(9, 42);		// слева на право можно захватить
		card[index].card_items[1].set_xy(76, 63);		// cправа суша, ее можно захватить
	}
	card[34].card_items[1].item_price = 1;				// справа на суше банан

	// + 9 тип карточек с [36 по 38]
	// нет рынка
	// нет бананов на суше
	for (index = 36; index < 39; ++index)
	{
		card[index].card_items[0].set_type(1, 2, 0);	// сверху дорога
		card[index].card_items[1].set_type(2, 1, 0);	// справа суша тупик
		card[index].card_items[2].set_type(1, 2, 0);	// снизу дорога
		card[index].card_items[3].set_type(1, 2, 0);	// слева дорога

		card[index].card_items[0].set_xy(44, 9);		// сверху
		card[index].card_items[1].set_xy(77, 63);		// справа
		card[index].card_items[2].set_xy(44, 76);		// снизу
		card[index].card_items[3].set_xy(16, 41);		// слева
	}

	card[36].card_items[0].item_price = 1;	// сверху ракушка
	card[37].card_items[0].item_price = 1;	// сверху ракушка
	card[38].card_items[2].item_price = 1;	// снизу ракушка

	// + 10 тип карточек [39]
	// нет рынка
	// ракушка на дороге снизу
	// нет ни ловли ни рыбок
	index = 39;
	card[index].card_items[0].set_type(1, 2, 0);	// сверху дорога тупик
	card[index].card_items[1].set_type(2, 1, 0);	// справа суша
	card[index].card_items[2].set_type(1, 2, 1);	// снизу дорога тупик
	card[index].card_items[3].set_type(0, 0, 0);	// слева ничего нет

	card[index].card_items[0].set_xy(44, 9);	// сверху
	card[index].card_items[1].set_xy(75, 54);	// справа
	card[index].card_items[2].set_xy(44, 76);	// снизу

	// + 11 тип карточек с [40 по 41]
	// нет рынка
	// везде банан на суше
	// на 41 карточке снизу ракушка
	for (index = 40; index < 42; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// сверху ничего нет
		card[index].card_items[1].set_type(2, 1, 1);	// справа суша с бананом
		card[index].card_items[2].set_type(1, 2, 0);	// снизу дорога тупик
		card[index].card_items[3].set_type(1, 2, 0);	// слева дорога тупик

		card[index].card_items[1].set_xy(78, 57);	// справа
		card[index].card_items[2].set_xy(44, 76);	// снизу
		card[index].card_items[3].set_xy(16, 41);	// слева
	}
	card[41].card_items[2].item_price = 1;			// cнизу ракушка

	// + 12 тип карточек с [42 по 44]
	// нет дорог, нет рынка
	// слева везде по банану, справа только на 43 карточке
	// на всех карточках по 1 рыбке (сверху вниз)
	for (index = 42; index < 45; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// сверху ничего нет
		card[index].card_items[1].set_type(2, 1, 0);	// справа суша
		card[index].card_items[2].set_type(0, 0, 0);	// снизу ничего нет
		card[index].card_items[3].set_type(2, 1, 1);	// слева суша

		card[index].card_items[1].set_xy(77, 59);		// справа
		card[index].card_items[3].set_xy(9, 61);		// слева
	}
	card[43].card_items[1].item_price = 1;			   	// справа банан

	// + 13 тип карточек с [45 по 47]
	// нет дорог, нет рынка
	// внизу на всех карточках нет банана, справа есть
	// на всех карточках нет рыбы и ловки
	for (index = 45; index < 48; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// cверху ничего нет
		card[index].card_items[1].set_type(2, 1, 1);	// справа суша с бананом
		card[index].card_items[2].set_type(2, 1, 0);	// снизу суша
		card[index].card_items[3].set_type(0, 0, 0);	// слева ничего нет

		card[index].card_items[1].set_xy(77, 53);		// справа
		card[index].card_items[2].set_xy(28, 73);		// снизу
	}

	// + 14 тип карточек c [48 по 50]
	// нет дорог, нет рынка
	// на суше везде по 1 банану
	for (index = 48; index < 51; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// сверху ничего нет
		card[index].card_items[1].set_type(2, 2, 1);	// справа суша
		card[index].card_items[2].set_type(0, 0, 0);	// снизу ничего нет
		card[index].card_items[3].set_type(2, 2, 1);	// слева суша

		card[index].card_items[1].set_xy(40, 42);		// справа
	}

	// + 15 карта c [51 по 52]
	// нет дорог, нет рынка
	// слева бананов нет, справа и снизу по 1 банану
	// нет рыбы и ловли на всех карточках
	for (index = 51; index < 53; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// сверху ничего нет
		card[index].card_items[1].set_type(2, 3, 1);	// справа вниз суша с бананом
		card[index].card_items[2].set_type(2, 4, 1);	// снизу вправо суша с бананом
		card[index].card_items[3].set_type(2, 1, 0);	// слева суша тупик

		card[index].card_items[1].set_xy(75, 30);		// справа вниз можно захватить сушу
		card[index].card_items[3].set_xy(8, 25);		// слева можно захватить сушу
	}

	// + 16 тип карточек с [53 по 58]
	// нет дорог, нет рынка
	// на суше везде по 1 банану
	for (index = 53; index < 59; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// сверху
		card[index].card_items[1].set_type(2, 3, 1);	// справа вниз суша с бананом
		card[index].card_items[2].set_type(2, 4, 1);	// снизу вправо суша с бананом
		card[index].card_items[3].set_type(0, 0, 0);	// слева

		card[index].card_items[1].set_xy(69, 36);		// справа вниз можно захватить
	}

	// + 17 тип карточек с [59 по 63]
	// нет рынка
	// на всех карточках по 1 банану, кроме 63 карточки
	// на 62 и 63 карточках на дороге слева по ракушке
	// нет рыбы и ловли
	for (index = 59; index < 64; ++index)
	{
		card[index].card_items[0].set_type(1, 2, 0);	// сверху дорога тупик
		card[index].card_items[1].set_type(2, 3, 1);	// справа вниз суша
		card[index].card_items[2].set_type(2, 4, 1);	// снизу вправо суша
		card[index].card_items[3].set_type(1, 2, 0);	// слева дорога тупик

		card[index].card_items[0].set_xy(44, 12);		// сверху
		card[index].card_items[1].set_xy(36, 74);		// справа вниз можно захватить сушу
		card[index].card_items[3].set_xy(9, 44);		// слева
	}

	card[63].card_items[1].item_price = 0;	// справа нет банана
	card[63].card_items[2].item_price = 0;	// снизу нет банана

	card[62].card_items[3].item_price = 1;	// слева есть ракушка
	card[63].card_items[3].item_price = 1;	// слева есть ракушка

	// + 18 тип карточки [64]
	// нет дорог, нет рынка
	// нет рыбы и ловли
	index = 64;
	card[index].card_items[0].set_type(0, 0, 0);	// сверху ничего нет
	card[index].card_items[1].set_type(2, 5, 2);	// справа суша
	card[index].card_items[2].set_type(2, 7, 2);	// снизу суша
	card[index].card_items[3].set_type(2, 6, 2);	// слева суша

	card[index].card_items[1].set_xy(14, 74);		// справа / снизу / слева можно захватить сушу

	// + 19 тип карточки с [65 по 67]
	// нет рынка
	// на суше по 2 банана везде, кроме 67 - на ней 1 банан
	// на 66 карточке на дороге сверху ракушка
	// нет рыбы и ловли
	for (index = 65; index < 68; ++index)
	{
		card[index].card_items[0].set_type(1, 2, 0);	// сверху дорога тупик
		card[index].card_items[1].set_type(2, 5, 2);	// справа суша
		card[index].card_items[2].set_type(2, 7, 2);	// снизу суша
		card[index].card_items[3].set_type(2, 6, 2);	// слева суша

		card[index].card_items[0].set_xy(41, 9);		// сверху
		card[index].card_items[2].set_xy(14, 74);		// снизу
	}
	card[66].card_items[0].item_price = 1;	// сверху на дороге ракушка

	card[67].card_items[1].item_price = 1;	// справа 1 банан
	card[67].card_items[2].item_price = 1;	// снизу 1 банан
	card[67].card_items[3].item_price = 1;	// слева 1 банан

	// !!! далее на карточках нет ничего дополнительного !!!
	// !!! нет бананов, ракушек, рыбок, нельзя ловить рыбу !!!

	// 20 карта (нет островов) - 68 карточка
	index = 68;
	card[index].card_items[0].set_type(1, 2, 0);		// сверху дорога
	card[index].card_items[1].set_type(1, 2, 0);		// справа дорога
	card[index].card_items[2].set_type(1, 2, 0);		// снизу дорога
	card[index].card_items[3].set_type(0, 0, 0);		// слева ничего нет
	card[index].card_items[4].set_type(3, 0, 0);    	// рынок присутствует (тип всегда один и тотже 2)

	card[index].card_items[0].set_xy(42, 9);			// сверху
	card[index].card_items[1].set_xy(79, 42);			// справа
	card[index].card_items[2].set_xy(42, 78);			// снизу
	card[index].card_items[4].set_xy(42, 42);

	// 21 карта (нет островов) - 69 карточка
	index = 69;
	card[index].card_items[0].set_type(1, 2, 0);		// сверху дорога тупик
	card[index].card_items[1].set_type(0, 0, 0);		// справа ничего нет
	card[index].card_items[2].set_type(1, 2, 0);		// снизу дорога тупик
	card[index].card_items[3].set_type(0, 0, 0);		// слева ничего нет
	card[index].card_items[4].set_type(3, 0, 0);    	// рынок присутствует (тип всегда один и тотже 2)

	card[index].card_items[0].set_xy(42, 9);	// сверху
	card[index].card_items[2].set_xy(42, 78);	// снизу
	card[index].card_items[4].set_xy(42,42);

	// 22 карта (нет островов) с [70 по 71]
	for (index = 70; index < 72; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);		// сверху
		card[index].card_items[1].set_type(1, 2, 0);		// справа
		card[index].card_items[2].set_type(0, 0, 0);		// снизу
		card[index].card_items[3].set_type(0, 0, 0);		// слева
		card[index].card_items[4].set_type(3, 0, 0);    	// рынок присутствует (тип всегда один и тотже 2)

		card[index].card_items[1].set_xy(76, 42);			// справа
		card[index].card_items[4].set_xy(42, 42);            // рынок
	}

	// 23 карта (нет островов) - 72 карточка
	index = 72;
	card[index].card_items[0].set_type(1, 2, 0);		// сверху дорога
	card[index].card_items[1].set_type(1, 2, 0);		// справа дорога
	card[index].card_items[2].set_type(0, 0, 0);		// снизу ничего нет
	card[index].card_items[3].set_type(0, 0, 0);		// слева ничего нет
	card[index].card_items[4].set_type(3, 0, 0);    	// рынок присутствует (тип всегда один и тотже 2)

	card[index].card_items[0].set_xy(44, 9);	// сверху
	card[index].card_items[1].set_xy(79, 42);	// справа
	card[index].card_items[4].set_xy(42, 42);

	// 24 карта (нет дорог, нет островов) - 73 карточка
	index = 73;
	card[index].card_items[4].set_type(3, 0, 0);    // рынок присутствует
	card[index].card_items[4].set_xy(42, 42);

	return true;
}
// ----------------------------------------------------------------------------
// перемешать карточки
// ----------------------------------------------------------------------------
void
A_Cards::shuffle_cards()
{
	int 	i, n;
	int     rnd_count, rnd_index_1, rnd_index_2, index_tmp;

	// число перестановок: [200; 300]
	rnd_count = 200 + (rand() % 101);

	// начальная инициализация массива индексов игровых карточек
	n = 74;
	for (i=0;i<n;i++)
	{
		indexes[i] = i;
	}

	// переставляем индексы игровых карточек
	for (i=0;i<rnd_count;i++)
	{
		rnd_index_1 = 1 + (rand() % 73);
		rnd_index_2 = 1 + (rand() % 73);

		if (rnd_index_1 == 0) rnd_index_1 = 1;
		if (rnd_index_2 == 0) rnd_index_2 = 1;

		if (rnd_index_1 == 29) rnd_index_1 = 30;
		if (rnd_index_2 == 29) rnd_index_2 = 30;

		index_tmp = indexes[rnd_index_1];
		indexes[rnd_index_1] = indexes[rnd_index_2];
		indexes[rnd_index_2] = index_tmp;
	}

}
// ----------------------------------------------------------------------------





#endif
// ----------------------------------------------------------------------------

