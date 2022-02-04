#include "snake.h"

#include <ncurses.h>

CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c)
{
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
	if (CFramedWindow::handleEvent(c) == true)
	{
		return true;
	}

	if (m_Started == false)
	{
		if (c == ' ')
		{
			m_Started = true;

			return true;
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

				return true;
			}
		}
		else if (m_Paused == true)
		{
			if (c == 'p' || c == 'P')
			{
				m_Paused = false;

				return true;
			}
		}
		else if (m_Finished == true)
		{
			if (c == ' ')
			{
				restart_game();

				return true;
			}
		}
		else
		{
			switch (c)
			{
			case 'w':
			case 'W':
				if (m_Direcrion != 's' && m_Direcrion != 'S')
				{
					m_Direcrion = c;
				}
				return true;
			case 'a':
			case 'A':
				if (m_Direcrion != 'd' && m_Direcrion != 'D')
				{
					m_Direcrion = c;
				}
				return true;
			case 's':
			case 'S':
				if (m_Direcrion != 'w' && m_Direcrion != 'W')
				{
					m_Direcrion = c;
				}
				return true;
			case 'd':
			case 'D':
				if (m_Direcrion != 'a' && m_Direcrion != 'A')
				{
					m_Direcrion = c;
				}
				return true;
			case 'h':
			case 'H':
				m_Help = true;
				m_Paused = true;
				return true;
			case 'p':
			case 'P':
				m_Paused = true;
				return true;
			case 'r':
			case 'R':
				restart_game();
				return true;
			}
		}
	}

	return false;
}

void CSnake::init_game()
{
}

void CSnake::restart_game()
{
}

void CSnake::next_point()
{
}

void CSnake::move_snake()
{
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