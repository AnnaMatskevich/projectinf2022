// ----------------------------------------------------------------------------
#ifndef _A_CARDS_HPP_
#define _A_CARDS_HPP_
// ----------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>

#include <stdlib.h>         // � ���� ������ ���� ��������� ��������� �����

// ----------------------------------------------------------------------------
// ����� �������� ������ 74: 0 - �������, 29-�� ������ ��������� ��������
// ----------------------------------------------------------------------------

#define CARDS_COUNT_MAX		74	// ���� ���������� � 1, 0-�� �������� ��� �������� ������� ������� �������� (������� ������� ��������)
#define CARD_WIDTH			86
#define CARD_HEIGHT 		86
#define CARD_SIZE   		87
// ----------------------------------------------------------------------------
// ������� ������� ��������
//      0
//    -----
//   |     |
//  3|  4  |1
//	 |     |
//	  -----
//      2
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ��������� ������ - ������� ������� ��������: ������, ����, �����
// ----------------------------------------------------------------------------
// ��� ����� ������� ��������(�� 5)
struct A_CardItem
{
	// ����������� (��������� ������������� �������� ������� ��������)
	A_CardItem() { clear(); }

	// �������� �������� - ������ �� ��� ���
	void clear() { item_type  = 0; item_connection = 0; item_price = 0; UserID = 0; x = 0; y = 0;}

	// ��� �������� ��������:
	// 0 - ������ ���, 1 - ������, 2 - ����, 3 - �����
	char   			item_type;

	// ��� ����������
	// ��� �����:                                     ��� ����:
	// 0 - ���,                                       0 - ���
	// 1 - ����� � ���� (�� ������������)             1 - �����
	// 2 - �����,                                     2 - ��������� � ��������������� ��������
	// 3 - ��������� c ������������� ��������,        3 - ��������� � ������ ��������
	// 4 - ��������� � ������ ��������,               4 - ��������� � ����� ��������
	// 5 - ��������� � ����� ��������                 5 - ��������� � ������ � ���������������
	//                								  6 - ��������� � ����� � ���������������
	//                                                7 - ��������� � ��������
	char 			item_connection;

	// ��������� (����� �������, �������, � ����� �� ���� ��� - ��� ������ ����� ���������)
	char 			item_price;

	// �������� (id ������������), 0 - ��� ���������
	unsigned int	UserID;

	// ���������� ���� ����� ���������� �����: (0,0) - ������ ����� ������� ������
	int				x;
	int				y;

	// ���������� ��������
	void set_type(char _item_type, char _item_connection, char _item_price)
	{
		item_type       = _item_type;
		item_connection = _item_connection;
		item_price      = _item_price;
	}

	// ���������� ���������� ��� �����
	void set_xy(int _x, int _y) { x = _x; y = _y; }

	// �������� ��������� �� ������� ������� �� 90'
	void rotate()
	{
		int tmp;
		if ((x == 0) && (y == 0))
			return;

		tmp = x;
		x = CARD_WIDTH - 1 - y;
		y = tmp;
	}
};


// ----------------------------------------------------------------------------
// ��������� ��� �������� � ���������
// ----------------------------------------------------------------------------
struct A_Card
{
	TPanel *	Panel;				// ������
	TImage *	Image;				// ��������

	// ������� �������� 0-3, 4-�����
	A_CardItem  card_items[5];

	// ����� �� ����?
	bool		on_field;

	// ����������� (��������� ������������� ������� ��������)
	A_Card()
	{
		Panel    = NULL;
		Image    = NULL;
		// �������� ������� ��������
		clear();
	}

	// �������� ������� ��������, ������ �����
	void 		clear()
	{
		// ������� ������� ��������
		if (Image != NULL) delete Image;
		// ����� ������� ������
		if (Panel != NULL) delete Panel;

		Panel    = NULL;
		Image    = NULL;
		on_field = false;
		card_items[0].clear();
		card_items[1].clear();
		card_items[2].clear();
		card_items[3].clear();
		card_items[4].clear();
	}

	// ���������������� �������� (��� ������ ������� ������): ������� ������ + ��������� ��������
	void 		init_paint(TComponent* Owner, TImageList * image_list, int index, TImage * image_target_enter, TImage * image_target_leave)
	{
		Panel = new TPanel(Owner);
		Image = new TImage(Panel);

		Panel->Parent   = (TWinControl *)Owner;
		Panel->Left     = 0;
		Panel->Top      = 0;
		Panel->Width    = CARD_WIDTH;
		Panel->Height   = CARD_HEIGHT;

		Image->Parent   = Panel;
		Image->Top      = 0;
		Image->Left     = 0;
		Image->AutoSize = true;

		image_list->GetBitmap(index, Image->Picture->Bitmap);

		Panel->Visible  = false;
	}

	// ����������� ������� �������� � ������ �����
	void 		move_to(int x, int y, bool is_visible)
	{
		Panel->Left     = x;
		Panel->Top      = y;
		Panel->Visible  = is_visible;
	}

	// ��������� �������� �� 90 �������� �� ������� �������
	void 		rotate()
	{
		A_CardItem		tmp;

		Graphics::TBitmap *SrcBitmap  = new Graphics::TBitmap();
		Graphics::TBitmap *DestBitmap = new Graphics::TBitmap();

		SrcBitmap->Assign(Image->Picture->Bitmap);

		DestBitmap->Width  = SrcBitmap->Height;
		DestBitmap->Height = SrcBitmap->Width;

		// �������� �� ������� �������

		for (int x=0;x<SrcBitmap->Width;x++)
		{
			for(int y=0;y<SrcBitmap->Height;y++)
			{
				DestBitmap->Canvas->Pixels[SrcBitmap->Width - 1 - y][x] = SrcBitmap->Canvas->Pixels[x][y];
			}
		}

		// ����� �����������

		Image->Picture->Bitmap = DestBitmap;

		delete DestBitmap;
		delete SrcBitmap;

		// ������� ���������� �����

		card_items[0].rotate(); // ������
		card_items[1].rotate(); // ������
		card_items[2].rotate(); // �����
		card_items[3].rotate(); // �����

		// ������� ������� ��������

		tmp = card_items[0];
		card_items[0] = card_items[3];
		card_items[3] = card_items[2];
		card_items[2] = card_items[1];
		card_items[1] = tmp;
	}
};
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// ����� ������ ����
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
class A_Cards
{
public:
	// ����������� (��������� ������������� ������� ������ ��������)
	A_Cards();

	// ����������
	~A_Cards();

	// �������� ��� ������� ��������
	void    clear();

	// ������������� ������� ��������
	bool	init(TComponent* Owner, TImageList * image_list, int ccards_count, TImage * image_target_enter, TImage * image_target_leave);

	// ���������� ��������
	void    shuffle_cards();

	// ���������� �������� � ����
	int		cards_count;

	// ������� ��������
	A_Card	card[CARDS_COUNT_MAX];

	// ������� ������� ����: [1; 73]
	// ������� ������� ��� ���� ����� ����� ���� ������������ ��������
	// 0 � 29 �������� �� ������������ (0-������� ����, 29 - ��������� ������� ��������)
	int     indexes[74];
};
// ----------------------------------------------------------------------------
inline
A_Cards::A_Cards()
{
	// �������������� ��������� ��������� �����
	randomize();

	// �������� ��� ��������
	clear();
}
// ----------------------------------------------------------------------------
inline
A_Cards::~A_Cards()
{
	// ������ �� ���� ������, �.�. ������� ����� ���� ������ ��� ����������� ��������
}
// ----------------------------------------------------------------------------
// �������� ��� ������� ��������
// ----------------------------------------------------------------------------
void
A_Cards::clear()
{
	int i;
	// ������� ������ ������� ��������
	for (i = 0; i < CARDS_COUNT_MAX; i++)
	{
		card[i].clear();
	}
	// ������ �� ���� �������� �� ������, ����� �������������
	cards_count = 0;
}
// ----------------------------------------------------------------------------
// ������������� ������� ��������
// ----------------------------------------------------------------------------
inline bool
A_Cards::init(TComponent* Owner, TImageList * image_list, int ccards_count, TImage * image_target_enter, TImage * image_target_leave)
{
	int	index;

	cards_count = 0;

	if (image_list->Count != ccards_count)
		return false;

	cards_count = ccards_count;
	// ������������� �������� �� ���������
	for (index = 0; index < cards_count; index++)
	{
		card[index].clear();
		card[index].init_paint(Owner, image_list, index, image_target_enter, image_target_leave);
	}

	// 0 ����� - ��� ��������� �������

	// 1 ��� ��������: � [1 �� 8]
	// ��� ����, ��� �����
	// � ���� �������� �� ������ �������, ����� 1 ��������
	for (index = 1; index < 9; ++index)
	{
		card[index].card_items[0].set_type(1, 3, 1); // ������ ���� ������
		card[index].card_items[1].set_type(0, 0, 0); // ������ ������ ���
		card[index].card_items[2].set_type(1, 3, 1); // ����� ����� ������
		card[index].card_items[3].set_type(0, 0, 0); // ����� ������ ���

		card[index].card_items[0].set_xy(42,25);
	}
	//
	card[1].card_items[0].item_price = 0;
	card[1].card_items[2].item_price = 0;

	card[3].card_items[0].set_xy(28,52);	// �� 3,4,5 �������� ���������� ��������
	card[4].card_items[0].set_xy(28,52);	// �� 3,4,5 �������� ���������� ��������
	card[5].card_items[0].set_xy(28,52);	// �� 3,4,5 �������� ���������� ��������

	// + 2 ��� �������� c [9 �� 13]
	// ��� ����, ��� �����
	// � ���� ��������� �� ������ �� 1 �������
	for (index = 9; index < 14; ++index)
	{
		card[index].card_items[0].set_type(1, 4, 1);	// ������ ������ ������
		card[index].card_items[1].set_type(1, 5, 1);	// ������ ����� ������
		card[index].card_items[2].set_type(0, 0, 0);	// ����� ������ ���
		card[index].card_items[3].set_type(0, 0, 0);	// ����� ������ ���

		card[index].card_items[0].set_xy(40,20);
	}

	// + 3 ��� �������� � [14 �� 22]
	// ��� ����, ��� �����
	for (index = 14; index < 23; ++index)
	{
		card[index].card_items[0].set_type(1, 2, 0);	// ������
		card[index].card_items[1].set_type(1, 2, 0);	// ������
		card[index].card_items[2].set_type(0, 0, 0);	// ����� ������ ���
		card[index].card_items[3].set_type(1, 2, 0);	// �����

		card[index].card_items[0].set_xy(44, 8);
		card[index].card_items[1].set_xy(78, 43);
		card[index].card_items[3].set_xy(8, 43);
	}

	card[17].card_items[1].item_price = 1;	// ������ �������
	card[17].card_items[3].item_price = 1;	// ����� �������

	card[19].card_items[3].item_price = 1;	// ����� �������

	card[20].card_items[1].item_price = 1;	// ������ �������

	card[22].card_items[0].item_price = 1;	// ������ �������

	// + 4 ��� �������� [23 - 1 �����]
	// ��� ����, ��� �����, ����� ���� � 6-7 �������
	index = 23;
	card[index].card_items[0].set_type(1, 2, 0);	// ������
	card[index].card_items[1].set_type(1, 2, 0);	// ������
	card[index].card_items[2].set_type(1, 2, 1);	// ����� �������
	card[index].card_items[3].set_type(1, 2, 0);	// �����

	card[index].card_items[0].set_xy(43,9);
	card[index].card_items[1].set_xy(76,42);
	card[index].card_items[2].set_xy(43,76);
	card[index].card_items[3].set_xy(9,42);

	// + 5 ��� �������� � [24 �� 28]
	// ��� �����, ��� �����
	// ����� ���� �����, ����� 24 ��������
	// ���� �������� ��� ������������ �����, ������ � ������
	for (index = 24; index < 29; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// ������ ������ ���
		card[index].card_items[1].set_type(0, 0, 0);	// ������ ������ ���
		card[index].card_items[2].set_type(2, 1, 1);	// ����� ���� � �������
		card[index].card_items[3].set_type(0, 0, 0);	// ����� ������ ���

		card[index].card_items[2].set_xy(32, 49);		// �����
	}
	card[24].card_items[2].item_price = 0;	// ����� ������ ���

	// + 6 ��� �������� c [29 �� 31]
	// ��� �����
	// ����� ���� �� ������ �������,
	// ����� ���� �����, ����� 31 ��������
	for (index = 29; index < 32; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// ������ �� ���� ���
		card[index].card_items[1].set_type(1, 3, 1);	// ������ ������ �� �������������� ������� � ��������
		card[index].card_items[2].set_type(2, 1, 1);	// ����� ���� � �������
		card[index].card_items[3].set_type(1, 3, 1);	// ����� ������ �� ��������������� ������� � ��������

		card[index].card_items[1].set_xy(13, 38);	// ������ ����� ��������� ������
		card[index].card_items[2].set_xy(22, 74);	// ����� ����� ��������� ����
	}

	card[31].card_items[2].item_price = 0;	// ����� �� ���� ������ ���

	// + 7 ��� �������� � [32 �� 33]
	// ��� �����
	// ����� ���� �������
	// �� ��� ��� �� ����� �� �����
	for (index = 32; index < 34; ++index)
	{
		card[index].card_items[0].set_type(1, 5, 1);	// ������ ������ ������ � ��������
		card[index].card_items[1].set_type(2, 1, 0);	// ������ ����
		card[index].card_items[2].set_type(0, 0, 0);	// ����� ������ ���
		card[index].card_items[3].set_type(1, 4, 1);	// ����� ������ ������ � ��������

		card[index].card_items[0].set_xy(9, 42);	// ����� �� ����� ����� ���������
		card[index].card_items[1].set_xy(76, 63);	// c����� ����, �� ����� ���������
	}

	// + 8 ��� �������� � [34 �� 35]
	// ��� �����
	// ����� ���� �������
	// ����� �� 34 ��������
	// �� ��� ��� ����� � �����
	for (index = 34; index < 36; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// ������ ������ ���
		card[index].card_items[1].set_type(2, 1, 0);	// ������ ���� �����
		card[index].card_items[2].set_type(1, 4, 1);	// ����� ������ ������ � ��������
		card[index].card_items[3].set_type(1, 5, 1);	// ����� ���� ������ � ��������

		card[index].card_items[3].set_xy(9, 42);		// ����� �� ����� ����� ���������
		card[index].card_items[1].set_xy(76, 63);		// c����� ����, �� ����� ���������
	}
	card[34].card_items[1].item_price = 1;				// ������ �� ���� �����

	// + 9 ��� �������� � [36 �� 38]
	// ��� �����
	// ��� ������� �� ����
	for (index = 36; index < 39; ++index)
	{
		card[index].card_items[0].set_type(1, 2, 0);	// ������ ������
		card[index].card_items[1].set_type(2, 1, 0);	// ������ ���� �����
		card[index].card_items[2].set_type(1, 2, 0);	// ����� ������
		card[index].card_items[3].set_type(1, 2, 0);	// ����� ������

		card[index].card_items[0].set_xy(44, 9);		// ������
		card[index].card_items[1].set_xy(77, 63);		// ������
		card[index].card_items[2].set_xy(44, 76);		// �����
		card[index].card_items[3].set_xy(16, 41);		// �����
	}

	card[36].card_items[0].item_price = 1;	// ������ �������
	card[37].card_items[0].item_price = 1;	// ������ �������
	card[38].card_items[2].item_price = 1;	// ����� �������

	// + 10 ��� �������� [39]
	// ��� �����
	// ������� �� ������ �����
	// ��� �� ����� �� �����
	index = 39;
	card[index].card_items[0].set_type(1, 2, 0);	// ������ ������ �����
	card[index].card_items[1].set_type(2, 1, 0);	// ������ ����
	card[index].card_items[2].set_type(1, 2, 1);	// ����� ������ �����
	card[index].card_items[3].set_type(0, 0, 0);	// ����� ������ ���

	card[index].card_items[0].set_xy(44, 9);	// ������
	card[index].card_items[1].set_xy(75, 54);	// ������
	card[index].card_items[2].set_xy(44, 76);	// �����

	// + 11 ��� �������� � [40 �� 41]
	// ��� �����
	// ����� ����� �� ����
	// �� 41 �������� ����� �������
	for (index = 40; index < 42; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// ������ ������ ���
		card[index].card_items[1].set_type(2, 1, 1);	// ������ ���� � �������
		card[index].card_items[2].set_type(1, 2, 0);	// ����� ������ �����
		card[index].card_items[3].set_type(1, 2, 0);	// ����� ������ �����

		card[index].card_items[1].set_xy(78, 57);	// ������
		card[index].card_items[2].set_xy(44, 76);	// �����
		card[index].card_items[3].set_xy(16, 41);	// �����
	}
	card[41].card_items[2].item_price = 1;			// c���� �������

	// + 12 ��� �������� � [42 �� 44]
	// ��� �����, ��� �����
	// ����� ����� �� ������, ������ ������ �� 43 ��������
	// �� ���� ��������� �� 1 ����� (������ ����)
	for (index = 42; index < 45; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// ������ ������ ���
		card[index].card_items[1].set_type(2, 1, 0);	// ������ ����
		card[index].card_items[2].set_type(0, 0, 0);	// ����� ������ ���
		card[index].card_items[3].set_type(2, 1, 1);	// ����� ����

		card[index].card_items[1].set_xy(77, 59);		// ������
		card[index].card_items[3].set_xy(9, 61);		// �����
	}
	card[43].card_items[1].item_price = 1;			   	// ������ �����

	// + 13 ��� �������� � [45 �� 47]
	// ��� �����, ��� �����
	// ����� �� ���� ��������� ��� ������, ������ ����
	// �� ���� ��������� ��� ���� � �����
	for (index = 45; index < 48; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// c����� ������ ���
		card[index].card_items[1].set_type(2, 1, 1);	// ������ ���� � �������
		card[index].card_items[2].set_type(2, 1, 0);	// ����� ����
		card[index].card_items[3].set_type(0, 0, 0);	// ����� ������ ���

		card[index].card_items[1].set_xy(77, 53);		// ������
		card[index].card_items[2].set_xy(28, 73);		// �����
	}

	// + 14 ��� �������� c [48 �� 50]
	// ��� �����, ��� �����
	// �� ���� ����� �� 1 ������
	for (index = 48; index < 51; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// ������ ������ ���
		card[index].card_items[1].set_type(2, 2, 1);	// ������ ����
		card[index].card_items[2].set_type(0, 0, 0);	// ����� ������ ���
		card[index].card_items[3].set_type(2, 2, 1);	// ����� ����

		card[index].card_items[1].set_xy(40, 42);		// ������
	}

	// + 15 ����� c [51 �� 52]
	// ��� �����, ��� �����
	// ����� ������� ���, ������ � ����� �� 1 ������
	// ��� ���� � ����� �� ���� ���������
	for (index = 51; index < 53; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// ������ ������ ���
		card[index].card_items[1].set_type(2, 3, 1);	// ������ ���� ���� � �������
		card[index].card_items[2].set_type(2, 4, 1);	// ����� ������ ���� � �������
		card[index].card_items[3].set_type(2, 1, 0);	// ����� ���� �����

		card[index].card_items[1].set_xy(75, 30);		// ������ ���� ����� ��������� ����
		card[index].card_items[3].set_xy(8, 25);		// ����� ����� ��������� ����
	}

	// + 16 ��� �������� � [53 �� 58]
	// ��� �����, ��� �����
	// �� ���� ����� �� 1 ������
	for (index = 53; index < 59; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);	// ������
		card[index].card_items[1].set_type(2, 3, 1);	// ������ ���� ���� � �������
		card[index].card_items[2].set_type(2, 4, 1);	// ����� ������ ���� � �������
		card[index].card_items[3].set_type(0, 0, 0);	// �����

		card[index].card_items[1].set_xy(69, 36);		// ������ ���� ����� ���������
	}

	// + 17 ��� �������� � [59 �� 63]
	// ��� �����
	// �� ���� ��������� �� 1 ������, ����� 63 ��������
	// �� 62 � 63 ��������� �� ������ ����� �� �������
	// ��� ���� � �����
	for (index = 59; index < 64; ++index)
	{
		card[index].card_items[0].set_type(1, 2, 0);	// ������ ������ �����
		card[index].card_items[1].set_type(2, 3, 1);	// ������ ���� ����
		card[index].card_items[2].set_type(2, 4, 1);	// ����� ������ ����
		card[index].card_items[3].set_type(1, 2, 0);	// ����� ������ �����

		card[index].card_items[0].set_xy(44, 12);		// ������
		card[index].card_items[1].set_xy(36, 74);		// ������ ���� ����� ��������� ����
		card[index].card_items[3].set_xy(9, 44);		// �����
	}

	card[63].card_items[1].item_price = 0;	// ������ ��� ������
	card[63].card_items[2].item_price = 0;	// ����� ��� ������

	card[62].card_items[3].item_price = 1;	// ����� ���� �������
	card[63].card_items[3].item_price = 1;	// ����� ���� �������

	// + 18 ��� �������� [64]
	// ��� �����, ��� �����
	// ��� ���� � �����
	index = 64;
	card[index].card_items[0].set_type(0, 0, 0);	// ������ ������ ���
	card[index].card_items[1].set_type(2, 5, 2);	// ������ ����
	card[index].card_items[2].set_type(2, 7, 2);	// ����� ����
	card[index].card_items[3].set_type(2, 6, 2);	// ����� ����

	card[index].card_items[1].set_xy(14, 74);		// ������ / ����� / ����� ����� ��������� ����

	// + 19 ��� �������� � [65 �� 67]
	// ��� �����
	// �� ���� �� 2 ������ �����, ����� 67 - �� ��� 1 �����
	// �� 66 �������� �� ������ ������ �������
	// ��� ���� � �����
	for (index = 65; index < 68; ++index)
	{
		card[index].card_items[0].set_type(1, 2, 0);	// ������ ������ �����
		card[index].card_items[1].set_type(2, 5, 2);	// ������ ����
		card[index].card_items[2].set_type(2, 7, 2);	// ����� ����
		card[index].card_items[3].set_type(2, 6, 2);	// ����� ����

		card[index].card_items[0].set_xy(41, 9);		// ������
		card[index].card_items[2].set_xy(14, 74);		// �����
	}
	card[66].card_items[0].item_price = 1;	// ������ �� ������ �������

	card[67].card_items[1].item_price = 1;	// ������ 1 �����
	card[67].card_items[2].item_price = 1;	// ����� 1 �����
	card[67].card_items[3].item_price = 1;	// ����� 1 �����

	// !!! ����� �� ��������� ��� ������ ��������������� !!!
	// !!! ��� �������, �������, �����, ������ ������ ���� !!!

	// 20 ����� (��� ��������) - 68 ��������
	index = 68;
	card[index].card_items[0].set_type(1, 2, 0);		// ������ ������
	card[index].card_items[1].set_type(1, 2, 0);		// ������ ������
	card[index].card_items[2].set_type(1, 2, 0);		// ����� ������
	card[index].card_items[3].set_type(0, 0, 0);		// ����� ������ ���
	card[index].card_items[4].set_type(3, 0, 0);    	// ����� ������������ (��� ������ ���� � ����� 2)

	card[index].card_items[0].set_xy(42, 9);			// ������
	card[index].card_items[1].set_xy(79, 42);			// ������
	card[index].card_items[2].set_xy(42, 78);			// �����
	card[index].card_items[4].set_xy(42, 42);

	// 21 ����� (��� ��������) - 69 ��������
	index = 69;
	card[index].card_items[0].set_type(1, 2, 0);		// ������ ������ �����
	card[index].card_items[1].set_type(0, 0, 0);		// ������ ������ ���
	card[index].card_items[2].set_type(1, 2, 0);		// ����� ������ �����
	card[index].card_items[3].set_type(0, 0, 0);		// ����� ������ ���
	card[index].card_items[4].set_type(3, 0, 0);    	// ����� ������������ (��� ������ ���� � ����� 2)

	card[index].card_items[0].set_xy(42, 9);	// ������
	card[index].card_items[2].set_xy(42, 78);	// �����
	card[index].card_items[4].set_xy(42,42);

	// 22 ����� (��� ��������) � [70 �� 71]
	for (index = 70; index < 72; ++index)
	{
		card[index].card_items[0].set_type(0, 0, 0);		// ������
		card[index].card_items[1].set_type(1, 2, 0);		// ������
		card[index].card_items[2].set_type(0, 0, 0);		// �����
		card[index].card_items[3].set_type(0, 0, 0);		// �����
		card[index].card_items[4].set_type(3, 0, 0);    	// ����� ������������ (��� ������ ���� � ����� 2)

		card[index].card_items[1].set_xy(76, 42);			// ������
		card[index].card_items[4].set_xy(42, 42);            // �����
	}

	// 23 ����� (��� ��������) - 72 ��������
	index = 72;
	card[index].card_items[0].set_type(1, 2, 0);		// ������ ������
	card[index].card_items[1].set_type(1, 2, 0);		// ������ ������
	card[index].card_items[2].set_type(0, 0, 0);		// ����� ������ ���
	card[index].card_items[3].set_type(0, 0, 0);		// ����� ������ ���
	card[index].card_items[4].set_type(3, 0, 0);    	// ����� ������������ (��� ������ ���� � ����� 2)

	card[index].card_items[0].set_xy(44, 9);	// ������
	card[index].card_items[1].set_xy(79, 42);	// ������
	card[index].card_items[4].set_xy(42, 42);

	// 24 ����� (��� �����, ��� ��������) - 73 ��������
	index = 73;
	card[index].card_items[4].set_type(3, 0, 0);    // ����� ������������
	card[index].card_items[4].set_xy(42, 42);

	return true;
}
// ----------------------------------------------------------------------------
// ���������� ��������
// ----------------------------------------------------------------------------
void
A_Cards::shuffle_cards()
{
	int 	i, n;
	int     rnd_count, rnd_index_1, rnd_index_2, index_tmp;

	// ����� ������������: [200; 300]
	rnd_count = 200 + (rand() % 101);

	// ��������� ������������� ������� �������� ������� ��������
	n = 74;
	for (i=0;i<n;i++)
	{
		indexes[i] = i;
	}

	// ������������ ������� ������� ��������
	for (i=0;i<rnd_count;i++)
	{
		rnd_index_1 = 1 + (rand() % 73);
		rnd_index_2 = 1 + (rand() % 73);

		if (rnd_index_1 == 0) rnd_index_1 = 1;
		if (rnd_index_2 == 0) rnd_index_2 = 1;

		if (rnd_index_1 == 29) rnd_index_1 = 30;
		if (rnd_index_2 == 29) rnd_index_2 = 30;

		index_tmp = indexes[rnd_index_1];
		indexes[rnd_index_1] = indexes[rnd_index_2];
		indexes[rnd_index_2] = index_tmp;
	}

}
// ----------------------------------------------------------------------------





#endif
// ----------------------------------------------------------------------------

