#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <Position.h>
#include <HealthComponent.h>

class Player;

using namespace std;


class Tile
{
    public:
        Tile();
        virtual ~Tile();
        char getGraphic() const;

        bool isWall() const;
		bool isFortified() const;
        bool isClaimed() const;
		bool isClaimedBy(string name) const;
        bool isDestructable() const;
        bool isWalkable() const;
        bool hasGold() const;
		bool hasOverlay() const;
        double getHealth() const;
        double getMaxHealth() const;
        int getClaimedPercentage() const;
        int getGold() const;
		int getObjectId() const;
		char getOverlayGraphic() const;
        string getClaimedBy() const;
        string getCurBeingClaimedBy() const;
        WORD getColor() const;
        WORD getBackground() const;
        WORD getAttribute() const;
        Position getPos() const;

        void setColor(WORD color);
        void setBackground(WORD background);
        void setGraphic(char graphic);
        void setGoldAmount(int amount);
        void setHealth(double health);
        void setMaxHealth(double maxHealth);
        void setPos(Position position);
        void setIsWall(bool);
        void setIsWalkable(bool);
        void setIsDestructable(bool);
        void setIsClaimed(bool);
        void setIsClaimed(bool isClaimed, string claimedBy);
        void setIsClaimable(bool);
        void setCanFlyOver(bool);
        void setClaimedBy(string);
		void setObjectId(int id);
		void setOverlayEnabled(bool);
		void setOverlayGraphic(char);
        void isWall(bool);
        void isWalkable(bool);
		void isFortified(bool);
        void isClaimed(bool);
        void isClaimed(bool, string claimedBy);
        void isClaimable(bool);
        void isDestructable(bool);
        void decrementHealth(int amount);
        void incrementHealth(int amount);
        void claim(int amount, string claimer);
        void forceClaim(string claim);
        void hasGold(bool hasGold);
        void mine(int damage, Player&);
		void removeOverlay();

        void serialize(fstream& file);
		void serialize(ofstream& file);
		void serialize(stringstream& file);
		string serialize(bool);
        void deserialize(fstream& file);
		void deserialize(ifstream& file);
		void deserialize(stringstream& data);

		void update();
    protected:

    private:
		HealthComponent health;
        int claimedPercentage_;
        int gold_;
		int objectid_;
        char graphic_;
		char overlayGraphic_;
        string claimedBy_;
        string curBeingClaimedBy_;
        WORD color_;
        WORD background_;
        bool isClaimable_;
        bool isWalkable_;
        bool canFlyOver_;
        bool isDestructable_;
        bool isWall_;
		bool isFortified_;
        bool isClaimed_;
        bool hasGold_;
		bool overlayEnabled_;
        Position pos_;
};
