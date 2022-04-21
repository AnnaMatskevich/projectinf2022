#ifndef _A_FIELD_HPP_
#define _A_FIELD_HPP_
// ----------------------------------------------------------------------------
#include "A_Cards.hpp"
#include <set>
// ----------------------------------------------------------------------------
#define FIELDS_COUNT	100
// ----------------------------------------------------------------------------
// ������� �������� ���� (������ �������� ����)
// ----------------------------------------------------------------------------
struct A_FieldItem
{
	// � ������ �������� ���� ����� ��������
	A_Card * card;

	// ����������� (��������� �������������: ������ �� ����� � ������)
	A_FieldItem() { clear(); }

	// ������� ������ �������� ���� (������ �� ����� � ������)
	void clear() { card = NULL; }
};
// ----------------------------------------------------------------------------
class A_Field
{
public:
	// ����������� (��������� ������������� �������� ����)
	A_Field();

	// ����������
	~A_Field();

	// �������� ������� ���� (�� ��� �� ����� �� ���� ��������)
	void clear();

	// �������� �� ������� ���� ��������
	bool set_card_on_field(int x, int y, A_Card * _card);

	// ����� �������� �����
	void set_card_cur(A_Card * _card = NULL);

	// ��������� ����� �� � ������� �������� ���� �������� ������� ��������
	bool can_set_card(int x, int y);

	// �������� ������� ������� ������ / �����
	bool field_add_column_right();
	bool field_add_column_left();

	// �������� ������� ������ ������ / �����
	bool field_add_row_top();
	bool field_add_row_bottom();

	// ������� ���� (������)
	A_FieldItem 	items[FIELDS_COUNT][FIELDS_COUNT];


	// ����� ����������� ��������
	char        	checkmap[FIELDS_COUNT][FIELDS_COUNT];

	// �������� ����� ����������� ��������
	void            checkmap_zero();

	// ����������� ������� �������� ��������� ��������� �������� ����
	// �� ������ - ���� true, �� ���� ������� ������� �� �����,
	//             ���� false, �� ������� ������� ��� ����� �������
	bool 			checkmap_check_at(int x, int y, int side);
	int             checkmap_roads_count;   // ����������� ��������� �����, �� �� ����� ���� ������ 2, ���� 2- ��� �������
	bool            checkmap_solution;      // ��� �������, ����� ������������ ��� ���
	int             checkmap_points;        // ��� ���������� ����� �� ����������� (�������������) ���������
	bool            checkmap_land_is_close; // ���� ��� ������� �� ���� ������?
	bool            checkmap_market_is_close;   // ����� ������ �� ���� ������?
	std::set<unsigned int> checkmap_UserID;     // ��� ������ c ���������������� �������������, ������� ������ �������


	// ��������� ���� �� ����� �� ����������� �������� ���� � ������������ �������
	bool            has_land_at(int x, int y, int side);
	// ��������� ���� �� �������� �� ����������� �������� ����
	bool            has_card(int x, int y);

	// ����� ��������� �� ����������� � ���������
	int				x_count;
	int 			y_count;

	// ������� ��������� �������� ������� (��� �� ��������, ������� ����� ������ �������� �� ������� ����)
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
// �������� ������� ���� (�� ��� �� ����� �� ���� ��������)
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
// �������� �� ������� ���� ��������
// ----------------------------------------------------------------------------
inline bool
A_Field::set_card_on_field(int x, int y, A_Card * _card)
{
	items[x][y].card = _card;
	items[x][y].card->on_field = true;
}
// ----------------------------------------------------------------------------
// ������� ��������� �������� �������
// ----------------------------------------------------------------------------
inline void
A_Field::set_card_cur(A_Card * _card)
{
	card_cur = _card;
}
// ----------------------------------------------------------------------------
// ��������� ����� �� � ������� �������� ���� �������� ������� ��������
// ----------------------------------------------------------------------------
inline bool
A_Field::can_set_card(int x, int y)
{
	bool		is_ok;
	A_Card *	card_friend;

	// �������� �� �������
	if (card_cur == NULL)
		return false;

	// ����� �� ������� �������� ����
	if ((x < 0) || (y < 0))
		return false;
	if ((x >= x_count) || (y >= y_count))
		return false;

	// �������� ����, ��� �������� �������� ���� �� � ����� ������� ������ ��������
	is_ok = false;
	// ��������� �������� � ����� �������
	if (x > 0)
		if (items[x-1][y].card != NULL) is_ok = true;

	// ��������� �������� � ������ �������
	if (x < x_count-1)
		if (items[x+1][y].card != NULL) is_ok = true;

	// ��������� �������� � ������� �������
	if (y > 0)
		if (items[x][y-1].card != NULL) is_ok = true;

	// ��������� �������� � ������ �������
	if (y < y_count-1)
		if (items[x][y+1].card != NULL) is_ok = true;

	// ���� �������� �� ������������� �� � ����� �� ������, �� ��� ������ �� ��������
	if (is_ok == false)
		return false;

	// �������� ������������ �������� � ����� �������, ������ ��������� ���� ���������
	if (x > 0)
	{
		is_ok = true;
		// �������� � ����� ������� �����������, � �� ������ �� ��� -
		// ��� ��������������� � ����� ������� ������� 1 (������) � ����� ������� - 3 (�����)
		if (items[x-1][y].card != NULL)
		{
			if (items[x-1][y].card->card_items[1].item_type != card_cur->card_items[3].item_type)
				return false;
		}
	}

	// ��������� �������� � ������ �������
	if (x < x_count-1)
	{
		// �������� � ������ ������� �����������, � �� ����� �� ��� -
		// ��� ��������������� � ������ ������� ������� 3 (�����) � ����� ������� - 1 (������)
		if (items[x+1][y].card != NULL)
		{
			if (items[x+1][y].card->card_items[3].item_type != card_cur->card_items[1].item_type)
				return false;
		}
	}

	// ��������� �������� � ������� �������
	if (y > 0)
	{
		// �������� � ������� ������� �����������, � �� ����� �� ��� -
		// ��� ��������������� � ������� ������� ���� ����� 2 (�����) � ����� ������� - 0 (������)
		if (items[x][y-1].card != NULL)
		{
			if (items[x][y-1].card->card_items[2].item_type != card_cur->card_items[0].item_type)
				return false;
		}
	}

	// ��������� �������� � ������ �������
	if (y < y_count-1)
	{
		// �������� � ������ ������� �����������, � �� ������ �� ��� -
		// ��� ��������������� � ������ ������� 2 ( � ����� ������� - 3
		if (items[x][y+1].card != NULL)
		{
			if (items[x][y+1].card->card_items[0].item_type != card_cur->card_items[2].item_type)
				return false;
		}
	}

	return true;
}
// ----------------------------------------------------------------------------
// �������� ������� ������� ������ / �����
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

	// �������� ��� ������� ���� ������
	for (x = x_count; x > 0; x--)
	for (y = y_count; y > 0; y--)
	{
		items[x][y] = items[x-1][y];
	}

	// 0 ������� ��������
	for (y = 0; y < y_count; y++)
	{
		items[0][y].clear();
	}

	x_count = x_count + 1;
	return true;
}
// ----------------------------------------------------------------------------
// �������� ������� ������ ������ / �����
// ----------------------------------------------------------------------------
inline bool
A_Field::field_add_row_top()
{
	int x, y;

	if (y_count >= FIELDS_COUNT)
		return false;

	// �������� ��� ������� ���� ������
	for (x = x_count; x > 0; x--)
	for (y = y_count; y > 0; y--)
	{
		items[x][y] = items[x][y-1];
	}

	// 0 ������ ��������
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
// �������� ����� ����������� ��������
// ----------------------------------------------------------------------------
void
A_Field::checkmap_zero()
{
	int x, y;

	checkmap_roads_count = 0;   	// ������ ����� 0
	checkmap_solution    = true;   	// ������� - ����� �����������
	checkmap_points      = 0;       // ������� ������� ��� ������� �� ��������� ����������
	checkmap_land_is_close = true; 	// ����� ���������? �� ��������� �������, ��� ��������,
									// � �������� �������� ��� ������ ���� ������� ����� - ����������� �����
	checkmap_market_is_close = true;// ����� �� ���� ������ �������?

	checkmap_UserID.clear();

	// ����� ����������� ��������
	for (x = 0; x < x_count; x++)
	for (y = 0; y < y_count; y++)
		checkmap[x][y] = 0;
}
// ----------------------------------------------------------------------------
// ����������� ������� �������� ��������� ��������� �������� ���� �����-�� �������
// �� ������ - ���� true, �� ���� ������� ������� �� �����,
//             ���� false, �� ������� ������� ��� ����� �������
// ----------------------------------------------------------------------------
bool
A_Field::checkmap_check_at(int x, int y, int side)
{
	A_Card *    card;
	int         item_type, item_connection, side_check;

	// ���� ���������� �� ������� ����, �� ������� ����� ��������������� ��������� ���� ������
	// �������, ��� ����� �� ������
	if (x < 0) 			return checkmap_solution;
	if (y < 0) 			return checkmap_solution;
	if (x >= x_count) 	return checkmap_solution;
	if (y >= y_count) 	return checkmap_solution;

	// ���� � ���� ������ ��� ����, �� �����
	if (checkmap[x][y] == 1)
		return checkmap_solution;

	// �������� ������ ��� �����������
	checkmap[x][y] = 1;

	// ���� � ���� ������ ��� ��������, �� ���� ����������� ���������� ����� - �� ������
	card = items[x][y].card;
	if (card == NULL) return checkmap_solution;

	// ���� ���-�� ������, �� ������� �� ������� �������������
	if (card->card_items[side].UserID != 0)
	{
		checkmap_solution = 0;  // ����������� ������
		checkmap_UserID.insert(card->card_items[side].UserID); // ���������� id � ���
	}

	// side - ��� ������� ��������, 0-3 (����, ������, �����, �����), 4 - ��� �����
	// ���� ����� �� �����, �� ��� ����� ����� ������ ��� ������ ��������
	if (side == 4)
	{
		// �������� ������� �������� �� ���� ������
		if (has_card(x-1, y-1) == false) checkmap_market_is_close = false;  // ����� ������
		if (has_card(x-1, y)   == false) checkmap_market_is_close = false;  // �����
		if (has_card(x-1, y+1) == false) checkmap_market_is_close = false;  // ����� �����

		if (has_card(x, y-1)   == false) checkmap_market_is_close = false;  // ������
		if (has_card(x, y+1)   == false) checkmap_market_is_close = false;  // �����

		if (has_card(x+1, y-1) == false) checkmap_market_is_close = false;  // ������ ������
		if (has_card(x+1, y)   == false) checkmap_market_is_close = false;  // ������
		if (has_card(x+1, y+1) == false) checkmap_market_is_close = false;  // ������ �����

		return checkmap_solution;
	}

	// ����������� ��� ��������
	// ������:
	// ���� ������� �������� � ����������� ����� �������� �� ��� ������� �� ������,
	// ���� ���� ���������� ����� ����� �����. ��� ����, ���� �� ���� ���� �� ��������� 2 ������,
	// �� ����� ����������, ��� ������ ��������� ������������� �� ���� ������ � ����� ���������� ���� � ����� �����

	item_type = card->card_items[side].item_type;
	item_connection = card->card_items[side].item_connection;
	checkmap_points = checkmap_points + card->card_items[side].item_price;

	// --------------
	// �������� �����
	// --------------

	if (item_type == 1)
	{
		if (item_connection < 2)
			return checkmap_solution;

		// ----------------------------------
		// �����
		// ----------------------------------

		if (item_connection == 2)  //�����
		{
			checkmap_roads_count = checkmap_roads_count + 1; // ������
			//��������� � ������ ������� �� ������
			switch(side)
			{
			case 0:
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				break;
			case 1:
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				break;
			case 2:
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				break;
			case 3:
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				break;
			}
		}

		// ----------------------------------
		// ��������� c ������������� ��������
		// ----------------------------------

		if (item_connection == 3) //��������� c ������������� ��������
		{
			switch(side)
			{
			case 0:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				break;
			case 1:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				break;
			case 2:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				break;
			case 3:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				break;
			}
		}
		// ----------------------------------
		// ��������� � ������ ��������
		// ----------------------------------
		if (item_connection == 4) // ��������� � ������ ��������
		{
			switch(side)
			{
			case 0:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				break;
			case 1:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				break;
			case 2:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				break;
			case 3:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				break;
			}
		}

		// ----------------------------------
		// ��������� � ����� ��������
		// ----------------------------------
		if (item_connection == 5)  // ��������� � ����� ��������
		{
			switch(side)
			{
			case 0:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				break;
			case 1:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				break;
			case 2:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				break;
			case 3:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				break;
			}
		}

	}

	// ----------------------------------
	// �������� ����
	// ----------------------------------

	if (item_type == 2) // �������� ����
	{
		if (item_connection == 0)
			return checkmap_solution;
		// ----------------------------------
		// �����
		// ----------------------------------
		if (item_connection == 1) // �����
		{
			switch(side)  // ��������� � ������ ������� �� ������
			{
			case 0:
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 1:
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			case 2:
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 3:
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			}
		}
		// ----------------------------------
		// ��������� c ������������� ��������
		// ----------------------------------
		if (item_connection == 2)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			}
		}
		// ----------------------------------
		// ��������� � ������ ��������
		// ----------------------------------
		if (item_connection == 3)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			}
		}

		// ----------------------------------
		// ��������� � ����� ��������
		// ----------------------------------

		if (item_connection == 4)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			}
		}

		// ----------------------------------
		// ��������� � ������ � ���������������
		// ----------------------------------
		if (item_connection == 5)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			}
		}

		// ----------------------------------
		// ��������� ����� � ���������������
		// ----------------------------------
		if (item_connection == 6)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			}
		}
		// ----------------------------------
		// ��������� � ��������
		// ----------------------------------
		if (item_connection == 7)
		{
			switch(side)
			{
			case 0:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 1:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				if (has_land_at(x, y-1, 2) == false) checkmap_land_is_close = false;
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				break;
			case 2:
				if (card->card_items[1].UserID != 0) checkmap_solution = false;
				if (card->card_items[3].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x+1, y, 3);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
				if (has_land_at(x+1, y, 3) == false) checkmap_land_is_close = false;
				if (has_land_at(x, y+1, 0) == false) checkmap_land_is_close = false;
				if (has_land_at(x-1, y, 1) == false) checkmap_land_is_close = false;
				break;
			case 3:
				if (card->card_items[0].UserID != 0) checkmap_solution = false;
				if (card->card_items[2].UserID != 0) checkmap_solution = false;
				checkmap_check_at(x, y-1, 2);    // �������� �������� ������
				checkmap_check_at(x, y+1, 0);    // �������� �������� �����
				checkmap_check_at(x-1, y, 1);    // �������� �������� �����
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
// ��������� ���� �� ����� �� ����������� �������� ���� � ������������ �������
// ----------------------------------------------------------------------------
inline bool
A_Field::has_land_at(int x, int y, int side)
{
	A_Card *    card;

	// �������� �� ������������ ������ ���������
	// 1) ���� side = 4 - ��� �����
	if (side == 4)
		return false;

	// 2) ���� �� �����-�� ������� ����� �� ������� �������� ����,
	//    �� ��� ���� ������ � ���� ��������� �������� ������
	if ((x < 0) || (y < 0) || (x >= x_count) || (y >= y_count))
		return false;

	// ���� � ���� ������ ��� ��������, �� ����� ���
	card = items[x][y].card;
	if (card == NULL) return false;

	// ��������� ������� (���� �����, �� ��� ��)
	if (card->card_items[side].item_type == 2)
		return true;

	return checkmap_solution;
}
// ----------------------------------------------------------------------------
// ��������� ���� �� �������� �� ����������� �������� ����
// ----------------------------------------------------------------------------
inline bool
A_Field::has_card(int x, int y)
{
	A_Card *    card;

	// 2) ���� �� �����-�� ������� ����� �� ������� �������� ����,
	//    �� ��� ���� ������ � ���� ��������� �������� ������
	if ((x < 0) || (y < 0) || (x >= x_count) || (y >= y_count))
		return false;

	// ���� � ���� ������ ��� ��������, �� ����� ���
	card = items[x][y].card;
	if (card == NULL) return false;

	return true;
}
// ----------------------------------------------------------------------------


#endif
