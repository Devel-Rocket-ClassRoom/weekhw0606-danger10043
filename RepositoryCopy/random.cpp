#include <random>

// 난수 생성 함수
std::random_device rd;

int RandomIntRange(int min, int max) {
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(min, max);
	return distrib(gen);
}
std::vector<int> RandomIntRange(int min, int max, int count) {
	std::vector<int> result;
	std::vector<int> tempVector;
	for (int i = min; i <= max; i++) {
		tempVector.push_back(i);
	}
	for (int i = 0; i < count; i++) {
		int j = RandomIntRange(i, max - min);
		std::swap(tempVector[i], tempVector[j]);
	}
	for (int i = 0; i < count; i++) {
		result.push_back(tempVector[i]);
	}
	return result;
}