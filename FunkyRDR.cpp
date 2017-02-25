#include "FunkyRDR.h"
#include <fstream>
#include <sstream>
#include <string>

const std::string CMD_POSITION = "p";
const std::string CMD_TYPEWRITER = "t";
const std::string CMD_WAIT = "w";
const std::string CMD_TYPEWRITER_DELAYED = "td";
const std::string CMD_CENTER = "c";
const std::string CMD_TYPEWRITER_CENTER = "tc";
const std::string CMD_INPUT = "i";
const std::string CMD_FILL = "f";
const std::string CMD_FLASH_FILL = "ff";
const std::string CMD_CLEAR = "clr";


void FunkyRDR::Init() {
	initscr();

	curs_set(0);
	getmaxyx(stdscr, row, col);
	width = 34;
	height = 5;
	
	tokens["SCREEN_ROWS"] = std::to_string(row);
	tokens["SCREEN_COLS"] = std::to_string(col);

}

void FunkyRDR::ReadScript(const char * script)
{
	std::string line;
	char lineBuffer[255];
	char inputBuffer[255];


	clear();
	move(0, 0);

	std::ifstream gameData(script);
	if (!gameData) {
		sprintf_s(lineBuffer, "Couldn't open script: %s\nPress any key to exit.\n", script);
		CurseHelper::typeWriter(lineBuffer);
		getch();
		gameData.close();
		return;
	}


	while (std::getline(gameData, line))
	{
		std::string chrCmdSep = ":";
		std::string chrCmdTerm = ";";
		std::string chrParmSep = ",";

		size_t tpos = 0;		// terminate position
		size_t cspos = 0;		// command separator position
		size_t pspos = 0;		// parameter separator position

		std::string command;	// One command with parameters
		std::string cmd;		// A single command only

		while ((tpos = line.find(chrCmdTerm)) != std::string::npos) {
			command = line.substr(0, tpos);

			if ((cspos = command.find(chrCmdSep)) != std::string::npos) {
				cmd = command.substr(0, cspos);
				command.erase(0, cspos + chrCmdSep.length());

				// p - Sets the position of the screen cursor.
				if (cmd == CMD_POSITION) {
					std::string filteredCMD = ReplaceTokens(command);
					if ((pspos = filteredCMD.find(chrParmSep)) != std::string::npos) {
						
						std::string sx = filteredCMD.substr(0, pspos);
						std::string sy = filteredCMD.substr(pspos + 1, filteredCMD.length());

						int x = std::stoi(sx, nullptr, 10);
						int y = std::stoi(sy, nullptr, 10);

						if (x == row) { x--; }

						move(x, y);
						line.erase(0, tpos + chrCmdTerm.length());
						continue;
					}
					else {
						CurseHelper::typeWriter("Command 'p' requires 2 arguments.");
						return;
					}
				}

				// w - Sleeps for n miliseconds.
				if (cmd == CMD_WAIT) {
					int wait = std::stoi(command, nullptr, 10);
					Sleep(wait);
					line.erase(0, tpos + chrCmdTerm.length());
					continue;
				}

				// t - Writes text to screen at throttled rate. 
				if (cmd == CMD_TYPEWRITER) {

					std::string filteredCMD = ReplaceTokens(command);
					sprintf_s(lineBuffer, filteredCMD.c_str());
					CurseHelper::typeWriter(lineBuffer);
					line.erase(0, tpos + chrCmdTerm.length());
					continue;
				}

				// td - Writes text to screen at user defined throttled rate.
				if (cmd == CMD_TYPEWRITER_DELAYED) {
					if ((pspos = command.find(chrParmSep)) != std::string::npos) {
						std::string filteredCMD = ReplaceTokens(command);
						std::string strdelay = filteredCMD.substr(0, pspos);
						std::string text = filteredCMD.substr(pspos + 1, command.length());
						//std::string filteredCMD = ReplaceTokens(command);

						int delay = std::stoi(strdelay, nullptr, 10);
						CurseHelper::typeWriter(text.c_str(), delay);
						line.erase(0, tpos + chrCmdTerm.length());
						continue;
					}
					else {
						CurseHelper::typeWriter("Command 'td' requires 2 arugments.");
						return;
					}
				}

				// c - Clears the screen then writes text to center screen.
				if (cmd == CMD_CENTER) {
					clear();
					std::string filteredCMD = ReplaceTokens(command);
					sprintf_s(lineBuffer, filteredCMD.c_str());
					mvprintw(row / 2, (col - strlen(lineBuffer)) / 2, "%s", filteredCMD.c_str());
					refresh();
					line.erase(0, tpos + chrCmdTerm.length());
					continue;
				}

				// tc - Clears the screen then writes text to the center screen at a throttled rate.
				if (cmd == CMD_TYPEWRITER_CENTER) {
					clear();
					std::string filteredCMD = ReplaceTokens(command);
					sprintf_s(lineBuffer, filteredCMD.c_str());
					SetPosition(row / 2, (col - strlen(lineBuffer)) / 2);
					CurseHelper::typeWriter(lineBuffer);
					line.erase(0, tpos + chrCmdTerm.length());
					continue;
				}

				// i - Get's input from the user, stores it in a map referenced by a user 
				// defined token.
				if (cmd == CMD_INPUT) {
					getstr(inputBuffer);
					tokens[command] = inputBuffer;
					line.erase(0, tpos + chrCmdTerm.length());
					continue;
				}

				// f - Fills the screen buffer with a single char.
				if (cmd == CMD_FILL) {
					CurseHelper::fillScreen(command[0], row, col);
					refresh();
					line.erase(0, tpos + chrCmdTerm.length());
					continue;
				}

				// ff - Flash fills the screen with an array of chars at a user defined rate.
				if (cmd == CMD_FLASH_FILL) {
					if ((pspos = command.find(chrParmSep)) != std::string::npos) {
						std::string sdelay = command.substr(0, pspos);
						std::string chars = command.substr(pspos + 1, command.length());

						int delay = std::stoi(sdelay, nullptr, 10);
						CurseHelper::animateFlashFill(chars, delay, false, row, col);
					}
					line.erase(0, tpos + chrCmdTerm.length());
					continue;
				}
			}
			else {
				// clr - Clears the screen
				if (command == CMD_CLEAR) {
					clear();
					refresh();
					line.erase(0, tpos + chrCmdTerm.length());
					continue;
				}
			}

			line.erase(0, tpos + chrCmdTerm.length());
		}

		refresh();
	}
	gameData.close();
	getch();
}

std::string FunkyRDR::ReplaceTokens(std::string line) {
	size_t start;
	size_t end;
	std::string tmp;
	std::string result;
	std::string index;
	std::string token;

	if ((start = line.find('[')) != std::string::npos) {
		tmp = line.substr(start + 1, line.length());
		if ((end = tmp.find(']')) != std::string::npos) {
			index = line.substr(start + 1, end);
			token = line.substr(start, end + 2);

			line.replace(line.find(token), token.length(), tokens[index]);
			return ReplaceTokens(line);
		}
	}
	return line;
}

void FunkyRDR::SetPosition(int x, int y) {
	move(x, y);
}