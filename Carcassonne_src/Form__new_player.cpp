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
	// ��� ������ ������� ���� ���������� ��� ��������� ����� � ���������� ������
	allow_all_colors();
}
//---------------------------------------------------------------------------
// ������� �� ����������� ����
//---------------------------------------------------------------------------
void __fastcall TForm_new_player::FormShow(TObject *Sender)
{
    // �� ���������: ����� �������� �� ������ ����� �������� � ����
	is_ok = false;

	// ������� ����� � ���������� ������
	update_colors();

	// ������ ������ ������
	Edit_player_name->Text = "< ������� ��� ������ >";
	Edit_player_name->SelectAll();
	Edit_player_name->SetFocus();
}
//---------------------------------------------------------------------------
// �� ������� �� ������ '������'
//---------------------------------------------------------------------------
void __fastcall TForm_new_player::Button_cancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
// �� ������� �� ������ '��������'
//---------------------------------------------------------------------------
void __fastcall TForm_new_player::Button_addClick(TObject *Sender)
{
	if ((Edit_player_name->Text == "") ||
		(Edit_player_name->Text == "< ������� ��� ������ >"))
	{
		MessageBoxA(NULL, "���������� ������ ��� ������", "��������", MB_OK + MB_ICONERROR);
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

	if (is_need_red    == true) ColorBox_colors->AddItem("�������",(TObject*)clRed);
	if (is_need_yellow == true) ColorBox_colors->AddItem("������", (TObject*)clYellow);
	if (is_need_green  == true) ColorBox_colors->AddItem("�������",(TObject*)clGreen);
	if (is_need_blue   == true) ColorBox_colors->AddItem("�����",  (TObject*)clBlue);
	if (is_need_silver == true) ColorBox_colors->AddItem("�����",  (TObject*)clSilver);

	ColorBox_colors->ItemIndex = 0;
}
//---------------------------------------------------------------------------
// ������� ��������� ���������� ��� �����
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

