#pragma once
#include <queue>
#include <string>

// 출력용 텍스트. 외부 함수에서 이용한 후 한번에 출력 후 초기화 용도
extern std::queue<std::string> TextBuffer;
extern std::string TempTextLine;

// 상수
extern const int WORLD_WIDTH;
extern const int WORLD_HEIGHT;
extern const int LENS_WIDTH_SIZE;
extern const int LENS_HEIGHT_SIZE;
extern const int INTERVAL_MAP_WITH_CONSOLE;
extern const int INVENTORY_UI_WIDTH;
extern const int CRAFT_UI_WIDTH;
extern const int BATTLE_UI_WIDTH;

// 기본 함수
void keyboardWait();

int range(int target, int min, int max);

char inputTask();

void printUILine(int white1Length, int blankLength, int white2Length, bool doLineBreak);

int getTextWidth(const std::string& text);

void moveCursor(int x, int y);

void clearScreen();

std::vector<int> linearColor(const std::vector<int>& startColor, const std::vector<int>& endColor, const int startValue, const int endValue, const int currentValue);

std::string InputString();

void Start();

void End();