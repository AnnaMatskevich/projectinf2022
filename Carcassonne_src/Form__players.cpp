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
	// установить активным игроком игрока с номером 0 (он ходит первым)
	player_index = 0;

	// все хорошо надо продолжать играть
	is_ok        = true;

    // закрываем окно с игроками
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_players::Button_addClick(TObject *Sender)
{
	UnicodeString 	color_name;

	Form_new_player->ShowModal();
	if (Form_new_player->is_ok == true) // если корректно заполнили данные новогоо игрока
	{
		players[players_cur].clear();
		players[players_cur].Name             = Form_new_player->Edit_player_name->Text;
		players[players_cur].Color            = Form_new_player->ColorBox_colors->Selected;
		players[players_cur].UserID           = players_cur + 1;

		// смотрим какие цвета остались на выбор
		color_name = Form_new_player->ColorBox_colors->Items->Strings[Form_new_player->ColorBox_colors->ItemIndex];
		if (color_name == "Красный")
			Form_new_player->is_need_red = false;
		if (color_name == "Желтый")
			Form_new_player->is_need_yellow = false;
		if (color_name == "Зеленый")
			Form_new_player->is_need_green = false;
		if (color_name == "Синий")
			Form_new_player->is_need_blue = false;
		if (color_name == "Серый")
			Form_new_player->is_need_silver = false;

		Form_new_player->update_colors();

		players_cur = players_cur + 1; // кол-во учеников +1

		VT_players->RootNodeCount = players_cur; //??? чего

		if (players_cur == PLAYERS_MAX)  // если больше добавить нельзя
		{
			Button_add->Enabled = false;
		}
	}
	// если мало игроков
	if (players_cur < 2)
		Button_start->Enabled = false;
	else
		Button_start->Enabled = true;
}

   // просмотр столбиков одной строки и отображение текста
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
			if (players[index].Color == clRed)    CellText = "Красный";
			if (players[index].Color == clYellow) CellText = "Желтый";
			if (players[index].Color == clGreen)  CellText = "Зеленый";
			if (players[index].Color == clBlue)   CellText = "Синий";
			if (players[index].Color == clSilver) CellText = "Серый";
			break;
	}
}

// перед написанием теста красим ячейку
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

	// в окне добавления нового игрока доступны все цвета
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
		MessageBoxA(NULL, "Чтобы удалить игрока, надо его выделить в списке", "Внимание", MB_OK + MB_ICONERROR);
		return;
	}

	question = "Удалить игрока: ";
	question = question + players[Node->Index].Name;
	if (MessageBoxA(NULL, question.c_str(), "Внимание", MB_ICONQUESTION + MB_YESNO) != IDYES)
		return;

	// удаляем игрока
	for (i=Node->Index;i<4;i++)
	{
		// сдвигаем всех игроков вниз
		players[i] = players[i+1];
	}

	// если бы задан 5-ый игрок, то данные о нем очищаем
	players[4].clear();

	// уменьшаем число игроков на 1
	players_cur = players_cur - 1;
	// обновим строки в списке
	VT_players->RootNodeCount = players_cur;
	// проверяем можем ли мы сделать кнопку "Начать игру" активной
	if (players_cur > 1)
		Button_start->Enabled = true;
	else
        Button_start->Enabled = false;

}
//---------------------------------------------------------------------------

