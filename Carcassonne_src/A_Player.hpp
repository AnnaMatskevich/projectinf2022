// ----------------------------------------------------------------------------
#ifndef _A_PLAYER_HPP_
#define _A_PLAYER_HPP_
// ----------------------------------------------------------------------------
#include "A_Chip.hpp"
#include <ustring.h>
// ----------------------------------------------------------------------------
// �������� ������ �� ������� ���� ���������:
// � ���� 5 ������� (�������, ������, �������, �����, �����)
// � ������� ������ �� 4 �����
// ----------------------------------------------------------------------------
#define CHIPS_MAX   4
// ----------------------------------------------------------------------------
class A_Player
{
public:
	// ����������� (������ ��������� �������� ���� ���������� ������)
	A_Player() { clear(); }

	// ����� ������?
	bool				Is_playing;

	// ������������� ������ (�� ����� ��� ����, ����� ����� ����� ���� ���������� ��� �������� ������ ��� ����)
	unsigned long long 	UserID;

	// ��� ������
	UnicodeString		Name;

	// ���� ������
	TColor				Color;

	// �������� �������
	int             	Bananas_count;

	// �������� �������
	int             	Seashells_count;

	// ������� ������
	int                 Maketplaces_count;

	// ����� ������
	A_Chip              Chips[CHIPS_MAX];

	// ����������� ��������� (������) ��� ����������� ����� � ������ ������������� ������

	// ������ �� ������� ������������� ��� �������������� ������
	TPanel *    		panel_main;

	// ����������� ����� �������� (�������� �������
	TPanel *    		panel_vert;

	// �� ������� ������ ���� panel_top, ��� ������������� ��� �����������
	// ����� ������ � ����� (������� - ��� �������� ������, ������� ���� - ���� ����� �� �����)
	TPanel *  			panel_top;

	// Label ��� ����������� ���������� �������
	TLabel *    		label_bananas_count;

	// Label ��� ����������� ���������� �������
	TLabel *    		label_shells_count;

	// Label ��� ����������� ���������� ����������� ������
	TLabel *    		label_markets_count;

	// Label ��� ����������� ���������� ������������ �����
	TLabel *    		label_points_count;

	// Label ��� ����������� ���������� ��������� �����
	TLabel *    		label_tuples_count;

	// �������� � ������������ ����� ��� ������� ������
	TImage *    		image_tuple;

	// �������� �������� ����������
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

		// ���� ���� �� ���� �������� �� ������� ���� - ������ ��
		for (i = 0; i < CHIPS_MAX; i++)
		{
			Chips[i].hide();
		}

		// ����������� ��������� ���������� �� �����
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

	// �������� ��������� �� ������ "���������" �����
	A_Chip * get_free_chip()
	{
		int i;

		for (i = 0; i < CHIPS_MAX; i++)
		{
			if (Chips[i].IsUsed == false)
				return &Chips[i];
		}
		// �� ����� "���������" ������� ����� �� �����
		return NULL;
	}

	// �������� ���������� ��������� �����
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

	// ������������� ������������ ���������� � ���������������� ������
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
		// � �������� ������������� ���� ������� ���������� ������
		set_player_active_state(false);
		// �������� �������� �������������
		update_gui();
	}

	// �������� �� ������ ��� �������������� ������
	void update_gui()
	{
		// ������
		if (label_bananas_count != NULL)
			label_bananas_count->Caption = Bananas_count;
		// �������
		if (label_shells_count != NULL)
			label_shells_count->Caption  = Seashells_count;
		// �����
		if (label_markets_count != NULL)
			label_markets_count->Caption = Maketplaces_count;
		// �����
		if (label_tuples_count != NULL)
			label_tuples_count->Caption  = get_free_chipa_count();
		// ����� ���������� ����� (����� ������ ����� ������ ������� �������� �����)
		if (label_points_count != NULL)
			label_points_count->Caption  = calc_points();
	}

	// ���������� ���������� ������ (����� �� ��� ���? ��� �������� �� ���� ����� ������)
	void set_player_active_state(bool is_active)
	{
		if (panel_top != NULL)
			if (is_active == true)
				panel_top->Color = clLime; //0x0053FF53;
			else
				panel_top->Color = clBtnFace;
	}

	// ���������� ��� ������ �� ������
	void set_name()
	{
		if (panel_top != NULL) panel_top->Caption = Name;
	}

	// �������� ���������� ����� ������
	int calc_points()
	{
		// ������ ������������ �� ��������� �������:
		// ���������� ����� = ����� ����� ����� ������� + ����� ������� + 2 * ����� ����������� ������
		int points;

		points = Bananas_count / 3 + Seashells_count / 3 + Maketplaces_count * 2;

		return points;
	}

};
// ----------------------------------------------------------------------------
#endif
