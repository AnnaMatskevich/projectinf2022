//---------------------------------------------------------------------------

#ifndef Form__mainH
#define Form__mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Graphics.hpp>
#include "VirtualTrees.hpp"
#include <Vcl.Buttons.hpp>

#include "A_Player.hpp"
#include "A_Cards.hpp"
#include "A_Field.hpp"
#include "A_Target.hpp"
#include "A_Chip.hpp"

#include <System.ImageList.hpp>
#include <Vcl.Menus.hpp>


//---------------------------------------------------------------------------
class TForm_main : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_player_turn_end;
	TScrollBox *ScrollBox_field;
	TImage *Image_field;
	TImage *Image_cur;
	TImageList *ImageList_cards_old;
	TPanel *Panel2;
	TPanel *Panel_card_cur;
	TEdit *Edit_number;
	TLabel *Label_desc_1;
	TLabel *Label_desc_2;
	TImage *Image_tuple_red;
	TImage *Image_target_leave;
	TImage *Image_target_enter;
	TImage *Image_tuple_green;
	TImage *Image_tuple_blue;
	TImage *Image_tuple_yellow;
	TImage *Image_tuple_silver;
	TPanel *Panel_top;
	TSplitter *Splitter2;
	TPanel *Panel_user_0;
	TImage *Image_user_00;
	TLabel *Label_user_bananas_count_0;
	TImage *Image_user_01;
	TLabel *Label_user_shells_count_0;
	TLabel *Label_user_00;
	TImage *Image_user_tuple_0;
	TLabel *Label_user_tuples_count_0;
	TPanel *Panel_top_user_0;
	TPanel *Panel_user_vert_3;
	TPanel *Panel9;
	TPanel *Panel10;
	TPanel *Panel_user_vert_1;
	TPanel *Panel_user_vert_2;
	TPanel *Panel_user_1;
	TImage *Image_user_10;
	TLabel *Label_user_bananas_count_1;
	TImage *Image_user_11;
	TLabel *Label_user_shells_count_1;
	TImage *Image_user_tuple_1;
	TLabel *Label_user_tuples_count_1;
	TPanel *Panel_top_user_1;
	TPanel *Panel_user_2;
	TImage *Image_user_20;
	TLabel *Label_user_bananas_count_2;
	TImage *Image_user_21;
	TLabel *Label_user_shells_count_2;
	TImage *Image_user_tuple_2;
	TLabel *Label_user_tuples_count_2;
	TPanel *Panel_top_user_2;
	TPanel *Panel_user_vert_4;
	TPanel *Panel_user_3;
	TImage *Image_user_30;
	TLabel *Label_user_bananas_count_3;
	TImage *Image_user_31;
	TLabel *Label_user_shells_count_3;
	TImage *Image_user_tuple_3;
	TLabel *Label_user_tuples_count_3;
	TPanel *Panel_top_user_3;
	TPanel *Panel_user_vert_0;
	TPanel *Panel_user_4;
	TImage *Image_user_40;
	TLabel *Label_user_bananas_count_4;
	TImage *Image_user_41;
	TLabel *Label_user_shells_count_4;
	TImage *Image_user_tuple_4;
	TLabel *Label_user_tuples_count_4;
	TPanel *Panel_top_user_4;
	TLabel *Label_user_points_0;
	TLabel *Label_user_points_1;
	TLabel *Label_user_10;
	TLabel *Label_user_points_2;
	TLabel *Label_user_20;
	TLabel *Label_user_points_3;
	TLabel *Label_user_30;
	TLabel *Label_user_points_4;
	TLabel *Label_user_40;
	TMainMenu *MainMenu1;
	TMenuItem *N_finish_start;
	TMenuItem *N_close;
	TLabel *Label_desc_0;
	TLabel *Label1;
	TBevel *Bevel1;
	TImage *Image_user_02;
	TLabel *Laber_user_makets_count_0;
	TImage *Image_user_12;
	TLabel *Laber_user_makets_count_1;
	TImage *Image_user_22;
	TLabel *Laber_user_makets_count_2;
	TImage *Image_user_32;
	TLabel *Laber_user_makets_count_3;
	TImage *Image_user_42;
	TLabel *Laber_user_makets_count_4;
	TMenuItem *N_about;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label_cards_free;
	TLabel *Label5;
	TMenuItem *N_rules;
	TImageList *ImageList_cards;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Image_fieldMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Image_fieldMouseEnter(TObject *Sender);
	void __fastcall Image_fieldMouseLeave(TObject *Sender);
	void __fastcall Image_fieldClick(TObject *Sender);
	void __fastcall Image_curClick(TObject *Sender);

	// сво€ функци€ обработки оконных сообщений
	void __fastcall A_WndProc(Messages::TMessage &Message);
	void __fastcall Button_player_turn_endClick(TObject *Sender);
	void __fastcall N_closeClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall N_finish_startClick(TObject *Sender);
	void __fastcall N_aboutClick(TObject *Sender);
	void __fastcall N_rulesClick(TObject *Sender);


private:	// User declarations
public:		// User declarations
	__fastcall TForm_main(TComponent* Owner);

	// отрисовать игровое поле
	void PaintField();
	void PaintRect(TCanvas * canvas, int X_field, int Y_field);
	int	 X_field_0;
	int	 Y_field_0;
	int	 X_cur;
	int  Y_cur;

	// Ќачало хода игрока: отобразить им€ текущего игрока и его игровую карточку
	bool	PlayerTurnStart();

	//  онец хода игрока: проверить все ли игрок за ход сделал, перейти к другому игроку и другой карточки
	void 	PlayerTurnEnd();

	// »грок положил текущую карточку? если нет, то перехода хода нет
	bool	is_put_card;

	// ƒействи€, когда положили игровую карточку на игровое поле
	void	CardPut(int x_field, int y_field, A_Card * card);

	// Ќайти координаты карточки на поле
	bool    CardGetXY(A_Card * card, int & x_field, int & y_field);

	// вз€ть очередную карточку
	void 	TakeCard();

	// ћишени дл€ захвата на карточке игровых элементов
	A_Target  *	target[10];

	// ќтобразить мишени на игровых элементах карточки, которые можно захватить
	// эта функци€ вызываетс€ после того как положили игровую карточку на игровое поле
	void 	ShowTargets();

	// ”брать все мишени дл€ захвата
	// эта функци€ вызываетс€ после того, как игрок нажал на кнопку '«акончить ход'
	void    HideTargets();

	// событие, которое возникает когда "мышкой" навели на мишень
	void __fastcall Image_target_fieldMouseEnter(TObject *Sender);

	// событие, которое возникает когда "мышка" покинула пределы мишени
	void __fastcall Image_target_fieldMouseLeave(TObject *Sender);

	// событие, которое возникает когда "мышкой" нажали на мишень,
	//          т.е. игрок хочет захватить игровой эелмент карточки
	void __fastcall Image_target_fieldClick(TObject *Sender);

	// ƒействие: после того как заканчиваем ход, провер€ем
	// может быть надо убрать какую-то фишку и начислить очков
	void    CheckAllTuples();

	void __fastcall Image_tuple_MouseEnter(TObject *Sender);
	void __fastcall Image_tuple_MouseLeave(TObject *Sender);
	void __fastcall Image_tuple_Click(TObject *Sender);

	// панель очков игроков

	// создать панель с очками игров
	void    CreatePlayersPanel();

	// одновить очки игроков
	void    UpdatePlayersPanel();

	// Ќачать новую игру
	bool    CreateNewGame();

	// «акончить игру
	void    FinishGame();

	// »гра закончилась?
	bool    is_game_finished;



};
//---------------------------------------------------------------------------
extern PACKAGE TForm_main *Form_main;
//---------------------------------------------------------------------------
#endif
