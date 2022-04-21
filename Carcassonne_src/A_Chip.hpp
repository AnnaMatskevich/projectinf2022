// ----------------------------------------------------------------------------
#ifndef _A_CHIP_HPP_
#define _A_CHIP_HPP_
// ----------------------------------------------------------------------------
#include <Vcl.ExtCtrls.hpp>
// ----------------------------------------------------------------------------
#include "A_Cards.hpp"
// ----------------------------------------------------------------------------
// �������� ������:
// ����� ����������� ����������� ������ (UserID);
// ����� ����� ���� ���������� �� ������� �������� (Card, ���������� x,y);
// ����� ����� ������ � �������� ���� (������� ��������), ��� ����� ����:
// �����������, ��� ����� ����� ����� �� ����� (on_mouse_enter)
// �����������, ��� ����� ������� ������� ����� (on_mouse_leave)
// �����������, ��� ����� ����� ������ �� ����� (on_click)
// !!! ���������� ����� ����� ������ �� ���������� Image x=Left, y=Top
// ----------------------------------------------------------------------------
class A_Chip
{
public:
	// �����������
	A_Chip()
	{
		Image  = NULL;
		Card   = NULL;
		IsUsed = false;
	}

	// ����������
	~A_Chip()
	{
		//hide();
	}

	// ���������� ����� �� ���������� ������� ��������
	void show(A_Card * card, int x, int y, TPicture * picture, TNotifyEvent on_mouse_enter, TNotifyEvent on_mouse_leave, TNotifyEvent on_click)
	{
		Card                = card;

		Image               = new TImage(Card->Panel);
		Image->Parent       = Card->Panel;
		Image->Picture      = picture;
		Image->AutoSize     = true;
		Image->Transparent  = true;// ��� �� ��� ��� ���������� � ���������
		Image->Left         = x - 8;
		Image->Top          = y - 8;
		Image->OnMouseEnter = on_mouse_enter;
		Image->OnMouseLeave = on_mouse_leave;
		Image->OnClick      = on_click;
		// ������������ ������� ��������
		Image->Visible      = true;
		Card->Panel->Invalidate();//������������

        IsUsed              = true;
	}

	// ������ ����� � ������� �������� (����)
	void hide()
	{
		if (Image != NULL)
		try {
			delete Image;
		} catch (...) { }

		// ����� �� ������������
		Image  = NULL;
		Card   = NULL;
		IsUsed = false;
	}

	// ������� ��������
	// � �������� ���� TPanel �� ������� ������������ ��� ������� �������� � ��� ����� �����
	A_Card *    	Card;

	// ��������
	TImage *  		Image;

	// ����� ������������? �.�. ���������� �� ������� ����?
	bool            IsUsed;
};
// ----------------------------------------------------------------------------
#endif
