//---------------------------------------------------------------------------

#ifndef Form__playersH
#define Form__playersH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VirtualTrees.hpp"
#include <Vcl.ExtCtrls.hpp>

#include "A_Player.hpp"
#include "Form__new_player.h"
//---------------------------------------------------------------------------
class TForm_players : public TForm
{
__published:	// IDE-managed Components
	TVirtualStringTree *VT_players;
	TPanel *Panel1;
	TButton *Button_add;
	TButton *Button_del;
	TPanel *Panel2;
	TButton *Button_start;
	TButton *Button_exit;
	void __fastcall Button_startClick(TObject *Sender);
	void __fastcall Button_addClick(TObject *Sender);
	void __fastcall VT_playersGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall VT_playersBeforeCellPaint(TBaseVirtualTree *Sender, TCanvas *TargetCanvas,
          PVirtualNode Node, TColumnIndex Column, TVTCellPaintMode CellPaintMode,
          TRect &CellRect, TRect &ContentRect);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_exitClick(TObject *Sender);
	void __fastcall Button_delClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_players(TComponent* Owner);

	#define 		PLAYERS_MAX		5

	A_Player		players[PLAYERS_MAX];
	int				players_cur;
	int				player_index;
    bool            is_ok;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_players *Form_players;
//---------------------------------------------------------------------------
#endif
