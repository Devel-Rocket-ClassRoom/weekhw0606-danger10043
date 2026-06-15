#include "itemdata.h"
#include "item.h"
#include "basic.h"
#include "colorprint.h"
#include "itemUse.h"
#include "Buff.h"
#include <map>
#include <string>
#include <vector>

// 조합법 관련
std::map<std::vector<std::pair<int, int>>, std::pair<int, int>> RECIPEBOOK = {
	{{{17, 1}}, {5, 4}}, // 참나무 판자
	{{{5, 4}}, {58, 1}}, // 제작대
	{{{4, 8}, {58, -1}}, {61, 1}}, // 화로
	{{{265, 3}, {280, 2}, {58, -1}}, {257, 1}}, // 철 곡괭이
	{{{328, 1}, {265, 1}}, {259, 1}}, // 철 곡괭이
	{{{280, 3}, {287, 3}, {58, -1}}, {261, 1}}, // 활
	{{{4, 1}, {280, 1}}, {262, 4}}, // 화살
	{{{505, 1}, {263, 1}, {61, -1}}, {265, 1}}, // 철괴
	{{{506, 1}, {263, 1}, {61, -1}}, {266, 1}}, // 금괴
	{{{265, 2}, {280, 1}, {58, -1}}, {267, 1}}, // 철 검
	{{{5, 2}, {280, 1}, {58, -1}}, {268, 1}}, // 나무 검
	{{{5, 3}, {280, 2}, {58, -1}}, {270, 1}}, // 나무 곡괭이
	{{{4, 2}, {280, 1}, {58, -1}}, {272, 1}}, // 돌 검
	{{{4, 3}, {280, 2},  {58, -1}}, {274, 1}}, // 돌 곡괭이
	{{{264, 2}, {280, 1}, {58, -1}}, {276, 1}}, // 다이아몬드 검
	{{{264, 3}, {280, 2}, {58, -1}}, {278, 1}}, // 다이아몬드 곡괭이
	{{{5, 2}}, {280, 4}}, // 막대기
	{{{266, 2}, {280, 1}, {58, -1}}, {283, 1}}, // 금 검
	{{{319, 1}, {263, 1}, {61, -1}}, {320, 1}}, // 익힌 돼지고기
	{{{280, 3}, {287, 2}, {58, -1}}, {346, 1}}, // 낚싯대
	{{{349, 1}, {263, 1}, {61, -1}}, {350, 1}}, // 익힌 대구
	{{{363, 1}, {263, 1}, {61, -1}}, {364, 1}}, // 스테이크
	{{{369, 1}}, {377, 2}}, // 블레이즈 가루
	{{{377, 1}, {368, 1}}, {381, 1}}, // 엔더의 눈
	{{{423, 1}, {263, 1}, {61, -1}}, {424, 1}}, // 익힌 양고기
	{{{319, 1}, {363, 1}, {423, 1}}, {1000, 6}}, // 몬스터 미끼
	{{{49, 4}, {264, 3}, {58, -1}}, {1001, 1}}, // 대장장이 모루
	{{{263, 1}, {2005, 4}, {61, -1}}, {1002, 1}}, // 네더라이트 파편
	{{{1002, 4}, {266, 4}, {58, -1}}, {1003, 1}}, // 네더라이트 주괴
	{{{265, 1}, {5, 6}, {58, -1}}, {1004, 1}}, // 방패
	{{{276, 1}, {1003, 1}}, {3000, 1}}, // 네더라이트 검
	{{{261, 1}, {352, 3}, {88, 3}}, {3001, 1}}, // 망령의 활
	{{{289, 3}, {259, 1}, {4, 10}}, {3002, 3}}, // 화염 건틀릿
	{{{87, 50}, {370, 2}}, {3003, 3}}, // 지옥의 심장
};

std::map<std::string, int> ItemName2ItemID = {
	{"공기", 0},
	{"돌", 1},
	{"잔디 블럭", 2},
	{"흙", 3},
	{"조약돌", 4},
	{"참나무 판자", 5},
	{"물", 8},
	{"용암", 10},
	{"자갈", 13},
	{"금 광석", 14},
	{"철 광석", 15},
	{"석탄 광석", 16},
	{"참나무 원목", 17},
	{"참나무 잎", 18},
	{"흑요석", 49},
	{"다이아몬드 광석", 56},
	{"제작대", 58},
	{"화로", 61},
	{"네더랙", 87},
	{"영혼 모래", 88},
	{"네더 포탈", 90},
	{"철 곡괭이", 257},
	{"라이터", 259},
	{"활", 261},
	{"화살", 262},
	{"석탄", 263},
	{"다이아몬드", 264},
	{"철괴", 265},
	{"금괴", 266},
	{"철 검", 267},
	{"나무 검", 268},
	{"나무 곡괭이", 270},
	{"돌 검", 272},
	{"돌 곡괭이", 274},
	{"다이아몬드 검", 276},
	{"다이아몬드 곡괭이", 278},
	{"막대기", 280},
	{"금 검", 283},
	{"실", 287},
	{"화약", 289},
	{"돼지고기", 319},
	{"익힌 돼지고기", 320},
	{"부싯돌", 328},
	{"낚싯대", 346},
	{"대구", 349},
	{"익힌 대구", 350},
	{"뼈", 352},
	{"소고기", 363},
	{"스테이크", 364},
	{"썩은 고기", 367},
	{"엔더 진주", 368},
	{"블레이즈 막대", 369},
	{"가스트의 눈물", 370},
	{"블레이즈 가루", 377},
	{"마그마 크림", 378},
	{"엔더의 눈", 381},
	{"양고기", 423},
	{"익힌 양고기", 424},
	{"철 원석", 505},
	{"금 원석", 506},
	{"몬스터 미끼", 1000},
	{"대장장이 모루", 1001},
	{"네더라이트 파편", 1002},
	{"네더라이트 주괴", 1003},
	{"방패", 1004},
	{"흑암", 2000},
	{"뒤틀린 네사체", 2001},
	{"뒤틀린 자루", 2002},
	{"뒤틀린 사마귀 블록", 2003},
	{"네더 금 광석", 2004},
	{"고대 잔해", 2005},
	{"네더라이트 검", 3000},
	{"망령의 활", 3001},
	{"화염 건틀릿", 3002},
	{"지옥의 심장", 3003},
};

std::map<int, ItemData> ITEM_DATABASE = {
	{0,   {0,   "공기",		         { 255, 255, 255 }, false, BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("세상을 가득 채우고 있는 보이지 않는 기체이다.");
		TextBuffer.push("대부분의 생명체가 숨 쉬는 데 필요하다.");
	}}},

	{1,   {1,   "돌",		         { 120, 120, 120 }, true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("지하에서 흔히 발견되는 단단한 암석이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "나무 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
	}}},

	{2,   {2,   "잔디 블럭",         { 21, 249, 28 },   true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("흙 위에 풀이 자란 블럭이다. 다양한 식물이 자라기에 적합하다.");
	}}},

	{3,   {3,   "흙",		         { 140, 88, 42 },   true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("식물이 자라기에 적합한 부드러운 흙이다.");
	}}},

	{4,   {4,   "조약돌",	         { 120, 120, 120 }, true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("돌이 부서져 거칠어진 암석이다. 건축과 제작에 널리 사용된다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "나무 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
	}}},

	{5,   {5,   "참나무 판자",       { 158, 132, 83 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("참나무 원목을 가공하여 만든 건축 자재이다.");
	}}},

	{8,   {8,   "물",                { 46, 116, 215 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("흐르며 주변을 적시는 액체이다. 생명과 모험에 꼭 필요한 자원이다.");
	}}},

	{10,  {10,  "용암",              { 243, 54, 29  },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("뜨겁게 끓어오르는 액체 암석이다. 닿는 것은 대부분 불타 버린다.");
	}}},

	{13,  {13,  "자갈",              { 140, 140, 140 }, true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("작은 돌들이 모여 이루어진 거친 블럭이다. 채굴하면 부싯돌이 나올 수도 있다.");
	}}},

	{14,  {14,  "금 광석",           { 255, 255, 70 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("제련하면 금을 얻을 수 있는 희귀한 광석이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "철 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
	}}},

	{15,  {15,  "철 광석",           { 197, 167, 144 }, true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("제련하면 철을 얻을 수 있는 광석이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "돌 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
	}}},

	{16,  {16,  "석탄 광석",         { 120, 120, 120 }, true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("채굴하면 석탄을 얻을 수 있는 광석이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "나무 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
	}}},

	{17,  {17,  "참나무 원목",       { 148, 107, 45},   true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("참나무의 줄기 부분이다. 다양한 제작과 건축에 사용된다.");
	}}},

	{18,  {18,  "참나무 잎",         { 21, 249, 28 },   true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("참나무를 이루는 싱그러운 잎사귀이다.");
	}}},


	{49,  {49,  "흑요석",            { 101, 6, 131 },   true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("물과 용암이 만나 만들어진 단단한 화산암이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "다이아몬드 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "적절하게 배치하여 라이터를 사용하면 네더 포탈을 만들 수 있다."));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "(자세한 설명은 \"네더 포탈\" 아이템 설명을 참고)"));
	}}},

	{56,  {56,  "다이아몬드 광석",   { 55, 224, 221 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("채굴하면 다이아몬드를 얻을 수 있는 매우 희귀한 광석이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "철 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
	}}},

	{58,  {58,  "제작대",            { 148, 107, 45 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("다양한 아이템과 블럭을 제작할 수 있는 작업대이다.");
	}}},

	{61,  {61,  "화로",              { 120, 120, 120 }, true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("광석을 제련하거나 음식을 구울 수 있는 작업 블럭이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "돌 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
	}}},

	{87,  {87,  "네더랙",            { 204, 57, 57 },   true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("네더를 이루는 붉은 암석이다. 뜨거운 열기가 스며 있는 듯하다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "나무 곡괭이 이상의 곡괭이로만 채굴이 가능하다."));
	}}},

	{88,  {88,  "영혼 모래",         { 121, 100, 86 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("수많은 영혼이 깃들어 있는 기묘한 모래이다. 네더 월드의 여러 곳에 퍼져있다.");
	}}},

	{89,  {89,  "발광석",            { 190, 130, 35 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("은은한 빛을 내는 신비로운 광석이다. 네더의 어둠을 밝힌다.");
	}}},

	{90,  {90,  "네더 포탈",         { 170, 40, 255 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("현실과 네더를 잇는 차원문이다. 신비로운 보랏빛 에너지가 흐르고 있다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "플레이어가 네더 포탈의 위치로 이동하면, [네더]로 이동한다."));
		TextBuffer.push(ColorText({ 120, 255, 120 }, "< 네더 포탈 생성 조건 >"));
		TextBuffer.push(ColorText({ 120, 255, 120 }, "플레이어가 다음 조건을 갖춘 \"흑요석\"에 라이터를 사용한다 :"));
		TextBuffer.push(ColorText({ 120, 255, 120 }, "1. 해당 흑요석은 \"흑요석으로만 폐쇄되어있는 공간\"의 테두리중 하나이다."));
		TextBuffer.push(ColorText({ 120, 255, 120 }, "2. 해당 공간에는 플레이어를 포함한 어떤 블럭도 존재하면 안된다."));
		TextBuffer.push(ColorText({ 120, 255, 120 }, "3. 해당 공간의 넓이는 최소 8칸 이다."));
	}}},

	{112,  {112,  "네더 벽돌",       { 204, 57, 57 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("네더랙을 구워 만든 벽돌이다. 견고하고 어두운 붉은빛을 띤다.");
	}}},

	{121,  {121,  "엔드 돌",       { 185, 178, 120 },  true,  BLOCK,    USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("엔드 차원을 이루는 창백한 돌이다. 메마르고 기묘한 기운이 감돈다.");
	}}},

	{257, {257, "철 곡괭이",         { 255, 255, 255 }, false, TOOL,     USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("철로 제작된 곡괭이이다. 단단한 광물도 채굴할 수 있다.");
	}}},

	{259, {259, "라이터",            { 140, 140, 140 }, false, TOOL,     USE_BOTH,   useItem259, battleUseItem259, []() {
		TextBuffer.push("불을 붙일 수 있는 도구이다. 모험과 생존에 유용하게 사용된다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "이 아이템은 사용해도 소모되지 않는다."));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "상하좌우 중 방향을 하나 선택한다."));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "선택한 방향의 인접 블럭이 적절하게 배치된 \"흑요석\"인 경우,"));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "네더 포탈을 생성한다."));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "(자세한 설명은 \"네더 포탈\" 아이템 설명을 참고)"));
	}}},

	{261, {261, "활",                { 158, 132, 83 },  false, WEAPON,   USE_BATTLE, nullptr, battleUseItem261, []() {
		TextBuffer.push("화살을 발사할 수 있는 원거리 무기이다.");
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "이 아이템은 사용해도 소모되지 않는다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "화살 하나를 소모하여 적에게 공격력의 150% 의 피해를 입힌다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "적이 피해를 입지 않은 상태일 경우 대신 250% 의 피해를 입힌다."));
	}}},

	{262, {262, "화살",              { 255, 255, 255 }, false, WEAPON,   USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("활에 사용되는 투사체이다. 먼 거리의 대상을 공격할 수 있다.");
	}}},

	{263, {263, "석탄",              { 90, 90, 90 },    false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("오랫동안 타오르는 연료이다. 제련에 주로 사용된다.");
	}}},

	{264, {264, "다이아몬드",        { 55, 224, 221 },  false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("채굴하면 다이아몬드를 얻을 수 있는 매우 희귀한 광석이다.");
		TextBuffer.push("1캐럿에 2500만원에 팔린다.");
	}}},

	{265, {265, "철괴",              { 255, 255, 255 }, false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("철 광석을 제련하여 얻는 금속 주괴이다. 다양한 도구와 장비 제작에 사용된다.");
	}}},

	{266, {266, "금괴",              { 255, 255, 70 },  false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("금 광석을 제련하여 얻는 금속 주괴이다.");
		TextBuffer.push("한 돈에 95만원에 팔린다.");
	}}},

	{267, {267, "철 검",             { 255, 255, 255 }, false, WEAPON,   USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("철로 제작된 검이다. 적과 몬스터를 상대하기에 적합하다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "보유 시 공격력이 25 증가한다."));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "(다른 검의 공격력 증가와 중첩되지 않는다.)"));
	}}},

	{268, {268, "나무 검",           { 158, 132, 83 },  false, WEAPON,   USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("나무로 만든 기본적인 검이다. 적과 몬스터를 상대하기에 적합하다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "보유 시 공격력이 15 증가한다."));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "(다른 검의 공격력 증가와 중첩되지 않는다.)"));
	}}},

	{270, {270, "나무 곡괭이",       { 158, 132, 83 },  false, TOOL,     USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("나무로 만든 기본적인 곡괭이이다. 돌과 석탄을 채굴할 수 있다.");
	}}},

	{272, {272, "돌 검",             { 120, 120, 120 }, false, WEAPON,   USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("단단한 돌로 만든 검이다. 나무 검보다 강한 위력을 지니고 있다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "보유 시 공격력이 20 증가한다."));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "(다른 검의 공격력 증가와 중첩되지 않는다.)"));
	}}},

	{274, {274, "돌 곡괭이",         { 120, 120, 120 }, false, TOOL,     USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("돌로 만든 곡괭이이다. 철 광석과 같은 단단한 광물을 채굴할 수 있다.");
	}}},

	{276, {276, "다이아몬드 검",     { 55, 224, 221 },  false, WEAPON,   USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("다이아몬드로 제작된 강력한 검이다. 뛰어난 위력을 자랑한다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "보유 시 공격력이 40 증가한다."));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "(다른 검의 공격력 증가와 중첩되지 않는다.)"));
	}}},

	{278, {278, "다이아몬드 곡괭이", { 55, 224, 221 },  false, TOOL,     USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("다이아몬드로 제작된 최고급 곡괭이이다. 거의 모든 광물을 빠르게 채굴할 수 있다.");
	}}},

	{280, {280, "막대기",            { 148, 107, 45 },  false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("도구와 무기를 제작하는 데 사용되는 기본 재료이다.");
	}}},

	{283, {283, "금 검",             { 255, 255, 70 },  false, WEAPON,   USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("금으로 제작된 날카로운 검이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "보유 시 공격력이 30 증가한다."));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "(다른 검의 공격력 증가와 중첩되지 않는다.)"));
	}}},

	{287, {287, "실",                { 255, 255, 255 }, false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("거미가 남긴 가느다란 섬유이다. 다양한 도구와 장비 제작에 사용된다.");
	}}},

	{289, {289, "화약",              { 150, 150, 150 }, false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("강한 폭발력을 지닌 가루이다. 특수한 아이템 제작에 사용된다.");
	}}},

	{319, {319, "돼지고기",          { 252, 118, 118 }, false, FOOD,     USE_BOTH,   useItem319, battleUseItem319, []() {
		TextBuffer.push("돼지에게서 얻을 수 있는 생고기이다. 구워 먹으면 더욱 맛있다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 40 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 40 회복한다."));
	}}},

	{320, {320, "익힌 돼지고기",     { 195, 171, 118 }, false, FOOD,     USE_BOTH,   useItem320, battleUseItem320, []() {
		TextBuffer.push("화로에 구워낸 돼지고기이다. 허기를 효과적으로 채울 수 있다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 100 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 100 회복한다."));
	}}},

	{328, {328, "부싯돌",            { 140, 140, 140 }, false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("자갈 속에서 발견되는 날카로운 광물 조각이다.");
		TextBuffer.push("불을 피우는 라이터로 제작할 수 있다.");
	}}},

	{346, {346, "낚싯대",            { 168, 127, 55 }, false,  TOOL,     USE_NORMAL, useItem346, nullptr, []() {
		TextBuffer.push("물고기를 낚을 수 있도록 하는 도구이다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "이 아이템은 사용해도 소모되지 않는다."));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "물 블럭과 인접한 상태에서만 사용 가능하다."));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "85% 확률로 대구를 1~3개 획득한다."));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "15% 확률로 몬스터와 전투를 시작한다."));
	}}},

	{349, {349, "대구",              { 229, 173, 124 }, false, FOOD,     USE_BOTH,   useItem349, battleUseItem349, []() {
		TextBuffer.push("물에서 잡을 수 있는 생선이다. 익혀 먹으면 더욱 맛있다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 30 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 30 회복한다."));
	}}},

	{350, {350, "익힌 대구",         { 182, 128, 82  }, false, FOOD,     USE_BOTH,   useItem350, battleUseItem350, []() {
		TextBuffer.push("노릇하게 구워낸 대구이다. 허기를 채우기에 좋은 음식이다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 80 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 80 회복한다."));
	}}},

	{352, {352, "뼈",                { 255, 255, 255 }, false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("스켈레톤이 남긴 뼈이다. 다양한 용도로 활용할 수 있다.");
	}}},

	{363, {363, "소고기",            { 233, 130, 112 }, false, FOOD,     USE_BOTH,   useItem363, battleUseItem363, []() {
		TextBuffer.push("소에게서 얻을 수 있는 생고기이다. 구워 먹으면 더욱 든든하다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 40 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 40 회복한다."));
	}}},

	{364, {364, "스테이크",          { 154, 105, 83 },  false, FOOD,     USE_BOTH,   useItem364, battleUseItem364, []() {
		TextBuffer.push("화로에 구워낸 소고기이다. 풍부한 영양으로 허기를 채워준다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 100 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 100 회복한다."));
	}}},

	{367, {367, "썩은 고기",         { 159, 147, 83 },  false, FOOD,     USE_BOTH,   useItem367, battleUseItem367, []() {
		TextBuffer.push("부패한 고기이다. 먹을 수는 있지만 몸에 좋지 않을 수도 있다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 20 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 20 회복한다."));
	}}},

	{368, {368, "엔더 진주",         { 43, 196, 166 },  false, ETC,      USE_BOTH,   useItem368, battleUseItem368, []() {
		TextBuffer.push("엔더맨이 떨어뜨리는 신비한 구슬이다. 특별한 힘이 담겨 있다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "상하좌우 중 방향을 하나 선택한다."));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "해당 방향으로 블럭을 처음으로 만나는 지점까지 순간이동 한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투에서 100% 확률로 도망친다."));
	}}},

	{369, {369, "블레이즈 막대",     { 238, 193, 65 },  false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("블레이즈가 남긴 뜨거운 막대이다. 강한 열기와 마력이 깃들어 있다.");
	}}},

	{370, {370, "가스트의 눈물",     { 197, 211, 208 }, false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("가스트가 흘린 희귀한 눈물이다. 신비로운 힘이 응축되어 있다.");
	}}},

	{377, {377, "블레이즈 가루",     { 238, 193, 65 },  false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("블레이즈 막대를 가공하여 만든 가루이다. 뜨거운 에너지가 응축되어 있다.");
	}}},

	{378, {378, "마그마 크림",       { 205, 109, 37 },  false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("마그마 큐브가 남긴 끈적한 덩어리이다. 뜨거운 열기가 느껴진다.");
	}}},

	{381, {381, "엔더의 눈",         { 103, 153, 125 }, false, SPECIAL,  USE_NORMAL, useItem381, nullptr, []() {
		TextBuffer.push("엔더 진주와 블레이즈 가루로 만든 신비한 구슬이다.");
		TextBuffer.push("강한 인도자의 힘이 깃들어 있다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용하려면 이 아이템을 12개 이상 보유해야 한다."));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 시 이 아이템을 12개 소모한 후 [엔드]로 이동한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "<!> 경고 <!>"));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "[엔드] 로 가면 사망하거나, \"엔더 드래곤\"을 처치하기 전에는"));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "기존 월드로 돌아올 수 없습니다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "충분한 준비를 갖추고 진입하십시오."));
	}}},

	{423, {423, "양고기",            { 235, 68, 68 },   false, FOOD,     USE_BOTH,   useItem423, battleUseItem423, []() {
		TextBuffer.push("양에게서 얻을 수 있는 생고기이다. 구워 먹으면 더욱 맛있다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 40 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 40 회복한다."));
	}}},

	{424, {424, "익힌 양고기",       { 135, 94, 76 },   false, FOOD,     USE_BOTH,   useItem424, battleUseItem424, []() {
		TextBuffer.push("화로에 구워낸 양고기이다. 부드러운 식감으로 허기를 채워준다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "체력을 100 회복한다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "체력을 100 회복한다."));
	}}},

	{505, {505, "철 원석",           { 197, 167, 144 }, false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("철 광석을 채굴하여 얻은 원석이다. 제련하면 철괴를 만들 수 있다.");
	}}},

	{506, {506, "금 원석",           { 255, 255, 70 },  false, MATERIAL, USE_NONE,   nullptr, nullptr, []() {
		TextBuffer.push("금 광석을 채굴하여 얻은 원석이다. 제련하면 금괴를 만들 수 있다.");
	}}},

	{1000, {1000, "몬스터 미끼",     { 253, 138, 105 }, false, TOOL,     USE_NORMAL, useItem1000, nullptr, []() {
		TextBuffer.push("몬스터를 유인하는 특수한 미끼이다.");
		TextBuffer.push("주변의 적들이 냄새를 맡고 몰려든다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "현재 시간 및 위치에서 출현하는 몬스터와 전투를 시작한다."));
	}}},

	{1001, {1001, "대장장이 모루",   { 222, 64, 24 }, false,  SPECIAL,  USE_NORMAL, useItem1001, nullptr, []() {
		TextBuffer.push("무기를 더욱 강력하게 단련할 수 있는 대장장이의 모루이다.");
		TextBuffer.push(ColorText({ 120, 120, 255 }, "사용 효과 : "));
		TextBuffer.push(ColorText({ 120, 120, 255 }, "기존 무기보다 더욱 강력한 무기를 제작할 수 있는 UI를 연다."));
	}}},

	{1002, {1002, "네더라이트 파편",   { 110, 110, 110 }, false,  MATERIAL,  USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("고대 잔해를 제련하여 얻은 희귀한 파편이다.");
		TextBuffer.push("최상급 장비 제작에 사용된다.");
	}}},

	{1003, {1003, "네더라이트 주괴",   { 110, 110, 110 }, false,  MATERIAL,  USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("네더라이트 파편과 금을 조합하여 만든 최고급 금속 주괴이다.");
	}}},

	{1004, {1004, "방패",             { 148, 107, 45 }, false,  WEAPON,  USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("적의 공격을 막아내기 위해 사용하는 방어구이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "방어력 +50 "));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "특수 효과 [ 저희는 그런 것 받지 않습니다 ] : "));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "이 아이템을 보유 시, 각 전투에서 받는 첫 번째 공격의"));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "피해를 무효화한다."));
	}}},

	{2000, {2000, "흑암",            { 110, 110, 110 }, true, BLOCK,    USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("빛조차 삼켜 버릴 듯한 깊은 어둠이 응축된 물질이다.");
	}}},

	{2001, {2001, "뒤틀린 네사체",   { 60, 210, 190 }, true,  BLOCK,    USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("뒤틀린 숲을 덮고 있는 청록빛 흙이다. 차가운 생명력이 스며 있다.");
	}}},

	{2002, {2002, "뒤틀린 자루",     { 80, 240, 220 }, true,  BLOCK,    USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("뒤틀린 균류의 줄기 부분이다. 청록빛 결이 독특한 목재 자원이다.");
	}}},

	{2003, {2003, "뒤틀린 사마귀 블록", { 80, 240, 220 }, true,  BLOCK,    USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("뒤틀린 균류에서 자라는 사마귀 덩어리이다. 선명한 청록빛을 띤다.");
	}}},

	{2004, {2004, "네더 금 광석",    { 255, 205, 45 }, true,  BLOCK,    USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("네더에서 발견되는 금 광석이다. 채굴하면 금을 얻을 수 있다.");
	}}},

	{2005, {2005, "고대 잔해",       { 185, 110, 70 }, true,    BLOCK,     USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("아주 오래전부터 네더 깊은 곳에 묻혀 있던 희귀한 잔해이다.");
		TextBuffer.push("강력한 금속의 흔적을 품고 있다.");
	}}},

	{3000, {3000, "네더라이트 검",    { 110, 110, 110 }, true,    STRONG_SWORD,  USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("네더라이트로 제작된 최강의 검이다. 막강한 위력과 뛰어난 내구성을 지녔다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "보유 시 공격력이 80 증가한다."));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "(다른 검의 공격력 증가와 중첩되지 않는다.)"));
	}}},

	{3001, {3001, "망령의 활",       { 68, 156, 137 }, true,    STRONG_BOW,  USE_BATTLE, nullptr, battleUseItem3001, []() {
		TextBuffer.push("망령의 기운이 깃든 활이다. 시위를 당길 때마다 서늘한 기척이 감돈다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "특수 효과 [ 망령 화살 ] : "));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "이 아이템을 보유 시, 전투에서 공격하면 공격력의 100% 만큼"));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "추가 피해를 입힌다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "전투 사용 효과 : "));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "이 아이템은 사용해도 소모되지 않는다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "화살 하나를 소모하여 적에게 공격력의 200% 의 피해를 입힌다."));
		TextBuffer.push(ColorText({ 255, 120, 120 }, "적이 피해를 입지 않은 상태일 경우 대신 400% 의 피해를 입힌다."));
	}}},

	{3002, {3002, "화염 건틀릿",     { 232, 49, 16 }, true,    STRONG_ACC, USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("타오르는 불꽃의 힘이 깃든 건틀릿이다.");
		TextBuffer.push("강력한 열기를 방출할 수 있다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "특수 효과 [ 열기 방출 ] : "));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "이 아이템을 보유 시, 전투에서 공격하면 적에게 2턴간"));
		TextBuffer.push(ColorText(BuffColor[FLAME], "불타오름") + ColorText({ 255, 255, 120 }, " 효과를 적용한다."));
	}}},

	{3003, {3003, "지옥의 심장",     { 204, 57, 57 }, true,    STRONG_ACC, USE_NONE, nullptr, nullptr, []() {
		TextBuffer.push("진홍빛 기운을 뿜어내는 심장이다.");
		TextBuffer.push(ColorText({ 255, 255, 120 }, "특수 효과 [ 수혈 ] : "));
		TextBuffer.push(ColorText({ 255, 255, 120 }, "이 아이템을 보유 시, 전투에서 어떤 종류든 피해를 입으면"));
		TextBuffer.push("입은 피해의 30% 를 회복한다.");
	}}},

	
};