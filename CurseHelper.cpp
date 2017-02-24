#include "CurseHelper.h"
#include <string>


void CurseHelper::typeWriter(char * s)
{
	for (int i = 0; i < strlen(s); i++)
	{
		Sleep(25);
		printw("%c", s[i]);
		refresh();
	}
}

void CurseHelper::typeWriter(const char * s, int delay)
{
	for (int i = 0; i < strlen(s); i++)
	{
		Sleep(delay);
		printw("%c", s[i]);
		refresh();
	}
}

void CurseHelper::fillScreen(char c, int row, int col)
{
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			mvprintw(y, x, "%c", c);
		}
	}
}

void CurseHelper::drawBorder(char c, int row, int col)
{
	for (int y = 0; y < row; y++) {
		if (y == 0 || y == (row - 1)) {
			for (int x = 0; x < col; x++) {
				if (y == 0 || y == (row - 1) || x == 0 || x == (col - 1)) {
					mvprintw(y, x, "%c", c);
				}
			}
		}
		else {
			mvprintw(y, 0, "%c", c);
			mvprintw(y, col - 1, "%c", c);
		}
	}
}

void CurseHelper::drawFillRect(int x, int y, int width, int height, char fill)
{
	for (int r = y; r < y + height; r++) {
		for (int c = x; c < x + width; c++) {
			mvprintw(r, c, "%c", fill);
		}
	}
}

void CurseHelper::drawRect(int x, int y, int width, int height, char border)
{
	for (int r = y; r < y + height; r++) {	// r == row
		if (r == y || r == (y + height - 1)) {
			for (int c = x; c < x + width; c++) {	// c == col
				if (c == x || r == y || c == x + width - 1 || r == y + height - 1) {
					mvprintw(r, c, "%c", border);
				}
			}
		}
		else {
			mvprintw(r, x, "%c", border);
			mvprintw(r, x + width - 1, "%c", border);
		}
	}
}

void CurseHelper::drawBG(int row, int col)
{
	clear();
	drawBorder('#', row, col);
}

void CurseHelper::animateFlashFill(const std::string & chars, int delay, bool seconds, int row, int col)
{
	for (int i = 0; i < chars.size(); ++i)
	{
		fillScreen(chars[i], row, col);
		refresh();

		if (seconds)
			Sleep(delay * 1000);
		else
			Sleep(delay);
	}
}



