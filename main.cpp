#include "include\curses.h"
#include <string.h>
#include <Windows.h>
#include <windows.system.h>
#include <time.h>
#include <algorithm>

#include "FunkyRDR.h"

int row, col;

int main(int argc, char *argv[])
{
	char * script = "funky.rdr";
	if (argc > 1) {
		script = argv[1];
	}

	FunkyRDR funk = FunkyRDR();
	funk.Init();
	funk.ReadScript(script);
	endwin();

	return 0;
}

