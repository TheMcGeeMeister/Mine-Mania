#pragma once
#include <string>
#include <windows.h>
#include "Turret.h"
#include "Position.h"
#include "UserInterface.h"
#include "HealthComponent.h"
#include "PlayerStatComponent.h"
#include "SoundPlayer.h"

using namespace std;

class Display;

class Player : public Entity
{
    public:
        Player();
		Player(Player&);
        virtual ~Player();

		void stopSounds();

        /* Getters */
		//////////////////////////////////
        int getGoldAmount();
        int getManaAmount();
        int getMaxGoldAmount();
        int getMaxManaAmount();
		int getAmmoAmount();
		int getHealth();
		int getMaxHealth();
		int getLevel();
		int getMaxLevel();
		int getExp();
		int getMaxExp();
        string getName();
		Position getSpawnPos();
		UserInterface& getUIRef();
		HealthComponent& getHealthRef();
		//////////////////////////////////

		/* Colors */
		//////////////////////////////////
		WORD getClaimedColor();
		void setClaimedColor(WORD color);
		//////////////////////////////////

        /* Gold Related */
		//////////////////////////////////
        void addGold(int amount);
        void removeGold(int amount);
		void setGoldAmount(int amount);
		void setMaxGoldAmount(int amount);
		//////////////////////////////////

        /* Setters */
		//////////////////////////////////
        void setManaAmount(int amount);
		void setMaxManaAmount(int amount);
		void setAmmoAmount(int amount);
        void setName(string name);
		void setSpawnPos(Position pos);
		void setHandPos(Position pos); /* Changes variable without changing map */
		void setHandPosNoUpdate(Position pos);
		void setHealth(int amount);
		void setMaxHealth(int amount);
		//////////////////////////////////

		/* Stats*/
		//////////////////////////////////
		//void damage(int amount);
		void damageS(int amount);
		void heal(int amount);
		void healS(int amount);
		//////////////////////////////////

		/* Hand */
		//////////////////////////////////
		void moveHandUp(Display& game);
		void moveHandDown(Display& game);
		void moveHandLeft(Display& game);
		void moveHandRight(Display& game);
		void moveHand(DIRECTION direction);
		void mineUp(Display& game);
		void mineDown(Display& game);
		void mineLeft(Display& game);
		void mineRight(Display& game);
		void mine(DIRECTION direction);
		void forceHandPosition(Position newPos, Display& game); /* Updates Position on map, and changes variable :: Deprecated Use forceHandPositon(Position);*/
		void forceHandPosition(Position newPos); /*Also updates position on the map*/
		void claimOnHand();
		void switchMode();
		void switchModeTo(int mode);
		void updateHandPos();
		void disableMovementFor(int time);
		void knockbackTo(DIRECTION direction, int amount);
		Position getHandPosition();
		//////////////////////////////////

		/* Object Place */
		//////////////////////////////////
		void placeObject(uint16_t objectID);
		void spawnTurret(Position pos);

		void purchaseTurret();
		void purchaseBullet();
		//////////////////////////////////

		/* Server */
		//////////////////////////////////
		void updatePosition();
		//////////////////////////////////

		/* Passive Gold */
		//////////////////////////////////
		void isGoldPassive(bool);
		bool isGoldPassive();
		void addPassiveGold(int amount);
		void removePassiveGold(int amount);
		//////////////////////////////////

		void reset();
		void updateMiningUI();

		//void operator=(Player& player);

		/* Serialize / Deserialize */
		//////////////////////////////////
		void serialize(fstream& file);
		void serialize(stringstream& file);
		void deserialize(fstream& file);
		void deserialize(stringstream& file);
		//////////////////////////////////

		virtual void update();
		virtual bool hasComponent(int id);
		virtual bool isKilled();
		virtual void kill();
		virtual void clean();
		virtual bool damage(int damage, string name = "", bool server=false);
		virtual void setPos(Position pos); // Calls forceHandPosition(Position)
		virtual void updateOverlay();
		virtual void updateID(); // Not used for player
		virtual void send();
		virtual void render();
		virtual void activate(Player* player);
		virtual Position getPos();

		void killS();


    protected:

    private:
        int goldAmount_;
        int maxGoldAmount_;
        int manaAmount_;
        int maxManaAmount_;
		int handMode_;
		int mineProgress_;
		int ammo_;
		int passiveGoldIncrease_;
		double baseDamage_;

		WORD claimedColor_;

		HealthComponent health;
		PlayerStatComponent stats;
		SoundPlayer turret_sound;
		SoundPlayer repair_sound;
		SoundPlayer mine_sound;

		bool moved_;
		bool mined_;
		bool isMining_;
		bool isDead_;
		bool isLocal_;
		bool isGoldPassive_;

		Position handPos;
		Position mineUIPos;
		Position spawnPos;

        string name_;

		UserInterface UI;

		Timer movementTimer_;
		Timer mineTimer_;
		Timer shootTimer_;
		Timer goldCooldown_;
		Timer expCooldown_;

		void Log(std::string);

};

namespace Common
{
	extern Player CreatePlayer(Position pos, std::string name, WORD claim_color, bool isLocal = false);
}
