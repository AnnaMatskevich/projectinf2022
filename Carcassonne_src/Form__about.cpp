//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form__about.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_about *Form_about;
//---------------------------------------------------------------------------
__fastcall TForm_about::TForm_about(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_about::Button_closeClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
