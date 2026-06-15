#pragma once
#include <random>

extern std::random_device rd;

int RandomIntRange(int min, int max);

std::vector<int> RandomIntRange(int min, int max, int count);