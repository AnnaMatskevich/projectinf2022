// ----------------------------------------------------------------------------
#ifndef _A_TARGET_HPP_
#define _A_TARGET_HPP_
// ----------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
// ----------------------------------------------------------------------------
class A_Target
{
public:
	// конструктор
	A_Target()
	{
		Image = NULL;
	}

	// деструктор
	~A_Target()
	{
		if (Image != NULL)
			delete Image;
	}
	// отобразить
	void show(TPanel * panel, int x, int y, TPicture * picture, TNotifyEvent on_mouse_enter, TNotifyEvent on_mouse_leave, TNotifyEvent on_click)
	{
		Image = new TImage(panel);
		Image->Parent      = panel;
		Image->Picture     = picture;
		Image->AutoSize    = true;
		Image->Transparent = true;
		Image->Left        = x - 8;
		Image->Top         = y - 8;
		Image->Visible     = true;

		Image->OnMouseEnter = on_mouse_enter;
		Image->OnMouseLeave = on_mouse_leave;
		Image->OnClick      = on_click;

		panel->Invalidate();
	}
	// картинка
	TImage *  	Image;

};
// ----------------------------------------------------------------------------
#endif
