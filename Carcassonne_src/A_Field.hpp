#ifndef _A_FIELD_HPP_
#define _A_FIELD_HPP_
// ----------------------------------------------------------------------------
#include "A_Cards.hpp"
#include <set>
// ----------------------------------------------------------------------------
#define FIELDS_COUNT	100
// ----------------------------------------------------------------------------
// Элемент игрового поля (ячейка игрового поля)
// ----------------------------------------------------------------------------
struct A_FieldItem
{
	// в ячейке игрового поля лежит карточка
	A_Card * card;

	// конструктор (начальная инициализация: ничего не лежит в ячейке)
	A_FieldItem() { clear(); }

	// очистка ячейки игрового поля (ничего не лежит в ячейке)
	void clear() { card = NULL; }
};
// ----------------------------------------------------------------------------
class A_Field
{
public:
	// конструктор (начальная инициализация игрового поля)
	A_Field();

	// деструктор
	~A_Field();

	// очистить игровое поле (на нем не лежит ни одна карточка)
	void clear();

	// положить на игровое поле карточку
	bool set_card_on_field(int x, int y, A_Card * _card);

	// взять выпавшую карту
	void set_card_cur(A_Card * _card = NULL);

	// проверить можно ли в квадрат игрового поля положить текущую карточку
	bool can_set_card(int x, int y);

	// добавить игровой столбец справа / слева
	bool field_add_column_right();
	bool field_add_column_left();

	// добавить игровую строку сверху / снизу
	bool field_add_row_top();
	bool field_add_row_bottom();

	// игровое поле (массив)
	A_FieldItem 	items[FIELDS_COUNT][FIELDS_COUNT];


	// карта проверенных карточек
	char        	checkmap[FIELDS_COUNT][FIELDS_COUNT];

	// обнулить карту проверенных карточек
	void            checkmap_zero();

	// рекурсивная функция проверки занятости элементов игрового поля
	// на выходе - если true, то этот игровой элемент не занят,
	//             если false, то игровой элемент уже занят игроком
	bool 			checkmap_check_at(int x, int y, int side);
	int             checkmap_roads_count;   // колличество окончаний дорог, их не может быть больше 2, если 2- она закрыта
	bool            checkmap_solution;      // это решение, можно захватытвать или нет
	int             checkmap_points;        // это количество очков на захваченной (захватываемой) терртории
	bool            checkmap_land_is_close; // суша вся закрыта со всех сторон?
	bool            checkmap_market_is_close;   // рынок закрыт со всех сторон?
	std::set<unsigned int> checkmap_UserID;     // это массив c идентификаторами пользователей, которые заняли локации


	// проверить есть ли земля по координатам игрового поля с определенной стороны
	bool            has_land_at(int x, int y, int side);
	// проверить есть ли карточка по координатам игрового поля
	bool            has_card(int x, int y);

	// число элементов по горизонтали и вертикали
	int				x_count;
	int 			y_count;

	// текущая выбранная карточка игроком (это та карточка, которую игрок должен положить на игровое поле)
	A_Card *		card_cur;
};
// ----------------------------------------------------------------------------
inline
A_Field::A_Field()
{
	clear();
}
// ----------------------------------------------------------------------------
inline
A_Field::~A_Field()
{

}
// ----------------------------------------------------------------------------
// очистить игровое поле (на нем не лежит ни одна карточка)
// ----------------------------------------------------------------------------
inline void
A_Field::clear()
{
	int x, y, n;

	n = FIELDS_COUNT;
	for (x = 0; x < n; x++)
	for (y = 0; y < n; y++)
	{
		items[x][y].clear();
	}

	x_count  = 3;
	y_count  = 3;
	card_cur = NULL;
}
// ----------------------------------------------------------------------------
// положить на игровое поле карточку
// ----------------------------------------------------------------------------
inline bool
A_Field::set_card_on_field(int x, int y, A_Card * _card)
{
	items[x][y].card = _card;
	items[x][y].card->on_field = true;
}
// ----------------------------------------------------------------------------
// текущая выбранная карточка игроком
// ----------------------------------------------------------------------------
inline void
A_Field::set_card_cur(A_Card * _card)
{
	card_cur = _card;
}
// ----------------------------------------------------------------------------
// проверить можно ли в квадрат игрового поля положить текущую карточку
// ----------------------------------------------------------------------------
inline bool
A_Field::can_set_card(int x, int y)
{
	bool		is_ok;
	A_Card *	card_friend;

	// карточка не выбрана
	if (card_cur == NULL)
		return false;

	// выход за пределы игрового поля
	if ((x < 0) || (y < 0))
		return false;
	if ((x >= x_count) || (y >= y_count))
		return false;

	// проверка того, что карточка касается хотя бы с одной стороны другой карточки
	is_ok = false;
	// проверяем стыковку с левой стороны
	if (x > 0)
		if (items[x-1][y].card != NULL) is_ok = true;

	// проверяем стыковку с правой стороны
	if (x < x_count-1)
		if (items[x+1][y].card != NULL) is_ok = true;

	// проверяем стыковку с верхней стороны
	if (y > 0)
		if (items[x][y-1].card != NULL) is_ok = true;

	// проверяем стыковку с нижней стороны
	if (y < y_count-1)
		if (items[x][y+1].card != NULL) is_ok = true;

	// если карточка не соприкасается ни с одной из сторон, то так нельзя по правилам
	if (is_ok == false)
		return false;

	// проверки правильности стыковки с левой стороны, должны совпадать типы элементов
	if (x > 0)
	{
		is_ok = true;
		// карточка с левой стороны присутсвует, а мы справа от нее -
		// это соприкосновения с левой стороны стороны 1 (справа) с нашей стороны - 3 (слева)
		if (items[x-1][y].card != NULL)
		{
			if (items[x-1][y].card->card_items[1].item_type != card_cur->card_items[3].item_type)
				return false;
		}
	}

	// проверяем стыковку с правой стороны
	if (x < x_count-1)
	{
		// карточка с правой стороны присутсвует, а мы слева от нее -
		// это соприкосновения с правой стороны стороны 3 (слева) с нашей стороны - 1 (справа)
		if (items[x+1][y].card != NULL)
		{
			if (items[x+1][y].card->card_items[3].item_type != card_cur->card_items[1].item_type)
				return false;
		}
	}

	// проверяем стыковку с верхней стороны
	if (y > 0)
	{
		// карточка с верхней стороны присутсвует, а мы снизу от нее -
		// это соприкосновения с верхней стороны надо взять 2 (снизу) с нашей стороны - 0 (сверху)
		if (items[x][y-1].card != NULL)
		{
			if (items[x][y-1].card->card_items[2].item_type != card_cur->card_items[0].item_type)
				return false;
		}
	}

	// проверяем стыковку с нижней стороны
	if (y < y_count-1)
	{
		// карточка с нижней стороны присутсвует, а мы сверху от нее -
		// это соприкосновения с нижней стороны 2 ( с нашей стороны - 3
		if (items[x][y+1].card != NULL)
		{
			if (items[x][y+1].card->card_items[0].item_type != card_cur->card_items[2].item_type)
				return false;
		}
	}

	return true;
}
// ----------------------------------------------------------------------------
// добавить игровой столбец справа / слева
// ----------------------------------------------------------------------------
inline bool
A_Field::field_add_column_right()
{
	int x, y;

	if (x_count >= FIELDS_COUNT)
		return false;

	x_count = x_count + 1;
	return true;
}
// ----------------------------------------------------------------------------
inline bool
A_Field::field_add_column_left()
{
	int x, y;

	if (x_count >= FIELDS_COUNT)
		return false;

	// сдвигаем все игровое поле вправо
	for (x = x_count; x > 0; x--)
	for (y = y_count; y > 0; y--)
	{
		items[x][y] = items[x-1][y];
	}

	// 0 столбец обнуляем
	for (y = 0; y < y_count; y++)
	{
		items[0][y].clear();
	}

	x_count = x_count + 1;
	return true;
}
// ----------------------------------------------------------------------------
// добавить игровую строку сверху / снизу
// ----------------------------------------------------------------------------
inline bool
A_Field::field_add_row_top()
{
	int x, y;

	if (y_count >= FIELDS_COUNT)
		return false;

	// сдвигаем все игровое поле вправо
	for (x = x_count; x > 0; x--)
	for (y = y_count; y > 0; y--)
	{
		items[x][y] = items[x][y-1];
	}

	// 0 строку обнуляем
	for (x = 0; x < x_count; x++)
	{
		items[x][0].clear();
	}

	y_count = y_count + 1;
	return true;
}
// ----------------------------------------------------------------------------
inline bool
A_Field::field_add_row_bottom()
{
	if (y_count >= FIELDS_COUNT)
		return false;

	y_count = y_count + 1;
	return true;
}

// ----------------------------------------------------------------------------
// обнулить карту проверенных карточек
// ----------------------------------------------------------------------------
void
A_Field::checkmap_zero()
{
	int x, y;

	checkmap_roads_count = 0;   	// концов дорог 0
	checkmap_solution    = true;   	// решение - можно захватывать
	checkmap_points      = 0;       // сколько бананов или ракушек на замкнутой территории
	checkmap_land_is_close = true; 	// земля замкнутая? по умолчанию считаем, что замкнута,
									// в процессе проверки как только одна сторона голая - незамкнутая тогда
	checkmap_market_is_close = true;// рынок со всех сторон окружен?

	checkmap_UserID.clear();

	// карта проверенных карточек
	for (x = 0; x < x_count; x++)
	for (y = 0; y < y_count; y++)
		checkmap[x][y] = 0;
}
// ----------------------------------------------------------------------------
// рекурсивная функция проверки занятости элементов игрового поля каким-то игроком
// на выходе - если true, то этот игровой элемент не занят,
//             если false, то игровой элемент уже занят игроком
// ----------------------------------------------------------------------------
bool
A_Field::checkmap_check_at(int x, int y, int side)
{
	A_Card *    card;
	int         item_type, item_connection, side_check;

	// если обращаемся за пределы поля, то строить карту соприкосновения элементов игры нельзя
	// считаем, что никем не занято
	if (x < 0) 			return checkmap_solution;
	if (y < 0) 			return checkmap_solution;
	if (x >= x_count) 	return checkmap_solution;
	if (y >= y_count) 	return checkmap_solution;

	// если в этой клетке уже были, то выход
	if (checkmap[x][y] == 1)
		return checkmap_solution;

	// помечаем клетку как проверенную
	checkmap[x][y] = 1;

	// если в этой клетке нет карточки, то тоже заканчиваем построение карты - не занято
	card = items[x][y].card;
	if (card == NULL) return checkmap_solution;

	// если кем-то занято, то решение по захвату отрецательное
	if (card->card_items[side].UserID != 0)
	{
		checkmap_solution = 0;  // захватывать нельзя
		checkmap_UserID.insert(card->card_items[side].UserID); // добавление id в сет
	}

	// side - это сторона карточки, 0-3 (верх, справа, внизу, слева), 4 - это рынок
	// если рынок не занят, то его точно можно занять без всяких проверок
	if (side == 4)
	{
		// проверка наличия карточек со всех сторон
		if (has_card(x-1, y-1) == false) checkmap_market_is_close = false;  // слева вверху
		if (has_card(x-1, y)   == false) checkmap_market_is_close = false;  // слева
		if (has_card(x-1, y+1) == false) checkmap_market_is_close = false;  // слева внизу

		if (has_card(x, y-1)   == false) checkmap_market_is_close = false;  // сверху
		if (has_card(x, y+1)   == false) checkmap_market_is_close = false;  // снизу

		if (has_card(x+1, y-1) == false) checkmap_market_is_close = false;  // справа вверху
		if (has_card(x+1, y)   == false) checkmap_market_is_close = false;  // справа
		if (has_card(x+1, y+1) == false) checkmap_market_is_close = false;  // справа снизу

		return checkmap_solution;
	}

	// анализируем всю карточку
	// дороги:
	// суть анализа сводится к прохождению всего маршрута во все стороны от дороги,
	// если есть соединения дорог между собой. При этом, если на всем пути мы встретили 2 тупика,
	// то можно утверждать, что дорога полностью заблокирована со всех сторон и можно заработать очки и снять фишку

	item_type = card->card_items[side].item_type;
	item_connection = card->card_items[side].item_connection;
	checkmap_points = checkmap_points + card->card_items[side].item_price;

	// --------------
	// проверка дорог
	// --------------

	if (item_type == 1)
	{
		if (item_connection < 2)
			return checkmap_solution;

		// ----------------------------------
		// тупик
		// ----------------------------------

		if (item_connection == 2)  //тупик
		{
			checkmap_roads_count = checkmap_roads_count + 1; // тупики
			//проверяем в другую сторону от тупика
			switch(side)
			{
			case 0:
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				break;
			case 1:
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				break;
			case 2:
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				break;
			case 3:
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				break;
			}
		}

		// ----------------------------------
		// соединено c противоложной стороной
		// ----------------------------------

		if (item_connection == 3) //соединено c противоложной стороной
		{
			switch(side)
			{
			case 0:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				break;
			case 1:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				break;
			case 2:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				break;
			case 3:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				break;
			}
		}
		// ----------------------------------
		// соединено с правой стороной
		// ----------------------------------
		if (item_connection == 4) // соединено с правой стороной
		{
			switch(side)
			{
			case 0:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				break;
			case 1:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				break;
			case 2:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				break;
			case 3:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				break;
			}
		}

		// ----------------------------------
		// соединено с левой стороной
		// ----------------------------------
		if (item_connection == 5)  // соединено с левой стороной
		{
			switch(side)
			{
			case 0:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				break;
			case 1:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				break;
			case 2:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				break;
			case 3:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				break;
			}
		}

	}

	// ----------------------------------
	// проверка суши
	// ----------------------------------

	if (item_type == 2) // проверка суши
	{
		if (item_connection == 0)
			return checkmap_solution;
		// ----------------------------------
		// тупик
		// ----------------------------------
		if (item_connection == 1) // тупик
		{
			switch(side)  // проверяем в другую сторону от тупика
			{
			case 0:
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 1:
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			case 2:
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 3:
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			}
		}
		// ----------------------------------
		// соединено c противоложной стороной
		// ----------------------------------
		if (item_connection == 2)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			}
		}
		// ----------------------------------
		// соединено с правой стороной
		// ----------------------------------
		if (item_connection == 3)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			}
		}

		// ----------------------------------
		// соединено с левой стороной
		// ----------------------------------

		if (item_connection == 4)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			}
		}

		// ----------------------------------
		// соединено с правой и противоположной
		// ----------------------------------
		if (item_connection == 5)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			}
		}

		// ----------------------------------
		// соединено слева и противоположной
		// ----------------------------------
		if (item_connection == 6)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			}
		}
		// ----------------------------------
		// соединено с боковыми
		// ----------------------------------
		if (item_connection == 7)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // проверка карточки справа
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // проверка карточки сверху
				checkmap_check_at(x, y+1, 0);    // проверка карточки снизу
				checkmap_check_at(x-1, y, 1);    // проверка карточки слева
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			}
		}
	}

	return checkmap_solution;
}
// ----------------------------------------------------------------------------
// проверить есть ли земля по координатам игрового поля с определенной стороны
// ----------------------------------------------------------------------------
inline bool
A_Field::has_land_at(int x, int y, int side)
{
	A_Card *    card;

	// проверки на корректность работы алгоритма
	// 1) если side = 4 - это рынок
	if (side == 4)
		return false;

	// 2) если по какой-то причине вышли за пределы игрового поля,
	//    то это тоже ошибка и надо проверять алгоритм работы
	if ((x < 0) || (y < 0) || (x >= x_count) || (y >= y_count))
		return false;

	// если в этой клетке нет карточки, то земли нет
	card = items[x][y].card;
	if (card == NULL) return false;

	// проверяем сторону (если земля, то все ок)
	if (card->card_items[side].item_type == 2)
		return true;

	return checkmap_solution;
}
// ----------------------------------------------------------------------------
// проверить есть ли карточка по координатам игрового поля
// ----------------------------------------------------------------------------
inline bool
A_Field::has_card(int x, int y)
{
	A_Card *    card;

	// 2) если по какой-то причине вышли за пределы игрового поля,
	//    то это тоже ошибка и надо проверять алгоритм работы
	if ((x < 0) || (y < 0) || (x >= x_count) || (y >= y_count))
		return false;

	// если в этой клетке нет карточки, то земли нет
	card = items[x][y].card;
	if (card == NULL) return false;

	return true;
}
// ----------------------------------------------------------------------------


#endif
