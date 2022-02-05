#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"

#include <deque>

class CSnake : public CFramedWindow
{
public:
	CSnake(CRect r, char _c = ' ');

	void paint();
	bool handleEvent(int c);

private:
	void init_game();
	void restart_game();
	void generate_food();
	void move_snake();
	void print_snake();
	void print_food();

private:
	std::deque<CPoint> m_SnakeSegmets;
	int m_Level;
	int m_Direction;
	bool m_ChangedDirection;

	CPoint m_Food;

	int m_Ticks;

	bool m_Started;
	bool m_Paused;
	bool m_Help;
	bool m_Finished;
};

#endif
