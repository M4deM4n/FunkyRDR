#pragma once
#include "curses.h"
#include <string.h>
#include <string>
#include <windows.system.h>

class CurseHelper {
public:
	static void typeWriter(char * s);
	static void typeWriter(const char * s, int delay);
	static void fillScreen(char c, int row, int col);
	static void drawBorder(char c, int row, int col);
	static void drawFillRect(int x, int y, int width, int height, char fill);
	static void drawRect(int x, int y, int width, int height, char border);
	static void drawBG(int row, int col);
	static void animateFlashFill(const std::string& chars, int delay, bool seconds, int row, int col);
private:
	CurseHelper();
};