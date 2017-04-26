#pragma once
#include "curses.h"
#include <string.h>
#include <string>
#include <windows.system.h>
#include <time.h>
#include <algorithm>
#include "CurseHelper.h"
#include <map>

// FunkyRDR ...
class FunkyRDR {
public:
	char * playerName;
	int row, col, width, height;
	std::map<std::string, std::string> tokens;

	void Init();
	//void Intro();
	//void Start();
	//void SetPlayerName(char * name);

	void ReadScript(const char * script);



private:
	char inputBuffer[255];

	std::string ReplaceTokens(std::string);
	void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
	void SetPosition(int x, int y);
	void Wait(int n);
	void Type(std::string s);
	void Type(int d, std::string s);
	void PrintCenter(std::string s);
};