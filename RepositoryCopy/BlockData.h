#pragma once
#include <string>
#include <map>

class Block;
class World;

typedef void(*RenderFunc)(const Block&, const World&);

struct BlockData {

    int blockID;
    std::string name;
    int hardness;
    int dropItemID;
    bool solid;
    RenderFunc renderFunc;
};

extern std::map<int, BlockData> BLOCK_DATABASE;
