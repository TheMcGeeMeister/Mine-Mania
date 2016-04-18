#pragma once

#include <Turret.h>
#include <string>
#include <Position.h>
#include <UserInterface.h>
#include <HealthComponent.h>

using namespace std;

class Display;

class Player : public Entity
{
    public:
        Player();
		Player(Player&);
        virtual ~Player();

        /* Getters */
		//////////////////////////////////
        int getGoldAmount();
        int getManaAmount();
        int getMaxGoldAmount();
        int getMaxManaAmount();
		int getAmmoAmount();
		int getHealth();
		int getMaxHealth();
        string getName();
		Position getSpawnPos();
		UserInterface& getUIRef();
		HealthComponent& getHealthRef();
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
		void mineUp(Display& game);
		void mineDown(Display& game);
		void mineLeft(Display& game);
		void mineRight(Display& game);
		void mine(DIRECTION direction);
		void attack(DIRECTION direction);
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

		void reset();
		void updateMiningUI();

		void operator=(Player& player);

		/* Serialize / Deserialize */
		//////////////////////////////////
		void serialize(fstream& file);
		//void serialize(ofstream& file);
		void serialize(stringstream& file);
		void deserialize(fstream& file);
		//void deserialize(ifstream& file);
		void deserialize(stringstream& file);
		//////////////////////////////////

		virtual void update();
		virtual bool hasComponent(int id);
		virtual bool isKilled();
		virtual void kill();
		virtual void clean();
		virtual void damage(int damage, string name = "");
		virtual void setPos(Position pos); // Calls forceHandPosition(Position)
		virtual void updateOverlay();
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

		HealthComponent health;

		bool moved_;
		bool mined_;
		bool isMining_;
		bool isSoundPlaying_;
		bool isDead_;
		bool isLocal_;

		Position handPos;
		Position mineUIPos;
		Position spawnPos;

        string name_;

		UserInterface UI;

		Timer miningStopSound;
		Timer movementTimer_;
		Timer shootTimer_;

		void Log(std::string);

};



Player CreatePlayerAt(Position pos, std::string name);
Player CreatePlayerAndSendAt(Position pos, std::string name, list<int> toSend);
