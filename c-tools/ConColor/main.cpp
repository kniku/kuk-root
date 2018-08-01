#include <stdlib.h>
#include "Console.h"

#define BUFFER_SIZE		10000
#define MODE_DEFAULT	0
#define MODE_SETCOLOR	1
#define MODE_PRINT		2
#define MODE_PRINTLN	3
#define MODE_SETCOLOR_PUSH	4
#define MODE_SETCOLOR_POP	5

//#define _DEBUG 

char * getColorName(WORD iColor) {
	char *r = 0;
	switch (iColor) {
	case JadedHoboConsole::fgLoRed:
	case JadedHoboConsole::bgLoRed:
		r = "darkred";
		break;
	case JadedHoboConsole::fgLoGreen:
	case JadedHoboConsole::bgLoGreen:
		r = "darkgreen";
		break;
	case JadedHoboConsole::fgLoBlue:
	case JadedHoboConsole::bgLoBlue:
		r = "darkblue";
		break;
	case JadedHoboConsole::fgLoCyan:
	case JadedHoboConsole::bgLoCyan:
		r = "darkcyan";
		break;
	case JadedHoboConsole::fgLoMagenta:
	case JadedHoboConsole::bgLoMagenta:
		r = "darkmagenta";
		break;
	case JadedHoboConsole::fgLoYellow:
	case JadedHoboConsole::bgLoYellow:
		r = "darkyellow";
		break;
	case JadedHoboConsole::fgLoWhite:
	case JadedHoboConsole::bgLoWhite:
		r = "darkwhite";
		break;
	case JadedHoboConsole::fgGray:
	case JadedHoboConsole::bgGray:
		r = "gray";
		break;
	case JadedHoboConsole::fgHiWhite:
	case JadedHoboConsole::bgHiWhite:
		r = "white";
		break;
	case JadedHoboConsole::fgHiBlue:
	case JadedHoboConsole::bgHiBlue:
		r = "blue";
		break;
	case JadedHoboConsole::fgHiGreen:
	case JadedHoboConsole::bgHiGreen:
		r = "green";
		break;
	case JadedHoboConsole::fgHiRed:
	case JadedHoboConsole::bgHiRed:
		r = "red";
		break;
	case JadedHoboConsole::fgHiCyan:
	case JadedHoboConsole::bgHiCyan:
		r = "cyan";
		break;
	case JadedHoboConsole::fgHiMagenta:
	case JadedHoboConsole::bgHiMagenta:
		r = "magenta";
		break;
	case JadedHoboConsole::fgHiYellow:
	case JadedHoboConsole::bgHiYellow:
		r = "yellow";
		break;
	case JadedHoboConsole::fgBlack:
		//case JadedHoboConsole::bgBlack:
	default:
		r = "black";
		break;
	}
	return r;
}

void set_colors(char *color, char isTextColor)
{
	if (strcmp(color, "red") == 0) {
		isTextColor ? JadedHoboConsole::fg_red(std::cout) : JadedHoboConsole::bg_red(std::cout);
	}
	else if (strcmp(color, "darkred") == 0) {
		isTextColor ? JadedHoboConsole::fg_darkred(std::cout) : JadedHoboConsole::bg_darkred(std::cout);
	}
	else if (strcmp(color, "green") == 0) {
		isTextColor ? JadedHoboConsole::fg_green(std::cout) : JadedHoboConsole::bg_green(std::cout);
	}
	else if (strcmp(color, "darkgreen") == 0) {
		isTextColor ? JadedHoboConsole::fg_darkgreen(std::cout) : JadedHoboConsole::bg_darkgreen(std::cout);
	}
	else if (strcmp(color, "blue") == 0) {
		isTextColor ? JadedHoboConsole::fg_blue(std::cout) : JadedHoboConsole::bg_blue(std::cout);
	}
	else if (strcmp(color, "darkblue") == 0) {
		isTextColor ? JadedHoboConsole::fg_darkblue(std::cout) : JadedHoboConsole::bg_darkblue(std::cout);
	}
	else if (strcmp(color, "white") == 0) {
		isTextColor ? JadedHoboConsole::fg_white(std::cout) : JadedHoboConsole::bg_white(std::cout);
	}
	else if (strcmp(color, "darkwhite") == 0) {
		isTextColor ? JadedHoboConsole::fg_darkwhite(std::cout) : JadedHoboConsole::bg_darkwhite(std::cout);
	}
	else if (strcmp(color, "cyan") == 0) {
		isTextColor ? JadedHoboConsole::fg_cyan(std::cout) : JadedHoboConsole::bg_cyan(std::cout);
	}
	else if (strcmp(color, "darkcyan") == 0) {
		isTextColor ? JadedHoboConsole::fg_darkcyan(std::cout) : JadedHoboConsole::bg_darkcyan(std::cout);
	}
	else if (strcmp(color, "magenta") == 0) {
		isTextColor ? JadedHoboConsole::fg_magenta(std::cout) : JadedHoboConsole::bg_magenta(std::cout);
	}
	else if (strcmp(color, "darkmagenta") == 0) {
		isTextColor ? JadedHoboConsole::fg_darkmagenta(std::cout) : JadedHoboConsole::bg_darkmagenta(std::cout);
	}
	else if (strcmp(color, "yellow") == 0) {
		isTextColor ? JadedHoboConsole::fg_yellow(std::cout) : JadedHoboConsole::bg_yellow(std::cout);
	}
	else if (strcmp(color, "darkyellow") == 0) {
		isTextColor ? JadedHoboConsole::fg_darkyellow(std::cout) : JadedHoboConsole::bg_darkyellow(std::cout);
	}
	else if (strcmp(color, "black") == 0) {
		isTextColor ? JadedHoboConsole::fg_black(std::cout) : JadedHoboConsole::bg_black(std::cout);
	}
	else if (strcmp(color, "gray") == 0) {
		isTextColor ? JadedHoboConsole::fg_gray(std::cout) : JadedHoboConsole::bg_gray(std::cout);
	}
	else {
		//JadedHoboConsole::fg_white(std::cout);
	}
}

int main(int argc, char **argv) {
	int mode = MODE_DEFAULT;
	int param_idx_color = 1;

	char xx[BUFFER_SIZE + 1];
	char color[100] = "";
	char bgcolor[100] = "";
	LONG err;
	HKEY hkey = 0;
	BYTE bytearr[200] = { 0 };

	JadedHoboConsole::con_dev x;

	if (argc == 1 || strcmp(argv[1], "-h") == 0) {
		JadedHoboConsole::fg_green(std::cout);
		std::cout << "\n=== " << argv[0] << ", Version 1.1, (C) kuk, 2005-2008 ===\n\n";
		std::cout << "usage: " << argv[0] << " [-h|-set|-push|-pop|-print[ln] \"text\"] [fgcolor] [bgcolor]\n\n";
		JadedHoboConsole::fg_white(std::cout);
		std::cout << "-set ...... just set the color(s) for active console\n";
		std::cout << "-push ..... like -set, but saves current state first\n";
		std::cout << "-pop ...... restores saved state (of last push)\n";
		std::cout << "-print .... print text direct instead reading from stdin\n";
		std::cout << "-println .. like -print, followed by \\n\n";
		std::cout << "-h ........ this help (ask kuk)\n\n";
		x.RestoreAttributes();
		std::cout << "possible colors: ";
		JadedHoboConsole::fg_darkred(std::cout);
		std::cout << "(dark)";
		JadedHoboConsole::fg_red(std::cout);
		std::cout << "red,";
		JadedHoboConsole::fg_darkgreen(std::cout);
		std::cout << " (dark)";
		JadedHoboConsole::fg_green(std::cout);
		std::cout << "green,";
		JadedHoboConsole::fg_darkblue(std::cout);
		std::cout << " (dark)";
		JadedHoboConsole::fg_blue(std::cout);
		std::cout << "blue,";
		JadedHoboConsole::fg_darkwhite(std::cout);
		std::cout << " (dark)";
		JadedHoboConsole::fg_white(std::cout);
		std::cout << "white,";
		JadedHoboConsole::fg_darkcyan(std::cout);
		std::cout << " (dark)";
		JadedHoboConsole::fg_cyan(std::cout);
		std::cout << "cyan,";
		JadedHoboConsole::fg_darkmagenta(std::cout);
		std::cout << " (dark)";
		JadedHoboConsole::fg_magenta(std::cout);
		std::cout << "magenta,";
		JadedHoboConsole::fg_darkyellow(std::cout);
		std::cout << " (dark)";
		JadedHoboConsole::fg_yellow(std::cout);
		std::cout << "yellow,";
		JadedHoboConsole::fg_gray(std::cout);
		std::cout << " gray";
		x.RestoreAttributes();
		std::cout << " and ";
		JadedHoboConsole::fg_black(std::cout);
		JadedHoboConsole::bg_gray(std::cout);
		std::cout << "black";
		x.RestoreAttributes();
		std::cout << "\n";
		return 0;
	}
	if (argc > 1 && strcmp(argv[1], "-set") == 0) {
		mode = MODE_SETCOLOR;
		param_idx_color = 2;
	}
	if (argc > 1 && strcmp(argv[1], "-push") == 0) {
		mode = MODE_SETCOLOR_PUSH;
		param_idx_color = 2;
	}
	if (argc > 1 && strcmp(argv[1], "-pop") == 0) {
		mode = MODE_SETCOLOR_POP;
		param_idx_color = 999;
	}
	if (argc > 2 && strcmp(argv[1], "-print") == 0) {
		mode = MODE_PRINT;
		strcpy(xx, argv[2]);
		param_idx_color = 3;
	}
	if (argc > 2 && strcmp(argv[1], "-println") == 0) {
		mode = MODE_PRINTLN;
		strcpy(xx, argv[2]);
		param_idx_color = 3;
	}

	WORD oc_bg, oc_fg;
	DWORD lType = 0;
	DWORD lSize = 200;
	switch (mode) {
	case MODE_SETCOLOR_PUSH:
		oc_bg = x.GetBgColor_original();
		oc_fg = x.GetFgColor_original();
		sprintf((char *)bytearr, "%5.5d;%5.5d", oc_fg, oc_bg);
#ifdef _DEBUG
		printf("PUSH : %s;%s\n", getColorName(oc_fg), getColorName(oc_bg));
#endif
		err = RegOpenCurrentUser(KEY_SET_VALUE, &hkey);
		if (err == ERROR_SUCCESS) {
			err = RegSetValueEx(hkey, "kukconcolor", 0, REG_SZ, bytearr, (DWORD)strlen((char *)bytearr) + 1);
		}
		RegCloseKey(hkey);
		if (err != ERROR_SUCCESS) {
#ifdef _DEBUG
			printf("-push: can't open regkey.\n");
#endif
			exit(1);
		}
		mode = MODE_SETCOLOR;
		break;
	case MODE_SETCOLOR_POP:
		err = RegOpenCurrentUser(KEY_QUERY_VALUE, &hkey);
#ifdef _DEBUG
		if (err != ERROR_SUCCESS) {
			printf("-pop: can't open regkey.\n");
		}
#endif
		if (err == ERROR_SUCCESS) {
			err = RegQueryValueEx(hkey, "kukconcolor", NULL, &lType, bytearr, &lSize);
		}
		RegCloseKey(hkey);
		if (err != ERROR_SUCCESS) {
#ifdef _DEBUG
			printf("-pop: no color found.\n");
#endif
			exit(1);
		}
		err = RegOpenCurrentUser(KEY_SET_VALUE, &hkey);
		if (err == ERROR_SUCCESS) {
			err = RegDeleteValue(hkey, "kukconcolor");
		}
		RegCloseKey(hkey);
		if (err != ERROR_SUCCESS) {
#ifdef _DEBUG
			printf("-pop: can't open regkey.\n");
#endif
			exit(1);
		}
		bytearr[5] = 0;
		oc_fg = atoi((char *)bytearr);
		oc_bg = atoi(((char *)bytearr) + 6);
		strcpy(color, getColorName(oc_fg));
		strcpy(bgcolor, getColorName(oc_bg));
#ifdef _DEBUG
		printf("POP : %s;%s\n", color, bgcolor);
#endif
		mode = MODE_SETCOLOR;
		break;
	}

	if (argc > param_idx_color) strcpy(color, argv[param_idx_color]);
	if (argc > param_idx_color + 1) strcpy(bgcolor, argv[param_idx_color + 1]);

	//	std::cout<<"args: "<<argc<<"\n";
	//	std::ws(std::cin);

	if (color[0])	set_colors(color, 1);
	if (bgcolor[0])	set_colors(bgcolor, 0);

	switch (mode) {
	case MODE_DEFAULT:
	default:
		std::cin.width(BUFFER_SIZE);
		while (!std::cin.eof()) {
			memset(xx, 0, sizeof(xx));
			std::cin.getline(xx, BUFFER_SIZE);
			std::cout << xx << "\n";
		}
		x.RestoreAttributes();
		break;
	case MODE_SETCOLOR:
		break;
	case MODE_PRINT:
		std::cout << xx;
		x.RestoreAttributes();
		break;
	case MODE_PRINTLN:
		std::cout << xx << "\n";
		x.RestoreAttributes();
		break;
	}

	return 0;
}