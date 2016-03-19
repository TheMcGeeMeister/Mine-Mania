#pragma once
#include <fstream>

class TileNet
{
public:
	TileNet();
	~TileNet();
	void serialize(std::fstream& file);
	void deserialize(std::fstream& file);
private:
	int health_;
	int maxHealth_;
	int claimedPercentage_;
	int gold_;
	char graphic_;
	std::string claimedBy_;
	std::string curBeingClaimedBy_;
	int color_;
	int background_;
	bool isClaimable_;
	bool isWalkable_;
	bool canFlyOver_;
	bool isDestructable_;
	bool isWall_;
	bool isFortified_;
	bool isClaimed_;
	bool hasGold_;
	int pos_x;
	int pos_y;
};

