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

	// игрок нажал на кнопку "Добавить"
	bool	is_ok;

	// нужно ли отображать цвета?
	// красный
	bool	is_need_red;
	// желтый
	bool	is_need_yellow;
	// зеленый
	bool	is_need_green;
	// синий
	bool	is_need_blue;
	// серый
	bool    is_need_silver;
	// функция обновить цвета в выпадающем списке
	void 	update_colors();
	// функция разрешить отображать все цвета
	void    allow_all_colors();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_new_player *Form_new_player;
//---------------------------------------------------------------------------
#endif
