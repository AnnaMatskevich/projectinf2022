//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "Form__main.h"
#include "Form__zastavka.h"
#include "Form__players.h"
#include "Form__about.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm_main *	Form_main;

// колода с игровыми карточками
A_Cards			cards;
// игровое поле
A_Field			field;

// все игроки и их описание находятся в Form_players

//---------------------------------------------------------------------------
__fastcall TForm_main::TForm_main(TComponent* Owner)
	: TForm(Owner)
{
	for (int i = 0; i < 10; i++) target[i] = NULL; //для чего
}
//---------------------------------------------------------------------------
// своя функция обработки оконных сообщений
//---------------------------------------------------------------------------
void __fastcall TForm_main::A_WndProc(Messages::TMessage &Message)
{
	WndProc(Message);
}
//---------------------------------------------------------------------------
void __fastcall TForm_main::FormShow(TObject *Sender)
{
	Form_zastavka->ShowModal();

	// Cоздаем новую игру: если функция возвращает false,
	//                     то игроки отказались играть, надо выйти из игры
	if (CreateNewGame() == false)
	{
		Close();
	}
}
//---------------------------------------------------------------------------
// Нажатие на кнопку меню: "Выход из игры" или на "крестик" закрытия приложения
//---------------------------------------------------------------------------
void __fastcall TForm_main::N_closeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
// Подтверждение выхода из игры: спросим у игроков надо закончить игру или случайно нажали
//---------------------------------------------------------------------------
void __fastcall TForm_main::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// по умолчанию из игры не выходим
	CanClose = false;
	// окно вопроса нужно ли выходить из игры
	if (MessageBoxA(NULL, "Выйти из игры?", "Внимание", MB_YESNO + MB_ICONQUESTION) == IDNO)
		return;
	// разрешаем выйти из игры
	CanClose = true;
}
//---------------------------------------------------------------------------
// Нажатие на кнопку меню: "Об игре..."
//---------------------------------------------------------------------------
void __fastcall TForm_main::N_aboutClick(TObject *Sender)
{
	Form_about->ShowModal();
}
//---------------------------------------------------------------------------
// Нарисовать игровое поле
//---------------------------------------------------------------------------
void
TForm_main::PaintField()
{
	int 		x, xx, y, yy;
	TCanvas *	canvas;
	TRect		rect;

	canvas = Image_field->Canvas;

	Image_field->Left   = 0;
	Image_field->Top    = 0;

	// заполняем поле пустотой
	rect.init(0, 0, Image_field->Width, Image_field->Height);
	canvas->Brush->Color = clCream;//clBtnFace;
	canvas->Brush->Style = bsSolid;
	canvas->FillRect(rect);

	for (x = 0; x < field.x_count; x++)
	for (y = 0; y < field.y_count; y++)
	{
		if (field.items[x][y].card == NULL)
		{
			// заполняем поле пустотой
			rect.init(10 + x * CARD_SIZE + 1, 10 + y * CARD_SIZE + 1, 10 + x * CARD_SIZE + CARD_WIDTH, 10 + y * CARD_SIZE + CARD_WIDTH);
			canvas->Brush->Color = clCream;//clWhite;//clBtnFace;
			canvas->Brush->Style = bsSolid;
			canvas->FillRect(rect);

			canvas->Pen->Color = clGreen;
			canvas->Pen->Style = psDot;
			PaintRect(canvas, x, y);
		}
		else
		{
			field.items[x][y].card->move_to(10 + x * CARD_SIZE + 1, 10 + y * CARD_SIZE + 1, true);
		}
	}

}
//---------------------------------------------------------------------------
// отрисовка на экране одного игрового квадрата
//---------------------------------------------------------------------------
void
TForm_main::PaintRect(TCanvas * canvas, int X_field, int Y_field)
{
	int x_start, y_start;

	if ((X_field < 0) || (Y_field < 0))
		return;

	if ((X_field >= field.x_count) || (Y_field >= field.y_count))
		return;

	x_start = 10 + X_field * CARD_SIZE;
	y_start = 10 + Y_field * CARD_SIZE;

	canvas->MoveTo(x_start, y_start);
	canvas->LineTo(x_start + CARD_SIZE, y_start);
	canvas->LineTo(x_start + CARD_SIZE, y_start + CARD_SIZE);
	canvas->LineTo(x_start, y_start + CARD_SIZE);
	canvas->LineTo(x_start, y_start);
}
//---------------------------------------------------------------------------
// мышка заходит на игровое поле
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_fieldMouseEnter(TObject *Sender)
{
	X_field_0 = -1;
	Y_field_0 = -1;
	X_cur     = -1;
	Y_cur     = -1;
}
//---------------------------------------------------------------------------
// мышка покидает игровое поле
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_fieldMouseLeave(TObject *Sender)
{
	TCanvas *	canvas;

	canvas  = Image_field->Canvas;
	// восстановить старый квадрат
	canvas->Pen->Color = clGreen;
	canvas->Pen->Style = psDot;
	PaintRect(canvas, X_field_0, Y_field_0);

	X_field_0 = -1;
	Y_field_0 = -1;
	X_cur     = -1;
	Y_cur     = -1;
}
//---------------------------------------------------------------------------
// мышка перемещается по игровому полю. Текущий квадрат красный
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_fieldMouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y)
{
	TCanvas *	canvas;
	int 		x_field, y_field;

	// текущие координаты мыши на игровом поле
	X_cur = X;
	Y_cur = Y;

	x_field = (X - 10) / CARD_SIZE;
	y_field = (Y - 10) / CARD_SIZE;

	if (X < 10) x_field = -1;
	if (Y < 10) y_field = -1;

	if ((x_field != X_field_0) || (y_field != Y_field_0))
	{
		canvas  = Image_field->Canvas;
		// восстановить старый квадрат
		canvas->Pen->Color = clGreen;
		canvas->Pen->Style = psDot;
		PaintRect(canvas, X_field_0, Y_field_0);
		// нарисовать новый квадрат
		canvas->Pen->Color = clRed;
		canvas->Pen->Style = psDot;
		PaintRect(canvas, x_field, y_field);
		// новые -> старый
		X_field_0 = x_field;
		Y_field_0 = y_field;
	}
}
//---------------------------------------------------------------------------
// Нажатие на игровое поле, это позволяет нам класть карточки на игровое поле
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_fieldClick(TObject *Sender)
{
	int 		x_field, y_field;

	// если карточка уже положена, то ни чего делать не надо
	if (is_put_card == true)
		return;

	x_field = (X_cur - 10) / CARD_SIZE;
	y_field = (Y_cur - 10) / CARD_SIZE;

	if ((x_field < 0) || (y_field < 0))
	{
		MessageBoxA(NULL, "В данное место игрового поля положить карточку нельзя", "Внимание", MB_OK + MB_ICONERROR);
		return;
	}

	if ((x_field >= field.x_count) || (y_field >= field.y_count))
	{
		MessageBoxA(NULL, "В данное место игрового поля положить карточку нельзя", "Внимание", MB_OK + MB_ICONERROR);
		return;
	}

	if (field.items[x_field][y_field].card != NULL)
	{
		MessageBoxA(NULL, "Данное место на игровом поле уже занято", "Внимание", MB_OK + MB_ICONERROR);
		return;
	}

	if (field.can_set_card(x_field, y_field) == false)
	{
		MessageBoxA(NULL, "Ошибка. В данное место игрового поля положить карточку нельзя", "Внимание", MB_OK + MB_ICONERROR);
		return;
	}
	else
	{
		// положить карточку на игровое поле
		CardPut(x_field, y_field, field.card_cur);
	}
}
//---------------------------------------------------------------------------
// Нажатие на текущую карточку, это позволяет нам вращать карточку
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_curClick(TObject *Sender)
{
	// если карточку уже положили, то нельзя вращать "рубашку"
	if (is_put_card == true)
		return;
	// если кнопка 'Закончить ход' недоступна, то тоже выход
	if (Button_player_turn_end->Enabled == false)
        return;

	field.card_cur->rotate();
	Image_cur->Picture = field.card_cur->Image->Picture;
}
//---------------------------------------------------------------------------
// Начало хода игрока: отобразить имя текущего игрока и его игровую карточку
//---------------------------------------------------------------------------
bool
TForm_main::PlayerTurnStart()
{
	int 	i, index, count;

	// карту игрок еще не положил на поле
	is_put_card = false;

	// обновить очки игроков
	UpdatePlayersPanel();

	// 1) отобразать имя текущего игрока и поменять цвет надписи
	if (Form_players->players_cur != 0)
	{
		Form_players->players[Form_players->player_index].set_player_active_state(true);
	}

	// 2) посчитать сколько осталось карточек
	count = 0;
	for (i = 1; i<cards.cards_count; i++)
	{
		// проверяем не лежит ли карточка на игровом поле
		// поскольку все карты перемешаны, то индекс очередной карточки берем из массива cards.indexes
		index = cards.indexes[i];

		// проверяем не лежит ли карточка на игровом поле
		if (cards.card[index].on_field == false)
			count = count + 1;
	}

	if (count != 0)
		count = count - 1;
	Label_cards_free->Caption = count;

	// 2) просматриваем все карточки, и отображаем только первую не положенную на игровое поле
	for (i = 1; i < cards.cards_count; i++)
	{
		// поскольку все карты перемешаны, то индекс очередной карточки берем из массива cards.indexes
		index = cards.indexes[i];

		// проверяем не лежит ли карточка на игровом поле
		if (cards.card[index].on_field == false)
			break;
	}

	// если просмотрели все карточки и все лежат на игровом поле, то заканчиваем игру
	if (i == cards.cards_count)
	{
		return false;
	}

	// текущая карточка
	Image_cur->Picture = cards.card[index].Image->Picture;
	field.set_card_cur(&cards.card[index]);
	Label_desc_1->Enabled = true;
	Label_desc_2->Enabled = true;

	// отладочное
	Edit_number->Text = index;

	return true;
}
//---------------------------------------------------------------------------
// Конец хода игрока: проверить всё ли игрок сделал за ход +
//                    проверить какие фишки можно убрать и начислить очки +
//                    перейти к другому игроку и другой карточки
//---------------------------------------------------------------------------
void
TForm_main::PlayerTurnEnd()
{
	// проверить положил ли текущий игрок карточку?
	if (is_put_card == false)
	{
		MessageBoxA(NULL, "Чтобы перейти к следующему игроку, необходимо положить карточку на игровое поле", "Внимание", MB_OK + MB_ICONERROR);
		return;
	}

	// сделать панель текущего игрока неактивной
	Form_players->players[Form_players->player_index].set_player_active_state(false);

	// перейти к следующему игроку
	Form_players->player_index = Form_players->player_index + 1;

	// если что, то начинаем сначала списка игроков
	if (Form_players->player_index == Form_players->players_cur)
		Form_players->player_index = 0;

	// убираем цели для захвата
	HideTargets();

	// после окончания хода игрока, проверим
	// какие фишки можно убрать с игрового поля и начислить очки игрокам
	CheckAllTuples();

	// начать следующий ход
	if (PlayerTurnStart() == false)
	{
		FinishGame();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_main::Button_player_turn_endClick(TObject *Sender)
{
	PlayerTurnEnd();
}
//---------------------------------------------------------------------------
// Действия, когда положили игровую карточку на игровое поле
//---------------------------------------------------------------------------
void
TForm_main::CardPut(int x_field, int y_field, A_Card * card)
{
	// положить карточку на игровое поле
	field.set_card_on_field(x_field, y_field, card);

	// расширяем игровое поле
	if (x_field == 0)
		field.field_add_column_left();

	if (y_field == 0)
		field.field_add_row_top();

	if (x_field == field.x_count - 1)
		field.field_add_column_right();

	if (y_field == field.y_count - 1)
		field.field_add_row_bottom();

	// перерисовать игровое поле
	PaintField();

	// карточка положена на поле
	is_put_card = true;

	// теперь пользователь не должен видеть, что что-то можно положить
	// отобразим 0 карточку - рубашку
	Image_cur->Picture = cards.card[0].Image->Picture;

	Label_desc_1->Enabled = false;
	Label_desc_2->Enabled = false;

	// отображаем что можно захватить
	ShowTargets();
}
//---------------------------------------------------------------------------
// Найти координаты карточки на поле
//---------------------------------------------------------------------------
bool
TForm_main::CardGetXY(A_Card * card, int & x_field, int & y_field)
{
	int x, y;

	x_field = -1;
	y_field = -1;

	for (x = 0; x < field.x_count; x++)
	for (y = 0; y < field.y_count; y++)
	{
		if (field.items[x][y].card == card)
		{
			x_field = x;
			y_field = y;
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
// Действие после того как положили игровую карточку на игровое поле:
// отобразить цели, которые можно захватить
//---------------------------------------------------------------------------
void
TForm_main::ShowTargets()
{
	int 			i, index, x_field, y_field;
	A_Card *    	card;
	A_Chip *        chip;
	A_CardItem * 	card_item;

	card = field.card_cur;

	// смотрим есть ли возможность игроку выставить фишку на игровое поле
	chip = Form_players->players[Form_players->player_index].get_free_chip();
	if (chip == NULL)
	{
		return;
	}

	if (CardGetXY(card, x_field, y_field) == false)
	{
		MessageBoxA(NULL, "Не удалось найти игровую карточку", "Внимание", MB_OK);
		return;
	}

	// смотрим можно ли захварить какаю-то территорию
	index = 0;
	for (i = 0; i < 5; i++)
	{
		// элементы игровой карточки: 0,1,2,3 - стороны игровой карточки, 4 - рынок
		card_item = &card->card_items[i];
		if ((card_item->x != 0) && (card_item->y != 0))
		{
			// можно ли захватить этот игровой элемент?
			field.checkmap_zero();
			if (field.checkmap_check_at(x_field, y_field, i) == false)
			{
				continue;
			}

			// создаем новую цель для захвата
			target[index] = new A_Target();
			target[index]->show(card->Panel, card_item->x, card_item->y, Image_target_leave->Picture,
									Image_target_fieldMouseEnter,
									Image_target_fieldMouseLeave,
									Image_target_fieldClick);
			index = index + 1;
		}
	}


}
//---------------------------------------------------------------------------
// Действие: убрать все цели для захвата
//---------------------------------------------------------------------------
void    TForm_main::HideTargets()
{
	int     i;
	for (i = 0; i < 10; i++)
	{
		if (target[i] != NULL) delete target[i];
		target[i] = NULL;
	}
}
//---------------------------------------------------------------------------
// мышка попадает на цель для захвата территории
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_target_fieldMouseEnter(TObject *Sender)
{
	TImage * image;
	image = (TImage *)Sender;
	image->Picture = Image_target_enter->Picture;
}
//---------------------------------------------------------------------------
// мышка покидает цель для захвата территории
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_target_fieldMouseLeave(TObject *Sender)
{
	TImage * image;
	image = (TImage *)Sender;
	image->Picture = Image_target_leave->Picture;
}
//---------------------------------------------------------------------------
// мышкой нажимаем на цель для захвата территории
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_target_fieldClick(TObject *Sender)
{

	AnsiString      text;
	TImage * 		image;
	A_Card *    	card;
	A_Player *      player;
	A_Chip *        chip;
	TPicture *      picture;
	int         	i, x, y, player_index;

	// есть хоть один игрок (должен быть, но на всякий случай проверим)
	if (Form_players->players_cur == 0)
	{
		MessageBoxA(NULL, "Не заданы игроки для игры", "Внимание", MB_OK + MB_ICONERROR);
		return;
	}

	// сработало по нажатию на картинку
	image = (TImage *)Sender;

	// текущая игровая карточка, которую игрок положил на игровое поле
	card  = field.card_cur;

	// игрок, который положил карточку
	player = &Form_players->players[Form_players->player_index];

	// фишка, которую может положить игрок
	chip = player->get_free_chip();
	if (chip == NULL)
	{
		MessageBoxA(NULL, "Игрок не может положить фишку на игровое поле: закончились", "Внимание", MB_OK + MB_ICONERROR);
		return;
	}

	// определяем какой именно игровой элемент можно захватить
	// к координатам добавляем 8 (см. функцию show, там вычитаем 8, чтобы по середине картинки)
	x = image->Left + 8;
	y = image->Top + 8;

	text = "";
	// просматриваем куда именно мы положили и задаем пользователя, который это сделал
	for (i = 0; i < 5; i++)
	{
		if ((card->card_items[i].x == x) && (card->card_items[i].y == y))
		{
			card->card_items[i].UserID = player->UserID;
			break;
		}
	}

	// скрываем все цели (мы же уже захватили одну из них)
	HideTargets();

	// отображаение фишки
	switch(player->Color)
	{
	case clRed:    picture = Image_tuple_red->Picture; break;
	case clYellow: picture = Image_tuple_yellow->Picture; break;
	case clGreen:  picture = Image_tuple_green->Picture; break;
	case clBlue:   picture = Image_tuple_blue->Picture; break;
	case clSilver: picture = Image_tuple_silver->Picture; break;
	}

	// отобразим
	chip->show(card, x, y, 	picture,
							Image_tuple_MouseEnter,
							Image_tuple_MouseLeave,
							Image_tuple_Click);

}
//---------------------------------------------------------------------------
// Мышка над игровой фишкой
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_tuple_MouseEnter(TObject *Sender)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_tuple_MouseLeave(TObject *Sender)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_main::Image_tuple_Click(TObject *Sender)
{

}
//---------------------------------------------------------------------------
// создать панель с очками игроков
//---------------------------------------------------------------------------
void
TForm_main::CreatePlayersPanel()
{
	int 			i, count;
	A_Player *      player;

	// инициализируем графический интерфейс для каждого игрока
	// 0-ому игроку соотвествует 0 панель в верхней части экрана,
	// 1-ому - 1 панель и т.д.
	player = &Form_players->players[0];
	player->init_gui(Panel_user_0, Panel_user_vert_0, Panel_top_user_0, Label_user_bananas_count_0, Label_user_shells_count_0, Laber_user_makets_count_0, Label_user_points_0, Label_user_tuples_count_0, Image_user_tuple_0);
	player = &Form_players->players[1];
	player->init_gui(Panel_user_1, Panel_user_vert_1, Panel_top_user_1, Label_user_bananas_count_1, Label_user_shells_count_1, Laber_user_makets_count_1, Label_user_points_1, Label_user_tuples_count_1, Image_user_tuple_1);
	player = &Form_players->players[2];
	player->init_gui(Panel_user_2, Panel_user_vert_2, Panel_top_user_2, Label_user_bananas_count_2, Label_user_shells_count_2, Laber_user_makets_count_2, Label_user_points_2, Label_user_tuples_count_2, Image_user_tuple_2);
	player = &Form_players->players[3];
	player->init_gui(Panel_user_3, Panel_user_vert_3, Panel_top_user_3, Label_user_bananas_count_3, Label_user_shells_count_3, Laber_user_makets_count_3, Label_user_points_3, Label_user_tuples_count_3, Image_user_tuple_3);
	player = &Form_players->players[4];
	player->init_gui(Panel_user_4, Panel_user_vert_4, Panel_top_user_4, Label_user_bananas_count_4, Label_user_shells_count_4, Laber_user_makets_count_4, Label_user_points_4, Label_user_tuples_count_4, Image_user_tuple_4);

	// сначала все панели скрываем
	for (i = 0; i < 5; i++)
	{
		player = &Form_players->players[i];
		player->panel_vert->Visible = false;
		player->panel_main->Visible = false;
	}

	count = Form_players->players_cur;

	// задаем имя игрока на панеле, картинку с фишкой и делаем панель видимой
	for (i = count-1; i>=0; i--)
	{
		player = &Form_players->players[i];
		// загружаем фишку нужного цвета
		switch(player->Color)
		{
			case clRed: 	player->image_tuple->Picture = Image_tuple_red->Picture; break;
			case clYellow:  player->image_tuple->Picture = Image_tuple_yellow->Picture; break;
			case clGreen:   player->image_tuple->Picture = Image_tuple_green->Picture; break;
			case clBlue:    player->image_tuple->Picture = Image_tuple_blue->Picture; break;
			case clSilver:  player->image_tuple->Picture = Image_tuple_silver->Picture; break;
		}

		player->set_name();
		player->panel_main->Visible = true;
	}

	// отображаем вертикальные панели - перемычки между панелями с описанием игроков
	for (i = count-1; i>=0; i--)
	{
		player = &Form_players->players[i];
		player->panel_vert->Visible = true;
	}
}
//---------------------------------------------------------------------------
// обновить очки игроков
//---------------------------------------------------------------------------
void
TForm_main::UpdatePlayersPanel()
{
	A_Player *      player;
	int 			i, count;

    // общее число игроков в данной игровой сессии
	count = Form_players->players_cur;
	// теперь начинаем восстанавливать последовательность
	for (i = 0; i < count; i++)
	{
		player = &Form_players->players[i];
		player->update_gui(); // обновить значения характеристик
	}
}
//---------------------------------------------------------------------------
// Действие: после того как заканчиваем ход, проверяем
// может быть надо убрать какую-то фишку и начислить очков
//---------------------------------------------------------------------------
void
TForm_main::CheckAllTuples()
{
	int 		i, j, count, x, y;
	int         x_field, y_field, side;
	A_Player * 	player_cur;
	A_Chip   *  chip_cur;
	A_Card 	 *  card_cur;

	count = Form_players->players_cur;
	for (i = 0; i < count; i++)
	{
		// текущий просматриваемый   игрок
		player_cur = &Form_players->players[i];
		// у него есть CHIPS_MAX фишки (сейчас это 4), каждую из них проверяем
		for (j = 0; j < CHIPS_MAX; j++)
		{
			chip_cur = &player_cur->Chips[j];
			// если фишка не используется (т.е. не лежит на игровом поле), то
			// переходим к следующей
			if (chip_cur->IsUsed == false) continue;

			// карточка на которой лежит фишка и ее координаты на игровом поле
			card_cur = chip_cur->Card;
			CardGetXY(card_cur, x_field, y_field);

			// координаты этой фишки на карточке (+8 см. функцию A_Chip::show())
			x = chip_cur->Image->Left + 8;
			y = chip_cur->Image->Top + 8;

			// определяем сторону, которая захвачена этой фишкой
			for (side = 0; side < 5; side++)
			{
				if ((card_cur->card_items[side].x == x) && (card_cur->card_items[side].y == y))
				{
					card_cur->card_items[side].UserID = player_cur->UserID;
					break;
				}
			}

			// теперь вызываем функцию просмотреть весь машрут на захваченной территории
			field.checkmap_zero();
			field.checkmap_check_at(x_field, y_field, side);

			// если это дорога и найдены были 2 конца дороги, то она полностью
			if (card_cur->card_items[side].item_type == 1)
			{
				if (field.checkmap_roads_count == 2)
				{
					// начисляем очки (если они есть)
					player_cur->Seashells_count = player_cur->Seashells_count + field.checkmap_points;
					// убираем фишку с поля
					chip_cur->hide();
				}
			}

			// если это суша и она с каждой стороны имеет тоже сушу, то она закрытая
			if (card_cur->card_items[side].item_type == 2)
			{
				if (field.checkmap_land_is_close == true)
				{
					// начисляем очки (если они есть)
					player_cur->Bananas_count = player_cur->Bananas_count + field.checkmap_points;
					// убираем фишку с поля
					chip_cur->hide();
				}
			}

			// если это рынок и он со всех сторон окружен карточками, то он закрыт (захвачен) игроком
			if (card_cur->card_items[side].item_type == 3)
			{
				if (field.checkmap_market_is_close == true)
				{
					// начисляем очки (если они есть)
					player_cur->Maketplaces_count = player_cur->Maketplaces_count + 1;
					// убираем фишку с поля
					chip_cur->hide();
				}
			}
		}
	}

	// обновляем очки
	UpdatePlayersPanel();

}
//---------------------------------------------------------------------------
// начать новую игру
//---------------------------------------------------------------------------
bool
TForm_main::CreateNewGame()
{
	int     i, n;

	// 1) очищаем сведения о всех игроках (их всего макс. 5)
	n = 5;
	for (i=0;i<n;i++)
	{
		// в списке 0 активных игроков
		Form_players->VT_players->RootNodeCount = 0;
		// в списке 0 активных игроков
		Form_players->players_cur = 0;
		// сведения о каждом очищены
		Form_players->players[i].clear();
	}

	// 2) вызываем окно с вводом данных об игроках
	Form_players->ShowModal();
	// анализируем значение Form_players->is_ok:
	// т.е. is_ok = true  -> игрок нажал на кнопку 'Начать игру'
	//      is_ok = false -> игрок нажал на кнопку 'Отмена'
	if (Form_players->is_ok == false)
		return false;

	// 3) создаем панель с очками всех игроков
	CreatePlayersPanel();

	// 4) очищаем все игровые карточки (они могли быть повернуты как угодно) +
	//    первоначальная инициализация (загрузка картинок и разметка сторон карточек) +
	//    перемешиваем карточки
	cards.clear();
	cards.init(ScrollBox_field, ImageList_cards, 74, Image_target_enter, Image_target_leave);
	cards.shuffle_cards();

	// 5) очищаем игровое поле (на нем не должно быть игровых карточек)
	//    по координатам (1,1) установить карточку 29 (исторически сложилась,
	//    что она первая и одна и та же всегда
	field.clear();
	field.set_card_on_field(1, 1, &cards.card[29]);

	// 6) рисуем игровое поле
	PaintField();

	// 7) игра не закончилась
	is_game_finished = false;

	// 8) начинаем первый ход игрока, т.к. это первый ход,
	//    то контролировать есть ли еще карточки не надо
	PlayerTurnStart();

	// 9) теперь можно только закончить игру
	N_finish_start->Caption = "Закончить игру";

	// 10) доступна кнопка 'Закончить ход'
	Button_player_turn_end->Enabled = true;

	return true;
}
//---------------------------------------------------------------------------
// действия по окончанию игры
//---------------------------------------------------------------------------
void
TForm_main::FinishGame()
{
	int         player_points_best, player_points_cur, i, players_points_best_count;
	AnsiString  winners;

	// игра окончена

	is_game_finished = true;

	// выявление кто является победителем, сначала макс количество очков в игре

	players_points_best_count = 0;
	player_points_best = 0;
	for (i=0; i<Form_players->players_cur; i++)
	{
		player_points_cur = Form_players->players[i].calc_points();
		// такое же количество очков
		if (player_points_cur == player_points_best)
		{
			players_points_best_count = players_points_best_count + 1;
		}
		// если больше очков
		if (player_points_cur > player_points_best)
		{
			player_points_best = player_points_cur;
			players_points_best_count = 1;
		}
	}

	// если макс количество очков равно 0, то победителей нет
	if (player_points_best == 0)
	{
		MessageBoxA(NULL, "Игра окончена! Победителя нет!", "Внимание", MB_OK + MB_ICONINFORMATION);
	}
	else
	{
		// если макс количество очков набрал 1 игрок, то он победитель,
		// если несколько игроков, то они победители :)

		if (players_points_best_count == 1)
			winners = "Игра окончена!\r\nПобедитель:\r\n";
		else
			winners = "Игра окончена!\r\nПобедители:\r\n";

		players_points_best_count   = 0;
		for (i=0; i<Form_players->players_cur; i++)
		{
			if (Form_players->players[i].calc_points() == player_points_best)
			{
				// если уже одного победителя назвали, то надо поставить "запятую" в текст
				if (players_points_best_count != 0)
				{
					winners = winners + ",\r\n";
				}
				winners = winners + Form_players->players[i].Name;
				players_points_best_count = players_points_best_count + 1;
			}
		}

		MessageBoxA(NULL, winners.c_str(), "Внимание", MB_OK + MB_ICONINFORMATION);
	}

	// теперь можно только начать новую игру
	N_finish_start->Caption = "Начать новую игру";

	// недоступна кнопка 'Закончить ход'
	Button_player_turn_end->Enabled = false;


	return;
}
//---------------------------------------------------------------------------
void __fastcall TForm_main::N_finish_startClick(TObject *Sender)
{
	// если игра еще не окончина, надо окончить ее или продолжить играть
	if (is_game_finished == false)
	{
		// Вопрос надо ли закончить игру, если она в самом разгаре?
		if (MessageBoxA(NULL, "Закончить игру?", "Внимание", MB_YESNO + MB_ICONQUESTION) == IDNO)
			return ;

		// действия по окнчанию игры
		FinishGame();
			return;
	}

	// Начать новую игру или выйти
	CreateNewGame();
}
//---------------------------------------------------------------------------
void __fastcall TForm_main::N_rulesClick(TObject *Sender)
{
	AnsiString  path;
	char 		buffer[MAX_PATH];

	// получить полный путь к запущенному exe-шнику
	// путь состоит из: <буква диска>:\<rfnfkjub>\Carcassonne-game.exe
	// например, d:\games\Carcassonne-game.exe
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	path = buffer;

	// найдем послений слеш (он может быть прямым или обратным) и обрежем строку до вида:
	// должно быть: d:\games\

	path = path.SubString(0, path.LastDelimiter("\\/"));

	// добавим к каталогу название файла с правилами игры:
	// d:\games\Правила игры Каркассон.docx
	path = path + "Правила игры Каркассон.docx";

	// запустим внешнюю программу, которая открывает docx файлы:
	ShellExecuteA(Form_main->Handle, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------



