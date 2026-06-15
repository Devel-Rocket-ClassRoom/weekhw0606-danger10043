#pragma once

class World;
enum WorldType;

class WorldGenerator {
public:
	static void WorldGenerate(World& world);

	static void PlantTree(World& targetWorld, WorldType worldType, int worldPosR, int worldPosC, int StemBlockID, int LeafBlockID);

	static void PlantOre(World& targetWorld, WorldType worldType, int worldPosR, int worldPosC, int blockID);

	static void PlantMass(World& targetWorld, WorldType worldType, int worldPosR, int worldPosC, int blockID, int Size);
};