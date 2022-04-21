//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form__new_player.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_new_player *Form_new_player;
//---------------------------------------------------------------------------
__fastcall TForm_new_player::TForm_new_player(TComponent* Owner)
	: TForm(Owner)
{
	// при первом запуске надо отображать все доступные цвета в выпадающем списке
	allow_all_colors();
}
//---------------------------------------------------------------------------
// Событие на отображение окна
//---------------------------------------------------------------------------
void __fastcall TForm_new_player::FormShow(TObject *Sender)
{
    // по умлочанию: игрок нажимает на отмену ввода сведение о себе
	is_ok = false;

	// обновим цвета в выпадающем списке
	update_colors();

	// вводим нового игрока
	Edit_player_name->Text = "< введите имя игрока >";
	Edit_player_name->SelectAll();
	Edit_player_name->SetFocus();
}
//---------------------------------------------------------------------------
// На нажитие на кнопку 'Отмена'
//---------------------------------------------------------------------------
void __fastcall TForm_new_player::Button_cancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
// На нажитие на кнопку 'Добавить'
//---------------------------------------------------------------------------
void __fastcall TForm_new_player::Button_addClick(TObject *Sender)
{
	if ((Edit_player_name->Text == "") ||
		(Edit_player_name->Text == "< введите имя игрока >"))
	{
		MessageBoxA(NULL, "Необходимо ввести имя игрока", "Внимание", MB_OK + MB_ICONERROR);
		Edit_player_name->SelectAll();
		Edit_player_name->SetFocus();
		return;
	}

	is_ok = true;
	Close();
}
//---------------------------------------------------------------------------
void 	TForm_new_player::update_colors()
{
	ColorBox_colors->Items->Clear();

	if (is_need_red    == true) ColorBox_colors->AddItem("Красный",(TObject*)clRed);
	if (is_need_yellow == true) ColorBox_colors->AddItem("Желтый", (TObject*)clYellow);
	if (is_need_green  == true) ColorBox_colors->AddItem("Зеленый",(TObject*)clGreen);
	if (is_need_blue   == true) ColorBox_colors->AddItem("Синий",  (TObject*)clBlue);
	if (is_need_silver == true) ColorBox_colors->AddItem("Серый",  (TObject*)clSilver);

	ColorBox_colors->ItemIndex = 0;
}
//---------------------------------------------------------------------------
// функция разрешить отображать все цвета
//---------------------------------------------------------------------------
void
TForm_new_player::allow_all_colors()
{
	is_need_red    = true;
	is_need_yellow = true;
	is_need_green  = true;
	is_need_blue   = true;
	is_need_silver = true;
}
//---------------------------------------------------------------------------

