//---------------------------------------------------------------------------

#ifndef Form__new_playerH
#define Form__new_playerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_new_player : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Edit_game_name;
	TLabel *Label2;
	TEdit *Edit_player_name;
	TLabel *Label3;
	TColorBox *ColorBox_colors;
	TButton *Button_add;
	TButton *Button_cancel;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_cancelClick(TObject *Sender);
	void __fastcall Button_addClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm_new_player(TComponent* Owner);

	// ����� ����� �� ������ "��������"
	bool	is_ok;

	// ����� �� ���������� �����?
	// �������
	bool	is_need_red;
	// ������
	bool	is_need_yellow;
	// �������
	bool	is_need_green;
	// �����
	bool	is_need_blue;
	// �����
	bool    is_need_silver;
	// ������� �������� ����� � ���������� ������
	void 	update_colors();
	// ������� ��������� ���������� ��� �����
	void    allow_all_colors();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_new_player *Form_new_player;
//---------------------------------------------------------------------------
#endif
