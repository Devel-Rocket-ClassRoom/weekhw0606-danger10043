#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include "colorprint.h"

extern const int BLACK = 0;
extern const int DARK_BLUE = 1;
extern const int DARK_GREEN = 2;
extern const int DARK_CYAN = 3;
extern const int DARK_RED = 4;
extern const int DARK_PURPLE = 5;
extern const int DARK_YELLOW = 6;
extern const int GRAY = 7;
extern const int DARK_GRAY = 8;
extern const int BLUE = 9;
extern const int GREEN = 10;
extern const int CYAN = 11;
extern const int RED = 12;
extern const int PURPLE = 13;
extern const int YELLOW = 14;
extern const int WHITE = 15;


void SetPrintColor(int colorCode) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}
void SetPrintColor(int textColor, int backgroundColor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16 * backgroundColor + textColor);
}
void PrintWithColor(int textColor, int backgroundColor, const std::string& text) {
	SetPrintColor(textColor, backgroundColor);
	printf("%3s", text.c_str());
	SetPrintColor(15, 15);
}

void PrintWithColor(const std::vector<int>& textColor, const std::string& text) {
	int TextRColor = textColor[0];
	int TextGColor = textColor[1];
	int TextBColor = textColor[2];
	std::string TextColor = "\x1b[38;2;" + std::to_string(TextRColor) + ";" + std::to_string(TextGColor) + ";" + std::to_string(TextBColor) + "m";
	std::cout << TextColor;
	std::cout << text;
	std::cout << "\x1b[0m";
}

void PrintWithColor(const std::vector<int>& textColor, const std::vector<int>& backgroundColor, const std::string& text) {
	int TextRColor = textColor[0];
	int TextGColor = textColor[1];
	int TextBColor = textColor[2];
	std::string TextColor = "\x1b[38;2;" + std::to_string(TextRColor) + ";" + std::to_string(TextGColor) + ";" + std::to_string(TextBColor) + "m";
	std::cout << TextColor;
	int BackgroundRColor = backgroundColor[0];
	int BackgroundGColor = backgroundColor[1];
	int BackgroundBColor = backgroundColor[2];
	std::string BackgroundColor = "\x1b[48;2;" + std::to_string(BackgroundRColor) + ";" + std::to_string(BackgroundGColor) + ";" + std::to_string(BackgroundBColor) + "m";
	std::cout << BackgroundColor;
	std::cout << text;
	std::cout << "\x1b[0m";
}
void PrintWithColor(const std::vector<int>& textColor, const std::vector<int>& backgroundColor, const std::vector<int>& colorNoise, const std::string& text) {
	int TempTextRColor = max(min(textColor[0] + colorNoise[0], 255), 0);
	int TempTextGColor = max(min(textColor[1] + colorNoise[1], 255), 0);
	int TempTextBColor = max(min(textColor[2] + colorNoise[2], 255), 0);
	int TempBackgroundRColor = max(min(backgroundColor[0] + colorNoise[0], 255), 0);
	int TempBackgroundGColor = max(min(backgroundColor[1] + colorNoise[1], 255), 0);
	int TempBackgroundBColor = max(min(backgroundColor[2] + colorNoise[2], 255), 0);
	PrintWithColor({ TempTextRColor, TempTextGColor, TempTextBColor }, { TempBackgroundRColor, TempBackgroundGColor, TempBackgroundBColor }, text);
}
std::string ColorText(const std::vector<int>& textColor, const std::string& text) {
	int TextRColor = textColor[0];
	int TextGColor = textColor[1];
	int TextBColor = textColor[2];
	std::string TextColor = "\x1b[38;2;" + std::to_string(TextRColor) + ";" + std::to_string(TextGColor) + ";" + std::to_string(TextBColor) + "m";
	return TextColor + text + "\x1b[0m";
}
std::string ColorText(const std::vector<int>& textColor, const std::vector<int>& backgroundColor, const std::string& text) {
	int TextRColor = textColor[0];
	int TextGColor = textColor[1];
	int TextBColor = textColor[2];
	std::string TextColor = "\x1b[38;2;" + std::to_string(TextRColor) + ";" + std::to_string(TextGColor) + ";" + std::to_string(TextBColor) + "m";
	int BackgroundRColor = backgroundColor[0];
	int BackgroundGColor = backgroundColor[1];
	int BackgroundBColor = backgroundColor[2];
	std::string BackgroundColor = "\x1b[48;2;" + std::to_string(BackgroundRColor) + ";" + std::to_string(BackgroundGColor) + ";" + std::to_string(BackgroundBColor) + "m";
	return TextColor + BackgroundColor + text + "\x1b[0m";
}
