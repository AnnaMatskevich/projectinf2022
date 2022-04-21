//---------------------------------------------------------------------------

#ifndef Form__aboutH
#define Form__aboutH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TForm_about : public TForm
{
__published:	// IDE-managed Components
	TImage *Image_zastavka;
	TLabel *Label1;
	TImage *Image1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TPanel *Panel_main;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TButton *Button_close;
	TBevel *Bevel1;
	void __fastcall Button_closeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_about(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_about *Form_about;
//---------------------------------------------------------------------------
#endif
