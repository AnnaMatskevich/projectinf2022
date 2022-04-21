//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form__players.h"
#include "Form__new_player.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"

TForm_players *Form_players;

//---------------------------------------------------------------------------
__fastcall TForm_players::TForm_players(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_players::Button_startClick(TObject *Sender)
{
	// ���������� �������� ������� ������ � ������� 0 (�� ����� ������)
	player_index = 0;

	// ��� ������ ���� ���������� ������
	is_ok        = true;

    // ��������� ���� � ��������
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_players::Button_addClick(TObject *Sender)
{
	UnicodeString 	color_name;

	Form_new_player->ShowModal();
	if (Form_new_player->is_ok == true) // ���� ��������� ��������� ������ ������� ������
	{
		players[players_cur].clear();
		players[players_cur].Name             = Form_new_player->Edit_player_name->Text;
		players[players_cur].Color            = Form_new_player->ColorBox_colors->Selected;
		players[players_cur].UserID           = players_cur + 1;

		// ������� ����� ����� �������� �� �����
		color_name = Form_new_player->ColorBox_colors->Items->Strings[Form_new_player->ColorBox_colors->ItemIndex];
		if (color_name == "�������")
			Form_new_player->is_need_red = false;
		if (color_name == "������")
			Form_new_player->is_need_yellow = false;
		if (color_name == "�������")
			Form_new_player->is_need_green = false;
		if (color_name == "�����")
			Form_new_player->is_need_blue = false;
		if (color_name == "�����")
			Form_new_player->is_need_silver = false;

		Form_new_player->update_colors();

		players_cur = players_cur + 1; // ���-�� �������� +1

		VT_players->RootNodeCount = players_cur; //??? ����

		if (players_cur == PLAYERS_MAX)  // ���� ������ �������� ������
		{
			Button_add->Enabled = false;
		}
	}
	// ���� ���� �������
	if (players_cur < 2)
		Button_start->Enabled = false;
	else
		Button_start->Enabled = true;
}

   // �������� ��������� ����� ������ � ����������� ������
//---------------------------------------------------------------------------
void __fastcall TForm_players::VT_playersGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)
{
	int index;

	index = Node->Index;
	switch(Column)
	{
		case 0:
			CellText = index + 1;
			break;
		case 1:
			CellText = players[index].Name;
			break;
		case 2:
			if (players[index].Color == clRed)    CellText = "�������";
			if (players[index].Color == clYellow) CellText = "������";
			if (players[index].Color == clGreen)  CellText = "�������";
			if (players[index].Color == clBlue)   CellText = "�����";
			if (players[index].Color == clSilver) CellText = "�����";
			break;
	}
}

// ����� ���������� ����� ������ ������
//---------------------------------------------------------------------------
void __fastcall TForm_players::VT_playersBeforeCellPaint(TBaseVirtualTree *Sender,
		  TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
		  TVTCellPaintMode CellPaintMode, TRect &CellRect, TRect &ContentRect)
{
	if (Column != 2)
	{
		TargetCanvas->Brush->Color = clWhite;
	}
	else
	{
		TargetCanvas->Brush->Color = players[Node->Index].Color;
	}

	TargetCanvas->FillRect(CellRect);
}
//---------------------------------------------------------------------------

void __fastcall TForm_players::FormShow(TObject *Sender)
{
	is_ok = false;

	// � ���� ���������� ������ ������ �������� ��� �����
	Form_new_player->allow_all_colors();

	if (players_cur < 2)
		Button_start->Enabled = false;
	else
		Button_start->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_players::Button_exitClick(TObject *Sender)
{
	is_ok = false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_players::Button_delClick(TObject *Sender)
{
	PVirtualNode 	Node;
	AnsiString   	question;
	int             i;

	Node = VT_players->GetFirstSelected(true);
	if (Node == NULL)
	{
		MessageBoxA(NULL, "����� ������� ������, ���� ��� �������� � ������", "��������", MB_OK + MB_ICONERROR);
		return;
	}

	question = "������� ������: ";
	question = question + players[Node->Index].Name;
	if (MessageBoxA(NULL, question.c_str(), "��������", MB_ICONQUESTION + MB_YESNO) != IDYES)
		return;

	// ������� ������
	for (i=Node->Index;i<4;i++)
	{
		// �������� ���� ������� ����
		players[i] = players[i+1];
	}

	// ���� �� ����� 5-�� �����, �� ������ � ��� �������
	players[4].clear();

	// ��������� ����� ������� �� 1
	players_cur = players_cur - 1;
	// ������� ������ � ������
	VT_players->RootNodeCount = players_cur;
	// ��������� ����� �� �� ������� ������ "������ ����" ��������
	if (players_cur > 1)
		Button_start->Enabled = true;
	else
        Button_start->Enabled = false;

}
//---------------------------------------------------------------------------

