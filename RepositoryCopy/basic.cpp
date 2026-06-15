#include <iostream>
#include <windows.h>
#include "basic.h"
#include "colorprint.h"

std::queue<std::string> TextBuffer;
std::string TempTextLine;

extern const int WORLD_WIDTH = 60;
extern const int WORLD_HEIGHT = 120;
extern const int LENS_WIDTH_SIZE = 7;
extern const int LENS_HEIGHT_SIZE = 10;
extern const int INTERVAL_MAP_WITH_CONSOLE = 5;
extern const int INVENTORY_UI_WIDTH = 20;
extern const int CRAFT_UI_WIDTH = 54;
extern const int BATTLE_UI_WIDTH = 50;

void keyboardWait() {
	printf("계속 진행하려면 아무 키나 입력하세요 : ");
	char input; std::cin >> input;
	printf("\n");
}

int range(int target, int min, int max) {
	return min(max(target, min), max);
}

char inputTask() {
	char input;
	printf("입력 : ");
	std::cin >> input;
	return input;
}

void printUILine(int white1Length, int blankLength, int white2Length, bool doLineBreak) {
	for (int i = 0; i < white1Length; i++) {
		PrintWithColor({ 255, 255, 255 }, { 255, 255, 255 }, "   ");
	}
	for (int i = 0; i < blankLength; i++) {
		printf("   ");
	}
	for (int i = 0; i < white2Length; i++) {
		PrintWithColor({ 255, 255, 255 }, { 255, 255, 255 }, "   ");
	}

	if (doLineBreak) {
		printf("\n");
	}
}

int getTextWidth(const std::string& text) {
	int result = 0;

	for (size_t i = 0; i < text.size();) {
		if (text[i] == '\x1b') {
			if (i + 1 < text.size() && text[i + 1] == '[') {
				i += 2;
				while (i < text.size() && text[i] != 'm') {
					i++;
				}
				if (i < text.size()) {
					i++;
				}
				continue;
			}
		}
		unsigned char c = static_cast<unsigned char>(text[i]);
		if (c < 128) {
			result += 1;
			i += 1;
		}
		else {
			result += 2;
			if ((c & 0xE0) == 0xC0) i += 2;
			else if ((c & 0xF0) == 0xE0) i += 3;
			else if ((c & 0xF8) == 0xF0) i += 4;
			else i += 1;
		}
	}
	return result;
}

void moveCursor(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void clearScreen() {
	moveCursor(0, 0);
	std::string blankLine = "";
	for (int column = 0; column < 200; column++) {
		blankLine += " ";
	}
	blankLine += "\n";
	for (int row = 0; row < 50; row++) {
		printf("%s", blankLine.c_str());
	}
	moveCursor(0, 0);
}

std::vector<int> linearColor(const std::vector<int>& startColor, const std::vector<int>& endColor, const int startValue, const int endValue, const int currentValue) {
	std::vector<int> result = { 0, 0, 0 };
	result[0] = startColor[0] + (currentValue - startValue) * (endColor[0] - startColor[0]) / (endValue - startValue);
	result[1] = startColor[1] + (currentValue - startValue) * (endColor[1] - startColor[1]) / (endValue - startValue);
	result[2] = startColor[2] + (currentValue - startValue) * (endColor[2] - startColor[2]) / (endValue - startValue);
	return result;
}

std::string InputString() {
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	wchar_t wbuffer[1024] = {};
	DWORD read = 0;

	if (!ReadConsoleW(hInput, wbuffer, 1023, &read, nullptr)) {
		return "";
	}

	if (read >= 2 && wbuffer[read - 2] == L'\r' && wbuffer[read - 1] == L'\n') {
		wbuffer[read - 2] = L'\0';
	}
	else if (read >= 1) {
		wbuffer[read - 1] = L'\0';
	}

	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, nullptr, 0, nullptr, nullptr);

	std::string result(utf8Length - 1, '\0');

	WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, result.data(), utf8Length, nullptr, nullptr);

	return result;
}

void Start() {
	printf("⠀⠀⠀⠀⢀⣀⣀⣀⣀⣀⣀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⠀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀\n");
	printf("⠀⠀⠀⠀⣾⠉⠀⠁⠀⢩⣿⣦⡀⠀⠀⢠⡏⠀⠀⠀⠀⣹⣿⠋⠀⠁⠈⠀⣿⣿⠁⠀⠀⠀⢨⣿⡄⢸⠃⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⡇⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡄\n");
	printf("⠀⠀⠀⣰⠇⠀⠀⠀⠀⠘⢛⣿⣿⣴⠟⠛⠁⠀⠀⠀⢀⣿⡿⠀⠀⠀⠀⢰⣿⡇⠀⠀⠀⠀⠘⠛⣿⣿⠒⠀⠀⠀⢠⣿⡇⠀⠀⠀⠀⢠⣶⣤⣦⣴⣤⣶⣾⣿⠁⠀⠀⠀⠀⣴⣦⣶⣴⣦⣴⣾⣿⡇⠀⠀⠀⠀⢰⣶⡄⠀⠀⠀⠀⢸⣿⡀⠀⠀⣶⣶⡀⠀⣶⣶⡄⠀⠀⣿⣧⠀⠀⠀⠀⠠⣦⣴⣶⣤⣤⣴⣿⣿⣦⣤⣤⣆⠀⠀⠀⠀⠰⣶⣤⣼⠗\n");
	printf("⠀⠀⢀⡟⠀⠀⠀⠀⠀⠀⠼⡿⢿⠿⠀⠀⠀⠀⠀⠀⣼⣿⠇⠀⠀⠀⠀⣸⣿⠁⠀⠀⠀⠀⠀⠠⠿⠇⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠸⠿⠿⢿⠿⠿⠿⣿⣿⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⡟⠁⠀⠀⠀⠘⠿⠇⠀⠀⠀⣀⣸⣿⡇⠀⠀⠀⣬⣿⣿⣧⡍⠀⠀⠀⢻⣿⠀⠀⠀⠀⠀⠿⠿⠿⠿⠿⠿⠿⣿⣿⣿⣿⣿⡀⠀⠀⠀⠀⢿⡿⠃⠀\n");
	printf("⠀⠀⣼⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡿⠀⠀⠀⠀⢀⣿⡟⠀⠀⠀⠀⢀⣤⡄⠀⠀⠀⠀⠀⠀⢸⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠻⣿⡇⠀⠀⠀⠸⡟⠛⠻⡇⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣇⠀⠀⠀⠀⠘⣇⠀⠀\n");
	printf("⠀⢰⡇⠀⠀⠀⠀⣰⣶⠂⠀⠀⢰⣶⠆⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⢸⣿⠇⠀⠀⠀⠀⢸⣿⡃⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣶⣶⣶⣶⣶⣶⣶⣿⣿⠀⠀⠀⠀⠀⣿⠉⠉⠉⠉⠉⠉⢻⣿⠀⠀⠀⠀⠀⣶⣶⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣷⠀⠀⠀⠀⠀⣶⣶⣶⣷⣶⣶⣾⣿⣿⣿⣿⣿⡀⠀⠀⠀⠀⢹⡄⠀\n");
	printf("⢀⡟⠀⠀⠀⠀⢠⣿⣟⣀⣀⣀⣾⡿⠀⠀⠀⠀⠀⣾⣿⠀⠀⠀⠀⠀⣾⣿⠀⠀⠀⠀⠀⣿⣿⣿⡟⠀⠀⠀⠀⠀⣿⡿⠀⠀⠀⠀⠀⠿⠿⠿⠿⠿⠿⠿⣿⣿⠀⠀⠀⠀⠀⠿⠦⠤⠴⠤⠦⠤⣾⣿⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⢿⣿⠀⠀⠀⠀⠀⢻⣿⠀⠀⠀⠀⠀⣻⣿⡀⠀⠀⠀⠀⢹⣿⣿⣿⣿⡿⠙⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠈⣷⠀\n");
	printf("⣼⠁⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⢠⣿⡇⠀⠀⠀⠀⢠⣿⡇⠀⠀⠀⠀⠀⣿⣿⣿⡇⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⢻⣿⠀⠀⠀⠀⠀⢸⣿⡄⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠘⣿⣿⣿⠟⠀⠀⢹⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⢸⡆\n");
	printf("⢿⣤⣤⣤⣤⣴⣿⣿⣿⣿⣿⣿⣿⣦⣤⣤⣤⣴⣾⣿⣧⣤⣶⣤⣤⣾⣿⣷⣤⣶⣤⣤⣾⣿⣿⣿⣶⣤⣤⣶⣤⣾⣿⣷⣤⣴⣦⣤⣤⣤⣤⣴⣦⣤⣤⣶⣿⣿⣤⣤⣶⣾⣷⣤⣤⣤⣤⣶⣦⣴⣿⣿⣶⣤⣤⣤⣴⣾⣿⣶⣤⣶⣤⣴⣾⣿⣷⣤⣴⣶⣤⣾⣿⣷⣤⣤⣤⣤⣴⣿⣿⣤⣤⣤⣤⣶⡟⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣷⣦⣤⣤⣤⣴⠟\n");
	printf("⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠛⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀\n");
	printf("⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⡟⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀\n\n");
	printf("<< 새로운 월드를 시작하려면 아무 키나 입력하세요 >>\n");
	char Input; std::cin >> Input;
}

void End() {
	printf("⠀⠀⠀⠀⢀⣀⣀⣀⣀⣀⣀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⠀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀\n");
	Sleep(700);
	printf("⠀⠀⠀⠀⣾⠉⠀⠁⠀⢩⣿⣦⡀⠀⠀⢠⡏⠀⠀⠀⠀⣹⣿⠋⠀⠁⠈⠀⣿⣿⠁⠀⠀⠀⢨⣿⡄⢸⠃⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⡇⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡄\n");
	Sleep(700);
	printf("⠀⠀⠀⣰⠇⠀⠀⠀⠀⠘⢛⣿⣿⣴⠟⠛⠁⠀⠀⠀⢀⣿⡿⠀⠀⠀⠀⢰⣿⡇⠀⠀⠀⠀⠘⠛⣿⣿⠒⠀⠀⠀⢠⣿⡇⠀⠀⠀⠀⢠⣶⣤⣦⣴⣤⣶⣾⣿⠁⠀⠀⠀⠀⣴⣦⣶⣴⣦⣴⣾⣿⡇⠀⠀⠀⠀⢰⣶⡄⠀⠀⠀⠀⢸⣿⡀⠀⠀⣶⣶⡀⠀⣶⣶⡄⠀⠀⣿⣧⠀⠀⠀⠀⠠⣦⣴⣶⣤⣤⣴⣿⣿⣦⣤⣤⣆⠀⠀⠀⠀⠰⣶⣤⣼⠗\n");
	Sleep(700);
	printf("⠀⠀⢀⡟⠀⠀⠀⠀⠀⠀⠼⡿⢿⠿⠀⠀⠀⠀⠀⠀⣼⣿⠇⠀⠀⠀⠀⣸⣿⠁⠀⠀⠀⠀⠀⠠⠿⠇⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠸⠿⠿⢿⠿⠿⠿⣿⣿⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⡟⠁⠀⠀⠀⠘⠿⠇⠀⠀⠀⣀⣸⣿⡇⠀⠀⠀⣬⣿⣿⣧⡍⠀⠀⠀⢻⣿⠀⠀⠀⠀⠀⠿⠿⠿⠿⠿⠿⠿⣿⣿⣿⣿⣿⡀⠀⠀⠀⠀⢿⡿⠃⠀\n");
	Sleep(700);
	printf("⠀⠀⣼⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡿⠀⠀⠀⠀⢀⣿⡟⠀⠀⠀⠀⢀⣤⡄⠀⠀⠀⠀⠀⠀⢸⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠻⣿⡇⠀⠀⠀⠸⡟⠛⠻⡇⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣇⠀⠀⠀⠀⠘⣇⠀⠀\n");
	Sleep(700);
	printf("⠀⢰⡇⠀⠀⠀⠀⣰⣶⠂⠀⠀⢰⣶⠆⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⢸⣿⠇⠀⠀⠀⠀⢸⣿⡃⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣶⣶⣶⣶⣶⣶⣶⣿⣿⠀⠀⠀⠀⠀⣿⠉⠉⠉⠉⠉⠉⢻⣿⠀⠀⠀⠀⠀⣶⣶⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣷⠀⠀⠀⠀⠀⣶⣶⣶⣷⣶⣶⣾⣿⣿⣿⣿⣿⡀⠀⠀⠀⠀⢹⡄⠀\n");
	Sleep(700);
	printf("⢀⡟⠀⠀⠀⠀⢠⣿⣟⣀⣀⣀⣾⡿⠀⠀⠀⠀⠀⣾⣿⠀⠀⠀⠀⠀⣾⣿⠀⠀⠀⠀⠀⣿⣿⣿⡟⠀⠀⠀⠀⠀⣿⡿⠀⠀⠀⠀⠀⠿⠿⠿⠿⠿⠿⠿⣿⣿⠀⠀⠀⠀⠀⠿⠦⠤⠴⠤⠦⠤⣾⣿⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⢿⣿⠀⠀⠀⠀⠀⢻⣿⠀⠀⠀⠀⠀⣻⣿⡀⠀⠀⠀⠀⢹⣿⣿⣿⣿⡿⠙⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠈⣷⠀\n");
	Sleep(700);
	printf("⣼⠁⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⢠⣿⡇⠀⠀⠀⠀⢠⣿⡇⠀⠀⠀⠀⠀⣿⣿⣿⡇⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⢻⣿⠀⠀⠀⠀⠀⢸⣿⡄⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠘⣿⣿⣿⠟⠀⠀⢹⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⢸⡆\n");
	Sleep(700);
	printf("⢿⣤⣤⣤⣤⣴⣿⣿⣿⣿⣿⣿⣿⣦⣤⣤⣤⣴⣾⣿⣧⣤⣶⣤⣤⣾⣿⣷⣤⣶⣤⣤⣾⣿⣿⣿⣶⣤⣤⣶⣤⣾⣿⣷⣤⣴⣦⣤⣤⣤⣤⣴⣦⣤⣤⣶⣿⣿⣤⣤⣶⣾⣷⣤⣤⣤⣤⣶⣦⣴⣿⣿⣶⣤⣤⣤⣴⣾⣿⣶⣤⣶⣤⣴⣾⣿⣷⣤⣴⣶⣤⣾⣿⣷⣤⣤⣤⣤⣴⣿⣿⣤⣤⣤⣤⣶⡟⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣷⣦⣤⣤⣤⣴⠟\n");
	Sleep(700);
	printf("⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠛⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀\n");
	Sleep(700);
	printf("⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⡟⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀\n\n");
	Sleep(700);
	printf("\n");
	Sleep(700);
	printf("\n");
	Sleep(700);
	printf("\n");
	Sleep(700);
	printf("<< 엔더 드래곤을 물리치고 오버월드의 평화를 되찾으셨습니다. >>\n");
	Sleep(700);
	printf("\n");
	Sleep(700);
	printf("<< 게임을 클리어 하셨습니다. 축하드립니다! >>\n");
	Sleep(700);
	printf("\n");
	Sleep(700);
	printf("\n");
	Sleep(700);
	printf("\n");
	printf("- THE END -");
	exit(0);
}