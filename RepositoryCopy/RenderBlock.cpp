#include "basic.h"
#include "RenderBlock.h"
#include "Block.h"
#include "World.h"
#include <vector>

const std::vector<int> DAWN = { 110, 140, 180 };
const std::vector<int> NOON = { 135, 206, 235 };
const std::vector<int> DUSK = { 255, 160, 100 };
const std::vector<int> MIDNIGHT = { 5, 10, 30 };

// 플레이어
void RenderBlock_1(const Block& block, const World& world) {
	std::vector<int> PlayerTextColor = { 255, 255, 255 };
	std::vector<int> PlayerBackColor = { 59, 120, 255 };
	std::string PlayerText = " P ";

	if (world.player.BuffState[FLAME] > 0) {
		PlayerBackColor = { 214, 73, 26 };
	}
	if (world.player.BuffState[POISON] > 0) {
		PlayerTextColor = { 68, 128, 36 };
	}
	if (world.player.BuffState[WITHER] > 0) {
		PlayerText = ";;;";
	}

	PrintWithColor(PlayerTextColor, PlayerBackColor, PlayerText);
}

// 공기
void RenderBlock0(const Block& block, const World& world) {
	if (world.worldType == OVERWORLD) {
		if (block.posR <= 7) {
			std::vector<int> skyColor;
			if (world.currentTime < 25) {
				skyColor = linearColor(DAWN, NOON, 0, 24, world.currentTime);
			}
			else if (world.currentTime < 75) {
				skyColor = NOON;
			}
			else if (world.currentTime < 100) {
				skyColor = linearColor(NOON, DUSK, 75, 99, world.currentTime);
			}
			else if (world.currentTime < 125) {
				skyColor = linearColor(DUSK, MIDNIGHT, 100, 124, world.currentTime);
			}
			else if (world.currentTime < 160) {
				skyColor = MIDNIGHT;
			}
			else {
				skyColor = linearColor(MIDNIGHT, DAWN, 160, 179, world.currentTime);
			}
			PrintWithColor(skyColor, skyColor, " . ");
		}
		else if (block.posR >= 8 && block.posR <= 12) {
			PrintWithColor({ 70, 30, 0 }, { 70, 30, 0 }, " . ");
		}
		else {
			PrintWithColor({ 35, 35, 35 }, { 35, 35, 35 }, " . ");
		}
	}
	else if (world.worldType == NETHER) {
		if (block.posC <= WORLD_WIDTH / 2) {
			std::vector<int> skyColor;
			skyColor = linearColor({ 19, 58, 49 }, { 71, 14, 14 }, 0, WORLD_WIDTH / 2, block.posC);
			PrintWithColor(skyColor, skyColor, "   ");
		}
		else if (block.posR >= 5 && block.posR <= 7 && block.posC >= 3 * WORLD_WIDTH / 4) {
			PrintWithColor({ 23, 12, 14 }, { 15, 5, 6 }, "_][");
		}
		else {
			PrintWithColor({ 71, 14, 14 }, { 71, 14, 14 }, "   ");
		}
	}
	else if (world.worldType == THE_END) {
		PrintWithColor({ 0, 0, 0 }, { 0, 0, 0 }, " . ");
	}
}

// 돌
void RenderBlock1(const Block& block, const World& world) {
	PrintWithColor({ 133, 133, 133 }, { 133, 133, 133 }, block.ColorNoise, "   ");
}

// 잔디 블럭
void RenderBlock2(const Block& block, const World& world) {
	PrintWithColor({ 9, 225 + 2 * (block.ColorNoise[1]), 16 }, { 9, 225 + 2 * (block.ColorNoise[1]), 16 }, block.ColorNoise, " . ");
}

// 흙
void RenderBlock3(const Block& block, const World& world) {
	PrintWithColor({ 135, 70, 18 }, { 135, 70, 18 }, block.ColorNoise, " . ");
}

// 조약돌
void RenderBlock4(const Block& block, const World& world) {
	PrintWithColor({ 133, 133, 133 }, { 179, 179, 179 }, block.ColorNoise, ":::");
}

// 참나무 판자
void RenderBlock5(const Block& block, const World& world) {
	PrintWithColor({ 189, 92, 21 }, { 185, 122, 87 }, block.ColorNoise, " = ");
}

// 기반암
void RenderBlock7(const Block& block, const World& world) {
	PrintWithColor({ 70, 70, 70 }, { 25, 25, 25 }, block.ColorNoise, "X#X");
}

// 물
void RenderBlock8(const Block& block, const World& world) {
	PrintWithColor({ 46, 116, 215 }, { 46, 116, 215 }, block.ColorNoise, "   ");
}

// 용암
void RenderBlock10(const Block& block, const World& world) {
	PrintWithColor({ 243, 54, 29 }, { 243, 54, 29 }, block.ColorNoise, "   ");
}

// 자갈
void RenderBlock13(const Block& block, const World& world) {
	PrintWithColor({ 145, 145, 145 }, { 105, 105, 105 }, block.ColorNoise, "o.o");
}

// 금 광석
void RenderBlock14(const Block& block, const World& world) {
	PrintWithColor({ 255, 255, 50 }, { 180, 180, 133 }, block.ColorNoise, "`,\'");
}

// 철 광석
void RenderBlock15(const Block& block, const World& world) {
	PrintWithColor({ 237, 183, 136 }, { 160, 145, 133 }, block.ColorNoise, ";,`");
}

// 석탄 광석
void RenderBlock16(const Block& block, const World& world) {
	PrintWithColor({ 31, 20, 20 }, { 100, 100, 100 }, block.ColorNoise, "`.-");
}

// 참나무 원목
void RenderBlock17(const Block& block, const World& world) {
	PrintWithColor({ 207, 79, 2 }, { 102, 39, 1 }, block.ColorNoise, "|||");
}

// 참나무 잎
void RenderBlock18(const Block& block, const World& world) {
	PrintWithColor({ 34, 143, 30 }, { 51, 217, 46 }, block.ColorNoise, " . ");
}

// 흑요석
void RenderBlock49(const Block& block, const World& world) {
	PrintWithColor({ 64, 36, 96 }, { 12, 8, 20 }, block.ColorNoise, "#%#");
}

// 다이아몬드 광석
void RenderBlock56(const Block& block, const World& world) {
	PrintWithColor({ 0, 255, 255 }, { 133, 170, 170 }, block.ColorNoise, ";`.");
}

// 제작대
void RenderBlock58(const Block& block, const World& world) {
	PrintWithColor({ 130, 130, 130 }, { 100, 50, 20 }, block.ColorNoise, "|X|");
}

// 화로
void RenderBlock61(const Block& block, const World& world) {
	PrintWithColor({ 255, 75, 5 }, { 100, 100, 100 }, block.ColorNoise, " 0 ");
}

// 네더랙
void RenderBlock87(const Block& block, const World& world) {
	PrintWithColor({ 120, 35, 35 }, { 120, 35, 35 }, block.ColorNoise, "   ");
}

// 영혼 모래
void RenderBlock88(const Block& block, const World& world) {
	PrintWithColor({ 168, 118, 82 }, { 72, 48, 38 }, block.ColorNoise, "°o°");
}

// 발광석
void RenderBlock89(const Block& block, const World& world) {
	PrintWithColor({ 255, 240, 120 }, { 190, 130, 35 }, block.ColorNoise, " * ");
}

// 네더 포탈
void RenderBlock90(const Block& block, const World& world) {
	PrintWithColor({ 170, 40, 255 }, { 45, 0, 80 }, block.ColorNoise, " @ ");
}

// 네더 벽돌
void RenderBlock112(const Block& block, const World& world) {
	PrintWithColor({ 95, 25, 25 }, { 45, 10, 10 }, block.ColorNoise, "][_");
}

// 엔드 돌
void RenderBlock121(const Block& block, const World& world) {
	PrintWithColor({ 228, 222, 168 }, { 185, 178, 120 }, block.ColorNoise, "   ");
}

// 흑암
void RenderBlock2000(const Block& block, const World& world) {
	PrintWithColor({ 22, 22, 28 }, { 6, 6, 10 }, block.ColorNoise, "###");
}

// 뒤틀린 네사체
void RenderBlock2001(const Block& block, const World& world) {
	PrintWithColor({ 60, 210, 190 }, { 70, 20, 20 }, block.ColorNoise, "\"^\"");
}

// 뒤틀린 자루
void RenderBlock2002(const Block& block, const World& world) {
	PrintWithColor({ 46, 118, 118 }, { 70, 37, 54 }, block.ColorNoise, "|^|");
}

// 뒤틀린 사마귀 블록
void RenderBlock2003(const Block& block, const World& world) {
	PrintWithColor({ 25, 170, 155 }, { 12, 85, 80 }, block.ColorNoise, " . ");
}

// 네더 금 광석
void RenderBlock2004(const Block& block, const World& world) {
	PrintWithColor({ 255, 205, 45 }, { 95, 25, 25 }, block.ColorNoise, "\'.;");
}

// 고대 잔해
void RenderBlock2005(const Block& block, const World& world) {
	PrintWithColor({ 185, 110, 70 }, { 40, 30, 25 }, block.ColorNoise, "#X#");
}

// 엔더드래곤
void RenderBlock2006(const Block& block, const World& world) {
	PrintWithColor({ 255, 255, 255 }, { 145, 44, 115 }, block.ColorNoise, " D ");
}






