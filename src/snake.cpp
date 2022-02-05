#include "snake.h"

#include <cstdlib>
#include <ctime>

#include <ncurses.h>

CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c)
{
	srand((unsigned int)time(NULL));

	init_game();
}

void CSnake::paint()
{
	CFramedWindow::paint();

	if (m_Started == false)
	{
		mvprintw(geom.topleft.y + 2, geom.topleft.x + 3, "h - help");
		mvprintw(geom.topleft.y + 3, geom.topleft.x + 3, "p - pause");
		mvprintw(geom.topleft.y + 4, geom.topleft.x + 3, "r - restart");
		mvprintw(geom.topleft.y + 5, geom.topleft.x + 3, "w, a, s, d - move");

		mvprintw(geom.topleft.y + 8, geom.topleft.x + 3, "spacebar - start game");
	}
	else
	{
		if (m_Help == true)
		{
			mvprintw(geom.topleft.y, geom.topleft.x + 1, " LEVEL %d +", m_Level);

			mvprintw(geom.topleft.y + 2, geom.topleft.x + 3, "h - help");
			mvprintw(geom.topleft.y + 3, geom.topleft.x + 3, "p - pause");
			mvprintw(geom.topleft.y + 4, geom.topleft.x + 3, "r - restart");
			mvprintw(geom.topleft.y + 5, geom.topleft.x + 3, "w, a, s, d - move");
		}
		else if (m_Paused == true)
		{
			mvprintw(geom.topleft.y, geom.topleft.x + 1, " LEVEL %d +", m_Level);

			mvprintw(geom.topleft.y + geom.size.y / 2 - 1, geom.topleft.x + 3, "game paused");
		}
		else if (m_Finished == true)
		{
			mvprintw(geom.topleft.y, geom.topleft.x + 1, " GAME OVER + LEVEL %d +", m_Level);

			print_snake();
			print_food();
		}
		else
		{
			mvprintw(geom.topleft.y, geom.topleft.x + 1, " LEVEL %d +", m_Level);

			print_snake();
			print_food();
		}
	}
}

bool CSnake::handleEvent(int c)
{
	bool handled = false;

	if (m_Paused == false && c == ERR)
	{
		m_Ticks++;

		handled = true;
	}

	if (m_Started == false)
	{
		if (c == ' ')
		{
			m_Started = true;

			handled = true;
		}
	}
	else
	{
		if (m_Help == true)
		{
			if (c == 'h' || c == 'H')
			{
				m_Help = false;
				m_Paused = false;

				handled = true;
			}
		}
		else if (m_Paused == true)
		{
			if (c == 'p' || c == 'P')
			{
				m_Paused = false;

				handled = true;
			}
		}
		else if (m_Finished == true)
		{
			if (c == ' ')
			{
				restart_game();

				handled = true;
			}
		}
		else
		{
			switch (c)
			{
			case 'w':
			case 'W':
				if (m_Direction != 's' && m_Direction != 'S' && m_ChangedDirection == false)
				{
					m_Direction = 'w';
					m_ChangedDirection = true;
				}

				m_Ticks++;
				handled = true;
				break;
			case 'a':
			case 'A':
				if (m_Direction != 'd' && m_Direction != 'D' && m_ChangedDirection == false)
				{
					m_Direction = 'a';
					m_ChangedDirection = true;
				}

				m_Ticks++;
				handled = true;
				break;
			case 's':
			case 'S':
				if (m_Direction != 'w' && m_Direction != 'W' && m_ChangedDirection == false)
				{
					m_Direction = 's';
					m_ChangedDirection = true;
				}

				m_Ticks++;
				handled = true;
				break;
			case 'd':
			case 'D':
				if (m_Direction != 'a' && m_Direction != 'A' && m_ChangedDirection == false)
				{
					m_Direction = 'd';
					m_ChangedDirection = true;
				}

				m_Ticks++;
				handled = true;
				break;
			case 'h':
			case 'H':
				m_Help = true;
				m_Paused = true;

				handled = true;
				break;
			case 'p':
			case 'P':
				m_Paused = true;

				handled = true;
				break;
			case 'r':
			case 'R':
				restart_game();

				handled = true;
				break;
			}
		}
	}

	if (handled == false)
	{
		if (m_Paused == false)
		{
			m_Ticks++;
		}

		handled = CFramedWindow::handleEvent(c);
	}

	int neededTicksToMove = m_Level < 7 ? 25 - m_Level * 2 : 25 - 7 * 2 - (m_Level - 6);

	if (m_Ticks == (neededTicksToMove >= 5 ? neededTicksToMove : 5))
	{
		move_snake();
		m_Ticks = 0;
		m_ChangedDirection = false;
	}

	return handled;
}

void CSnake::init_game()
{
	m_Started = false;
	m_Paused = false;
	m_Help = false;
	m_Finished = false;

	m_SnakeSegmets.push_back(CPoint(geom.size.x / 2 - 1, geom.size.y / 2 - 1));
	m_Level = 1;
	m_Direction = 'd';
	m_ChangedDirection = false;

	m_Ticks = 0;

	generate_food();
}

void CSnake::restart_game()
{
	m_SnakeSegmets.clear();

	init_game();
}

void CSnake::generate_food()
{
	while (1)
	{
		bool occupied = false;

		int x = rand() % (geom.size.x - 2);
		int y = rand() % (geom.size.y - 2);

		for (auto &segment : m_SnakeSegmets)
		{
			if (segment.x == x && segment.y == y)
			{
				occupied = true;

				break;
			}
		}

		if (occupied == false)
		{
			m_Food.x = x;
			m_Food.y = y;

			break;
		}
	}
}

void CSnake::move_snake()
{
	CPoint new_head = m_SnakeSegmets.front();

	switch (m_Direction)
	{
	case 'w':
		new_head.y--;

		if (new_head.y < 0)
		{
			new_head.y = geom.size.y - 3;
		}
		break;
	case 'a':
		new_head.x--;

		if (new_head.x < 0)
		{
			new_head.x = geom.size.x - 3;
		}
		break;
	case 's':
		new_head.y++;

		if (new_head.y > geom.size.y - 3)
		{
			new_head.y = 0;
		}
		break;
	case 'd':
		new_head.x++;

		if (new_head.x > geom.size.x - 3)
		{
			new_head.x = 0;
		}
		break;
	}

	if (new_head.x == m_Food.x && new_head.y == m_Food.y)
	{
		m_SnakeSegmets.push_front(new_head);
		m_Level++;

		generate_food();
	}
	else
	{
		bool on_segment = false;

		for (auto &segment : m_SnakeSegmets)
		{
			if (new_head.x == segment.x && new_head.y == segment.y)
			{
				on_segment = true;

				break;
			}
		}

		if (on_segment == true)
		{
			m_Finished = true;
		}
		else
		{
			m_SnakeSegmets.push_front(new_head);
			m_SnakeSegmets.pop_back();
		}
	}
}

void CSnake::print_snake()
{
	bool printed_head = false;

	for (auto &segment : m_SnakeSegmets)
	{
		if (printed_head == false)
		{
			mvprintw(geom.topleft.y + segment.y + 1, geom.topleft.x + segment.x + 1, "@");

			printed_head = true;
		}
		else
		{
			mvprintw(geom.topleft.y + segment.y + 1, geom.topleft.x + segment.x + 1, "X");
		}
	}
}

void CSnake::print_food()
{
	mvprintw(geom.topleft.y + m_Food.y + 1, geom.topleft.x + m_Food.x + 1, "#");
}