// ----------------------------------------------------------------------------
#ifndef _A_CHIP_HPP_
#define _A_CHIP_HPP_
// ----------------------------------------------------------------------------
#include <Vcl.ExtCtrls.hpp>
// ----------------------------------------------------------------------------
#include "A_Cards.hpp"
// ----------------------------------------------------------------------------
// Исходные данные:
// фишка принадлежит конкретному игроку (UserID);
// фишка может быть выставлена на игровую карточку (Card, координаты x,y);
// фишку можно убрать с игрового поля (игровой карточки), для этого надо:
// отслеживать, что игрок навел мышку на фишку (on_mouse_enter)
// отслеживать, что игрок покинул пределы фишки (on_mouse_leave)
// отслеживать, что игрок нажал мышкой на фишку (on_click)
// !!! координаты фишки можно узнать по кординатам Image x=Left, y=Top
// ----------------------------------------------------------------------------
class A_Chip
{
public:
	// конструктор
	A_Chip()
	{
		Image  = NULL;
		Card   = NULL;
		IsUsed = false;
	}

	// деструктор
	~A_Chip()
	{
		//hide();
	}

	// отобразить фишку на конкретной игровой карточке
	void show(A_Card * card, int x, int y, TPicture * picture, TNotifyEvent on_mouse_enter, TNotifyEvent on_mouse_leave, TNotifyEvent on_click)
	{
		Card                = card;

		Image               = new TImage(Card->Panel);
		Image->Parent       = Card->Panel;
		Image->Picture      = picture;
		Image->AutoSize     = true;
		Image->Transparent  = true;// что бы фон был прозрачным у человечка
		Image->Left         = x - 8;
		Image->Top          = y - 8;
		Image->OnMouseEnter = on_mouse_enter;
		Image->OnMouseLeave = on_mouse_leave;
		Image->OnClick      = on_click;
		// перерисовать игровую карточку
		Image->Visible      = true;
		Card->Panel->Invalidate();//перерисовать

        IsUsed              = true;
	}

	// убрать фишку с игровой карточки (поля)
	void hide()
	{
		if (Image != NULL)
		try {
			delete Image;
		} catch (...) { }

		// фишка не используется
		Image  = NULL;
		Card   = NULL;
		IsUsed = false;
	}

	// игровая карточка
	// у карточки есть TPanel на которую выставляются все игровые элементы в том числе фишки
	A_Card *    	Card;

	// картинка
	TImage *  		Image;

	// фишка используется? т.е. отображена на игровом поле?
	bool            IsUsed;
};
// ----------------------------------------------------------------------------
#endif
