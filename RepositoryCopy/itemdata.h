#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>

class Item;
class World;
class Player;
class Monster;

typedef std::function<void()> ItemInfoFunc;
typedef void(*ItemuseFunc)(Item&, World&);
typedef void(*ItemBattleuseFunc)(Player&, Monster&);

extern std::map<std::vector<std::pair<int, int>>, std::pair<int, int>> RECIPEBOOK;

extern std::map<std::string, int> ItemName2ItemID;

enum useFlag {
	USE_NONE = 0,
	USE_NORMAL = 1 << 0,
	USE_BATTLE = 1 << 1,
	USE_BOTH = USE_NORMAL | USE_BATTLE,
};

enum ItemType {
    WEAPON,
    TOOL,
    BLOCK,
    FOOD,
    MATERIAL,
    SPECIAL,
    ETC,
    STRONG_SWORD,
    STRONG_BOW,
    STRONG_ACC,
};

struct ItemData
{
    int itemID;
    std::string name;
    std::vector<int> nameColor;
    bool isBlock;
    ItemType type;
    int useFlag;
    ItemuseFunc useFunc;
    ItemBattleuseFunc battleFunc;
    ItemInfoFunc infoFunc;
};

extern std::map<int, ItemData> ITEM_DATABASE;