//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "Form__main.h"
#include "Form__zastavka.h"
#include "Form__players.h"
#include "Form__about.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm_main *	Form_main;

// ������ � �������� ����������
A_Cards			cards;
// ������� ����
A_Field			field;

// ��� ������ � �� �������� ��������� � Form_players

//---------------------------------------------------------------------------
__fastcall TForm_main::TForm_main(TComponent* Owner)
	: TForm(Owner)
{
	for (int i = 0; i < 10; i++) target[i] = NULL; //��� ����
}
//---------------------------------------------------------------------------
// ���� ������� ��������� ������� ���������
//---------------------------------------------------------------------------
void __fastcall TForm_main::A_WndProc(Messages::TMessage &Message)
{
	WndProc(Message);
}
//---------------------------------------------------------------------------
void __fastcall TForm_main::FormShow(TObject *Sender)
{
	Form_zastavka->ShowModal();

	// C������ ����� ����: ���� ������� ���������� false,
	//                     �� ������ ���������� ������, ���� ����� �� ����
	if (CreateNewGame() == false)
	{
		Close();
	}
}
//---------------------------------------------------------------------------
// ������� �� ������ ����: "����� �� ����" ��� �� "�������" �������� ����������
//---------------------------------------------------------------------------
void __fastcall TForm_main::N_closeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
// ������������� ������ �� ����: ������� � ������� ���� ��������� ���� ��� �������� ������
//---------------------------------------------------------------------------
void __fastcall TForm_main::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// �� ��������� �� ���� �� �������
	CanClose = false;
	// ���� ������� ����� �� �������� �� ����
	if (MessageBoxA(NULL, "����� �� ����?", "��������", MB_YESNO + MB_ICONQUESTION) == IDNO)
		return;
	// ��������� ����� �� ����
	CanClose = true;
}
//---------------------------------------------------------------------------
// ������� �� ������ ����: "�� ����..."
//---------------------------------------------------------------------------
void __fastcall TForm_main::N_aboutClick(TObject *Sender)
{
	Form_about->ShowModal();
}
//---------------------------------------------------------------------------
// ���������� ������� ����
//---------------------------------------------------------------------------
void
TForm_main::PaintField()
{
	int 		x, xx, y, yy;
	TCanvas *	canvas;
	TRect		rect;

	canvas = Image_field->Canvas;

	Image_field->Left   = 0;
	Image_field->Top    = 0;

	// ��������� ���� ��������
	rect.init(0, 0, Image_field->Width, Image_field->Height);
	canvas->Brush->Color = clCream;//clBtnFace;
	canvas->Brush->Style = bsSolid;
	canvas->FillRect(rect);

	for (x = 0; x < field.x_count; x++)
	for (y = 0; y < field.y_count; y++)
	{
		if (field.items[x][y].card == NULL)
		{
			// ��������� ���� ��������
			rect.init(10 + x * CARD_SIZE + 1, 10 + y * CARD_SIZE + 1, 10 + x * CARD_SIZE + CARD_WIDTH, 10 + y * CARD_SIZE + CARD_WIDTH);
			canvas->Brush->Color = clCream;//clWhite;//clBtnFace;
			canvas->Brush->Style = bsSolid;
			canvas->FillRect(rect);

			canvas->Pen->Color = clGreen;
			canvas->Pen->Style = psDot;
			PaintRect(canvas, x, y);
		}
		else
		{
			field.items[x][y].card->move_to(10 + x * CARD_SIZE + 1, 10 + y * CARD_SIZE + 1, true);
		}
	}

}
//---------------------------------------------------------------------------
// ��������� �� ������ ������ �������� ��������
//---------------------------------------------------------------------------
void
TForm_main::PaintRect(TCanvas * canvas, int X_field, int Y_field)
{
	int x_start, y_start;

	if ((X_field < 0) || (Y_field < 0))
		return;

	if ((X_field >= field.x_count) || (Y_field >= field.y_count))
		return;

	x_start = 10 + X_field * CARD_SIZE;
	y_start = 10 + Y_field * CARD_SIZE;

	canvas->MoveTo(x_start, y_start);
	canvas->LineTo(x_start + CARD_SIZE, y_start);
	canvas->LineTo(x_start + CARD_SIZE, y_start + CARD_SIZE);
	canvas->LineTo(x_start, y_start + CARD_SIZE);
	canvas->LineTo(x_start, y_start);
}
//---------------------------------------------------------------------------
// ����� ������� �� ������� ����
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_fieldMouseEnter(TObject *Sender)
{
	X_field_0 = -1;
	Y_field_0 = -1;
	X_cur     = -1;
	Y_cur     = -1;
}
//---------------------------------------------------------------------------
// ����� �������� ������� ����
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_fieldMouseLeave(TObject *Sender)
{
	TCanvas *	canvas;

	canvas  = Image_field->Canvas;
	// ������������ ������ �������
	canvas->Pen->Color = clGreen;
	canvas->Pen->Style = psDot;
	PaintRect(canvas, X_field_0, Y_field_0);

	X_field_0 = -1;
	Y_field_0 = -1;
	X_cur     = -1;
	Y_cur     = -1;
}
//---------------------------------------------------------------------------
// ����� ������������ �� �������� ����
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_fieldMouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y)
{
	TCanvas *	canvas;
	int 		x_field, y_field;

	// ������� ���������� ���� �� ������� ����
	X_cur = X;
	Y_cur = Y;

	x_field = (X - 10) / CARD_SIZE;
	y_field = (Y - 10) / CARD_SIZE;

	if (X < 10) x_field = -1;
	if (Y < 10) y_field = -1;

	if ((x_field != X_field_0) || (y_field != Y_field_0))
	{
		canvas  = Image_field->Canvas;
		// ������������ ������ �������
		canvas->Pen->Color = clGreen;
		canvas->Pen->Style = psDot;
		PaintRect(canvas, X_field_0, Y_field_0);
		// ���������� ����� �������
		canvas->Pen->Color = clRed;
		canvas->Pen->Style = psDot;
		PaintRect(canvas, x_field, y_field);
		// ����� -> ������
		X_field_0 = x_field;
		Y_field_0 = y_field;
	}
}
//---------------------------------------------------------------------------
// ������� �� ������� ����, ��� ��������� ��� ������ �������� �� ������� ����
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_fieldClick(TObject *Sender)
{
	int 		x_field, y_field;

	// ���� �������� ��� ��������, �� �� ���� ������ �� ����
	if (is_put_card == true)
		return;

	x_field = (X_cur - 10) / CARD_SIZE;
	y_field = (Y_cur - 10) / CARD_SIZE;

	if ((x_field < 0) || (y_field < 0))
	{
		MessageBoxA(NULL, "� ������ ����� �������� ���� �������� �������� ������", "��������", MB_OK + MB_ICONERROR);
		return;
	}

	if ((x_field >= field.x_count) || (y_field >= field.y_count))
	{
		MessageBoxA(NULL, "� ������ ����� �������� ���� �������� �������� ������", "��������", MB_OK + MB_ICONERROR);
		return;
	}

	if (field.items[x_field][y_field].card != NULL)
	{
		MessageBoxA(NULL, "������ ����� �� ������� ���� ��� ������", "��������", MB_OK + MB_ICONERROR);
		return;
	}

	if (field.can_set_card(x_field, y_field) == false)
	{
		MessageBoxA(NULL, "������. � ������ ����� �������� ���� �������� �������� ������", "��������", MB_OK + MB_ICONERROR);
		return;
	}
	else
	{
		// �������� �������� �� ������� ����
		CardPut(x_field, y_field, field.card_cur);
	}
}
//---------------------------------------------------------------------------
// ������� �� ������� ��������, ��� ��������� ��� ������� ��������
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_curClick(TObject *Sender)
{
	// ���� �������� ��� ��������, �� ������ ������� "�������"
	if (is_put_card == true)
		return;
	// ���� ������ '��������� ���' ����������, �� ���� �����
	if (Button_player_turn_end->Enabled == false)
        return;

	field.card_cur->rotate();
	Image_cur->Picture = field.card_cur->Image->Picture;
}
//---------------------------------------------------------------------------
// ������ ���� ������: ���������� ��� �������� ������ � ��� ������� ��������
//---------------------------------------------------------------------------
bool
TForm_main::PlayerTurnStart()
{
	int 	i, index, count;

	// ����� ����� �� ������� �� ����
	is_put_card = false;

	// �������� ���� �������
	UpdatePlayersPanel();

	// 1) ���������� ��� �������� ������ � �������� ���� �������
	if (Form_players->players_cur != 0)
	{
		Form_players->players[Form_players->player_index].set_player_active_state(true);
	}

	// 2) ��������� ������� �������� ��������
	count = 0;
	for (i = 1; i<cards.cards_count; i++)
	{
		// ��������� �� ����� �� �������� �� ������� ����
		// ��������� ��� ����� ����������, �� ������ ��������� �������� ����� �� ������� cards.indexes
		index = cards.indexes[i];

		// ��������� �� ����� �� �������� �� ������� ����
		if (cards.card[index].on_field == false)
			count = count + 1;
	}

	if (count != 0)
		count = count - 1;
	Label_cards_free->Caption = count;

	// 2) ������������� ��� ��������, � ���������� ������ ������ �� ���������� �� ������� ����
	for (i = 1; i < cards.cards_count; i++)
	{
		// ��������� ��� ����� ����������, �� ������ ��������� �������� ����� �� ������� cards.indexes
		index = cards.indexes[i];

		// ��������� �� ����� �� �������� �� ������� ����
		if (cards.card[index].on_field == false)
			break;
	}

	// ���� ����������� ��� �������� � ��� ����� �� ������� ����, �� ����������� ����
	if (i == cards.cards_count)
	{
		return false;
	}

	// ������� ��������
	Image_cur->Picture = cards.card[index].Image->Picture;
	field.set_card_cur(&cards.card[index]);
	Label_desc_1->Enabled = true;
	Label_desc_2->Enabled = true;

	// ����������
	Edit_number->Text = index;

	return true;
}
//---------------------------------------------------------------------------
// ����� ���� ������: ��������� �� �� ����� ������ �� ��� +
//                    ��������� ����� ����� ����� ������ � ��������� ���� +
//                    ������� � ������� ������ � ������ ��������
//---------------------------------------------------------------------------
void
TForm_main::PlayerTurnEnd()
{
	// ��������� ������� �� ������� ����� ��������?
	if (is_put_card == false)
	{
		MessageBoxA(NULL, "����� ������� � ���������� ������, ���������� �������� �������� �� ������� ����", "��������", MB_OK + MB_ICONERROR);
		return;
	}

	// ������� ������ �������� ������ ����������
	Form_players->players[Form_players->player_index].set_player_active_state(false);

	// ������� � ���������� ������
	Form_players->player_index = Form_players->player_index + 1;

	// ���� ���, �� �������� ������� ������ �������
	if (Form_players->player_index == Form_players->players_cur)
		Form_players->player_index = 0;

	// ������� ���� ��� �������
	HideTargets();

	// ����� ��������� ���� ������, ��������
	// ����� ����� ����� ������ � �������� ���� � ��������� ���� �������
	CheckAllTuples();

	// ������ ��������� ���
	if (PlayerTurnStart() == false)
	{
		FinishGame();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_main::Button_player_turn_endClick(TObject *Sender)
{
	PlayerTurnEnd();
}
//---------------------------------------------------------------------------
// ��������, ����� �������� ������� �������� �� ������� ����
//---------------------------------------------------------------------------
void
TForm_main::CardPut(int x_field, int y_field, A_Card * card)
{
	// �������� �������� �� ������� ����
	field.set_card_on_field(x_field, y_field, card);

	// ��������� ������� ����
	if (x_field == 0)
		field.field_add_column_left();

	if (y_field == 0)
		field.field_add_row_top();

	if (x_field == field.x_count - 1)
		field.field_add_column_right();

	if (y_field == field.y_count - 1)
		field.field_add_row_bottom();

	// ������������ ������� ����
	PaintField();

	// �������� �������� �� ����
	is_put_card = true;

	// ������ ������������ �� ������ ������, ��� ���-�� ����� ��������
	// ��������� 0 �������� - �������
	Image_cur->Picture = cards.card[0].Image->Picture;

	Label_desc_1->Enabled = false;
	Label_desc_2->Enabled = false;

	// ���������� ��� ����� ���������
	ShowTargets();
}
//---------------------------------------------------------------------------
// ����� ���������� �������� �� ����
//---------------------------------------------------------------------------
bool
TForm_main::CardGetXY(A_Card * card, int & x_field, int & y_field)
{
	int x, y;

	x_field = -1;
	y_field = -1;

	for (x = 0; x < field.x_count; x++)
	for (y = 0; y < field.y_count; y++)
	{
		if (field.items[x][y].card == card)
		{
			x_field = x;
			y_field = y;
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
// �������� ����� ���� ��� �������� ������� �������� �� ������� ����:
// ���������� ����, ������� ����� ���������
//---------------------------------------------------------------------------
void
TForm_main::ShowTargets()
{
	int 			i, index, x_field, y_field;
	A_Card *    	card;
	A_Chip *        chip;
	A_CardItem * 	card_item;

	card = field.card_cur;

	// ������� ���� �� ����������� ������ ��������� ����� �� ������� ����
	chip = Form_players->players[Form_players->player_index].get_free_chip();
	if (chip == NULL)
	{
		return;
	}

	if (CardGetXY(card, x_field, y_field) == false)
	{
		MessageBoxA(NULL, "�� ������� ����� ������� ��������", "��������", MB_OK);
		return;
	}

	// ������� ����� �� ��������� �����-�� ����������
	index = 0;
	for (i = 0; i < 5; i++)
	{
		// �������� ������� ��������: 0,1,2,3 - ������� ������� ��������, 4 - �����
		card_item = &card->card_items[i];
		if ((card_item->x != 0) && (card_item->y != 0))
		{
			// ����� �� ��������� ���� ������� �������?
			field.checkmap_zero();
			if (field.checkmap_check_at(x_field, y_field, i) == false)
			{
				continue;
			}

			// ������� ����� ���� ��� �������
			target[index] = new A_Target();
			target[index]->show(card->Panel, card_item->x, card_item->y, Image_target_leave->Picture,
									Image_target_fieldMouseEnter,
									Image_target_fieldMouseLeave,
									Image_target_fieldClick);
			index = index + 1;
		}
	}


}
//---------------------------------------------------------------------------
// ��������: ������ ��� ���� ��� �������
//---------------------------------------------------------------------------
void    TForm_main::HideTargets()
{
	int     i;
	for (i = 0; i < 10; i++)
	{
		if (target[i] != NULL) delete target[i];
		target[i] = NULL;
	}
}
//---------------------------------------------------------------------------
// ����� �������� �� ���� ��� ������� ����������
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_target_fieldMouseEnter(TObject *Sender)
{
	TImage * image;
	image = (TImage *)Sender;
	image->Picture = Image_target_enter->Picture;
}
//---------------------------------------------------------------------------
// ����� �������� ���� ��� ������� ����������
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_target_fieldMouseLeave(TObject *Sender)
{
	TImage * image;
	image = (TImage *)Sender;
	image->Picture = Image_target_leave->Picture;
}
//---------------------------------------------------------------------------
// ������ �������� �� ���� ��� ������� ����������
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_target_fieldClick(TObject *Sender)
{

	AnsiString      text;
	TImage * 		image;
	A_Card *    	card;
	A_Player *      player;
	A_Chip *        chip;
	TPicture *      picture;
	int         	i, x, y, player_index;

	// ���� ���� ���� ����� (������ ����, �� �� ������ ������ ��������)
	if (Form_players->players_cur == 0)
	{
		MessageBoxA(NULL, "�� ������ ������ ��� ����", "��������", MB_OK + MB_ICONERROR);
		return;
	}

	// ��������� �� ������� �� ��������
	image = (TImage *)Sender;

	// ������� ������� ��������, ������� ����� ������� �� ������� ����
	card  = field.card_cur;

	// �����, ������� ������� ��������
	player = &Form_players->players[Form_players->player_index];

	// �����, ������� ����� �������� �����
	chip = player->get_free_chip();
	if (chip == NULL)
	{
		MessageBoxA(NULL, "����� �� ����� �������� ����� �� ������� ����: �����������", "��������", MB_OK + MB_ICONERROR);
		return;
	}

	// ���������� ����� ������ ������� ������� ����� ���������
	// � ����������� ��������� 8 (��. ������� show, ��� �������� 8, ����� �� �������� ��������)
	x = image->Left + 8;
	y = image->Top + 8;

	text = "";
	// ������������� ���� ������ �� �������� � ������ ������������, ������� ��� ������
	for (i = 0; i < 5; i++)
	{
		if ((card->card_items[i].x == x) && (card->card_items[i].y == y))
		{
			card->card_items[i].UserID = player->UserID;
			break;
		}
	}

	// �������� ��� ���� (�� �� ��� ��������� ���� �� ���)
	HideTargets();

	// ������������ �����
	switch(player->Color)
	{
	case clRed:    picture = Image_tuple_red->Picture; break;
	case clYellow: picture = Image_tuple_yellow->Picture; break;
	case clGreen:  picture = Image_tuple_green->Picture; break;
	case clBlue:   picture = Image_tuple_blue->Picture; break;
	case clSilver: picture = Image_tuple_silver->Picture; break;
	}

	// ���������
	chip->show(card, x, y, 	picture,
							Image_tuple_MouseEnter,
							Image_tuple_MouseLeave,
							Image_tuple_Click);

}
//---------------------------------------------------------------------------
// ����� ��� ������� ������
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_tuple_MouseEnter(TObject *Sender)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_tuple_MouseLeave(TObject *Sender)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_tuple_Click(TObject *Sender)
{

}
//---------------------------------------------------------------------------
// ������� ������ � ������ �������
//---------------------------------------------------------------------------
void
TForm_main::CreatePlayersPanel()
{
	int 			i, count;
	A_Player *      player;

	// �������������� ����������� ��������� ��� ������� ������
	// 0-��� ������ ������������ 0 ������ � ������� ����� ������,
	// 1-��� - 1 ������ � �.�.
	player = &Form_players->players[0];
	player->init_gui(Panel_user_0, Panel_user_vert_0, Panel_top_user_0, Label_user_bananas_count_0, Label_user_shells_count_0, Laber_user_makets_count_0, Label_user_points_0, Label_user_tuples_count_0, Image_user_tuple_0);
	player = &Form_players->players[1];
	player->init_gui(Panel_user_1, Panel_user_vert_1, Panel_top_user_1, Label_user_bananas_count_1, Label_user_shells_count_1, Laber_user_makets_count_1, Label_user_points_1, Label_user_tuples_count_1, Image_user_tuple_1);
	player = &Form_players->players[2];
	player->init_gui(Panel_user_2, Panel_user_vert_2, Panel_top_user_2, Label_user_bananas_count_2, Label_user_shells_count_2, Laber_user_makets_count_2, Label_user_points_2, Label_user_tuples_count_2, Image_user_tuple_2);
	player = &Form_players->players[3];
	player->init_gui(Panel_user_3, Panel_user_vert_3, Panel_top_user_3, Label_user_bananas_count_3, Label_user_shells_count_3, Laber_user_makets_count_3, Label_user_points_3, Label_user_tuples_count_3, Image_user_tuple_3);
	player = &Form_players->players[4];
	player->init_gui(Panel_user_4, Panel_user_vert_4, Panel_top_user_4, Label_user_bananas_count_4, Label_user_shells_count_4, Laber_user_makets_count_4, Label_user_points_4, Label_user_tuples_count_4, Image_user_tuple_4);

	// ������� ��� ������ ��������
	for (i = 0; i < 5; i++)
	{
		player = &Form_players->players[i];
		player->panel_vert->Visible = false;
		player->panel_main->Visible = false;
	}

	count = Form_players->players_cur;

	// ������ ��� ������ �� ������, �������� � ������ � ������ ������ �������
	for (i = count-1; i>=0; i--)
	{
		player = &Form_players->players[i];
		// ��������� ����� ������� �����
		switch(player->Color)
		{
			case clRed: 	player->image_tuple->Picture = Image_tuple_red->Picture; break;
			case clYellow:  player->image_tuple->Picture = Image_tuple_yellow->Picture; break;
			case clGreen:   player->image_tuple->Picture = Image_tuple_green->Picture; break;
			case clBlue:    player->image_tuple->Picture = Image_tuple_blue->Picture; break;
			case clSilver:  player->image_tuple->Picture = Image_tuple_silver->Picture; break;
		}

		player->set_name();
		player->panel_main->Visible = true;
	}

	// ���������� ������������ ������ - ��������� ����� �������� � ��������� �������
	for (i = count-1; i>=0; i--)
	{
		player = &Form_players->players[i];
		player->panel_vert->Visible = true;
	}
}
//---------------------------------------------------------------------------
// �������� ���� �������
//---------------------------------------------------------------------------
void
TForm_main::UpdatePlayersPanel()
{
	A_Player *      player;
	int 			i, count;

    // ����� ����� ������� � ������ ������� ������
	count = Form_players->players_cur;
	// ������ �������� ��������������� ������������������
	for (i = 0; i < count; i++)
	{
		player = &Form_players->players[i];
		player->update_gui();
	}
}
//---------------------------------------------------------------------------
// ��������: ����� ���� ��� ����������� ���, ���������
// ����� ���� ���� ������ �����-�� ����� � ��������� �����
//---------------------------------------------------------------------------
void
TForm_main::CheckAllTuples()
{
	int 		i, j, count, x, y;
	int         x_field, y_field, side;
	A_Player * 	player_cur;
	A_Chip   *  chip_cur;
	A_Card 	 *  card_cur;

	count = Form_players->players_cur;
	for (i = 0; i < count; i++)
	{
		// ������� ���������������   �����
		player_cur = &Form_players->players[i];
		// � ���� ���� CHIPS_MAX ����� (������ ��� 4), ������ �� ��� ���������
		for (j = 0; j < CHIPS_MAX; j++)
		{
			chip_cur = &player_cur->Chips[j];
			// ���� ����� �� ������������ (�.�. �� ����� �� ������� ����), ��
			// ��������� � ���������
			if (chip_cur->IsUsed == false) continue;

			// �������� �� ������� ����� ����� � �� ���������� �� ������� ����
			card_cur = chip_cur->Card;
			CardGetXY(card_cur, x_field, y_field);

			// ���������� ���� ����� �� �������� (+8 ��. ������� A_Chip::show())
			x = chip_cur->Image->Left + 8;
			y = chip_cur->Image->Top + 8;

			// ���������� �������, ������� ��������� ���� ������
			for (side = 0; side < 5; side++)
			{
				if ((card_cur->card_items[side].x == x) && (card_cur->card_items[side].y == y))
				{
					card_cur->card_items[side].UserID = player_cur->UserID;
					break;
				}
			}

			// ������ �������� ������� ����������� ���� ������ �� ����������� ����������
			field.checkmap_zero();
			field.checkmap_check_at(x_field, y_field, side);

			// ���� ��� ������ � ������� ���� 2 ����� ������, �� ��� ���������
			if (card_cur->card_items[side].item_type == 1)
			{
				if (field.checkmap_roads_count == 2)
				{
					// ��������� ���� (���� ��� ����)
					player_cur->Seashells_count = player_cur->Seashells_count + field.checkmap_points;
					// ������� ����� � ����
					chip_cur->hide();
				}
			}

			// ���� ��� ���� � ��� � ������ ������� ����� ���� ����, �� ��� ��������
			if (card_cur->card_items[side].item_type == 2)
			{
				if (field.checkmap_land_is_close == true)
				{
					// ��������� ���� (���� ��� ����)
					player_cur->Bananas_count = player_cur->Bananas_count + field.checkmap_points;
					// ������� ����� � ����
					chip_cur->hide();
				}
			}

			// ���� ��� ����� � �� �� ���� ������ ������� ����������, �� �� ������ (��������) �������
			if (card_cur->card_items[side].item_type == 3)
			{
				if (field.checkmap_market_is_close == true)
				{
					// ��������� ���� (���� ��� ����)
					player_cur->Maketplaces_count = player_cur->Maketplaces_count + 1;
					// ������� ����� � ����
					chip_cur->hide();
				}
			}
		}
	}

	// ��������� ����
	UpdatePlayersPanel();

}
//---------------------------------------------------------------------------
// ������ ����� ����
//---------------------------------------------------------------------------
bool
TForm_main::CreateNewGame()
{
	int     i, n;

	// 1) ������� �������� � ���� ������� (�� ����� ����. 5)
	n = 5;
	for (i=0;i<n;i++)
	{
		// � ������ 0 �������� �������
		Form_players->VT_players->RootNodeCount = 0;
		// � ������ 0 �������� �������
		Form_players->players_cur = 0;
		// �������� � ������ �������
		Form_players->players[i].clear();
	}

	// 2) �������� ���� � ������ ������ �� �������
	Form_players->ShowModal();
	// ����������� �������� Form_players->is_ok:
	// �.�. is_ok = true  -> ����� ����� �� ������ '������ ����'
	//      is_ok = false -> ����� ����� �� ������ '������'
	if (Form_players->is_ok == false)
		return false;

	// 3) ������� ������ � ������ ���� �������
	CreatePlayersPanel();

	// 4) ������� ��� ������� �������� (��� ����� ���� ��������� ��� ������) +
	//    �������������� ������������� (�������� �������� � �������� ������ ��������) +
	//    ������������ ��������
	cards.clear();
	cards.init(ScrollBox_field, ImageList_cards, 74, Image_target_enter, Image_target_leave);
	cards.shuffle_cards();

	// 5) ������� ������� ���� (�� ��� �� ������ ���� ������� ��������)
	//    �� ����������� (1,1) ���������� �������� 29 (����������� ���������,
	//    ��� ��� ������ � ���� � �� �� ������
	field.clear();
	field.set_card_on_field(1, 1, &cards.card[29]);

	// 6) ������ ������� ����
	PaintField();

	// 7) ���� �� �����������
	is_game_finished = false;

	// 8) �������� ������ ��� ������, �.�. ��� ������ ���,
	//    �� �������������� ���� �� ��� �������� �� ����
	PlayerTurnStart();

	// 9) ������ ����� ������ ��������� ����
	N_finish_start->Caption = "��������� ����";

	// 10) �������� ������ '��������� ���'
	Button_player_turn_end->Enabled = true;

	return true;
}
//---------------------------------------------------------------------------
// �������� �� ��������� ����
//---------------------------------------------------------------------------
void
TForm_main::FinishGame()
{
	int         player_points_best, player_points_cur, i, players_points_best_count;
	AnsiString  winners;

	// ���� ��������

	is_game_finished = true;

	// ��������� ��� �������� �����������, ������� ���� ���������� ����� � ����

	players_points_best_count = 0;
	player_points_best = 0;
	for (i=0; i<Form_players->players_cur; i++)
	{
		player_points_cur = Form_players->players[i].calc_points();
		// ����� �� ���������� �����
		if (player_points_cur == player_points_best)
		{
			players_points_best_count = players_points_best_count + 1;
		}
		// ���� ������ �����
		if (player_points_cur > player_points_best)
		{
			player_points_best = player_points_cur;
			players_points_best_count = 1;
		}
	}

	// ���� ���� ���������� ����� ����� 0, �� ����������� ���
	if (player_points_best == 0)
	{
		MessageBoxA(NULL, "���� ��������! ���������� ���!", "��������", MB_OK + MB_ICONINFORMATION);
	}
	else
	{
		// ���� ���� ���������� ����� ������ 1 �����, �� �� ����������,
		// ���� ��������� �������, �� ��� ���������� :)

		if (players_points_best_count == 1)
			winners = "���� ��������!\r\n����������:\r\n";
		else
			winners = "���� ��������!\r\n����������:\r\n";

		players_points_best_count   = 0;
		for (i=0; i<Form_players->players_cur; i++)
		{
			if (Form_players->players[i].calc_points() == player_points_best)
			{
				// ���� ��� ������ ���������� �������, �� ���� ��������� "�������" � �����
				if (players_points_best_count != 0)
				{
					winners = winners + ",\r\n";
				}
				winners = winners + Form_players->players[i].Name;
				players_points_best_count = players_points_best_count + 1;
			}
		}

		MessageBoxA(NULL, winners.c_str(), "��������", MB_OK + MB_ICONINFORMATION);
	}

	// ������ ����� ������ ������ ����� ����
	N_finish_start->Caption = "������ ����� ����";

	// ���������� ������ '��������� ���'
	Button_player_turn_end->Enabled = false;


	return;
}
//---------------------------------------------------------------------------
void __fastcall TForm_main::N_finish_startClick(TObject *Sender)
{
	// ���� ���� ��� �� ��������, ���� �������� �� ��� ���������� ������
	if (is_game_finished == false)
	{
		// ������ ���� �� ��������� ����, ���� ��� � ����� �������?
		if (MessageBoxA(NULL, "��������� ����?", "��������", MB_YESNO + MB_ICONQUESTION) == IDNO)
			return ;

		// �������� �� �������� ����
		FinishGame();
			return;
	}

	// ������ ����� ���� ��� �����
	CreateNewGame();
}
//---------------------------------------------------------------------------
void __fastcall TForm_main::N_rulesClick(TObject *Sender)
{
	AnsiString  path;
	char 		buffer[MAX_PATH];

	// �������� ������ ���� � ����������� exe-�����
	// ���� ������� ��: <����� �����>:\<rfnfkjub>\Carcassonne-game.exe
	// ��������, d:\games\Carcassonne-game.exe
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	path = buffer;

	// ������ �������� ���� (�� ����� ���� ������ ��� ��������) � ������� ������ �� ����:
	// ������ ����: d:\games\

	path = path.SubString(0, path.LastDelimiter("\\/"));

	// ������� � �������� �������� ����� � ��������� ����:
	// d:\games\������� ���� ���������.docx
	path = path + "������� ���� ���������.docx";

	// �������� ������� ���������, ������� ��������� docx �����:
	ShellExecuteA(Form_main->Handle, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------



