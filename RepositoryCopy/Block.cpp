#include "Block.h"
#include "BlockData.h"
#include "colorprint.h"
#include "basic.h"

// * [블럭추가] 시 printBlock 수정할 것.
void Block::printBlock(const Block& block, const World& world) {
	BLOCK_DATABASE[block.itemID].renderFunc(block, world);
}

void Block::printBlock(const World& world) {
	BLOCK_DATABASE[this->itemID].renderFunc(*this, world);
}

int Block::getHardness() {
	return BLOCK_DATABASE[itemID].hardness;
}

int Block::getItem() {
	return BLOCK_DATABASE[itemID].dropItemID;
}