//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form__zastavka.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_zastavka *Form_zastavka;
//---------------------------------------------------------------------------
__fastcall TForm_zastavka::TForm_zastavka(TComponent* Owner) : TForm(Owner) { }
//---------------------------------------------------------------------------
void __fastcall TForm_zastavka::Timer_closeTimer(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

