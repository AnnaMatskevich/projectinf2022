//---------------------------------------------------------------------------

#ifndef Form__zastavkaH
#define Form__zastavkaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TForm_zastavka : public TForm
{
__published:	// IDE-managed Components
	TTimer *Timer_close;
	TImage *Image_zastavka;
	void __fastcall Timer_closeTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_zastavka(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_zastavka *Form_zastavka;
//---------------------------------------------------------------------------
#endif
