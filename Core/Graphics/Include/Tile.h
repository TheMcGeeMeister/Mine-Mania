#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <windows.h>
#include "Position.h"
#include "HealthComponent.h"

class Player;

using namespace std;

class Tile
{
    public:
        Tile();
		Tile(Position pos);
        virtual ~Tile();
        char getGraphic() const;

        bool isWall() const;
		bool isFortified() const;
        bool isClaimed() const;
		bool isClaimedBy(string name) const;
        bool isDestructable() const;
        bool isWalkable() const;
		bool isHealthFull() const;
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
		WORD getRenderColor() const;
        Position getPos() const;

        void setColor(WORD color);
		void setClaimColor(WORD color);
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
        void claim(int amount, string claimer, WORD color);
        void forceClaim(string claim);
		void fortify(int amount);
        void hasGold(bool hasGold);
		void heal(int amount);
        bool mine(int damage, Player&);
		void removeOverlay();
		void updateOverlay(bool enabled, char graphic);
		void updateOverlayS(bool enabled, char graphic); // Updates Overlay without sending to server
		void updateServer();
		void updateFortify();

      void serialize(fstream& stream);
	  void serialize(ofstream& stream);
	  void serialize(stringstream& stream);
      void deserialize(fstream& stream);
	  void deserialize(ifstream& stream);
	  void deserialize(stringstream& stream);

	  void update();
    protected:

    private:
		HealthComponent health;
        int claimedPercentage_;
        int gold_;
		int objectid_;
		int fortifyAmount_;
        char graphic_;
		char overlayGraphic_;
        string claimedBy_;
        string curBeingClaimedBy_;
        WORD color_;
        WORD background_;
		WORD claimColor_;
        bool isClaimable_;
        bool isWalkable_;
        bool isDestructable_;
        bool isWall_;
		bool isFortified_;
        bool isClaimed_;
        bool hasGold_;
		bool overlayEnabled_;
        Position pos_;
};