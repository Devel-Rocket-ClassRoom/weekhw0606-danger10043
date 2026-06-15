#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

extern const int BLACK;
extern const int DARK_BLUE;
extern const int DARK_GREEN;
extern const int DARK_CYAN;
extern const int DARK_RED;
extern const int DARK_PURPLE;
extern const int DARK_YELLOW;
extern const int GRAY;
extern const int DARK_GRAY;
extern const int BLUE;
extern const int GREEN;
extern const int CYAN;
extern const int RED;
extern const int PURPLE;
extern const int YELLOW;
extern const int WHITE;

struct Color {
	int r, g, b;
};

void SetPrintColor(int colorCode);

void SetPrintColor(int textColor, int backgroundColor);

void PrintWithColor(int textColor, int backgroundColor, const std::string& text);

void PrintWithColor(const std::vector<int>& textColor, const std::string& text);

void PrintWithColor(const std::vector<int>& textColor, const std::vector<int>& backgroundColor, const std::string& text);

void PrintWithColor(const std::vector<int>& textColor, const std::vector<int>& backgroundColor, const std::vector<int>& colorNoise, const std::string& text);

std::string ColorText(const std::vector<int>& textColor, const std::string& text);

std::string ColorText(const std::vector<int>& textColor, const std::vector<int>& backgroundColor, const std::string& text);