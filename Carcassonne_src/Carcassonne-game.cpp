//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------




















USEFORM("Form__zastavka.cpp", Form_zastavka);
USEFORM("Form__players.cpp", Form_players);
USEFORM("Form__new_player.cpp", Form_new_player);
USEFORM("Form__main.cpp", Form_main);
USEFORM("Form__about.cpp", Form_about);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm_main), &Form_main);
		Application->CreateForm(__classid(TForm_zastavka), &Form_zastavka);
		Application->CreateForm(__classid(TForm_players), &Form_players);
		Application->CreateForm(__classid(TForm_new_player), &Form_new_player);
		Application->CreateForm(__classid(TForm_about), &Form_about);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
