#pragma once

#include <string>
#include <Position.h>
#include <UserInterface.h>

using namespace std;

class Display;

enum ObjectID
{
	NORMTURRETID
};

class Underlord
{
    public:
        Underlord();
        virtual ~Underlord();

        /* Getters */
		//////////////////////////////////
        int getGoldAmount();
        int getManaAmount();
        int getMaxGoldAmount();
        int getMaxManaAmount();
        string getName();
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
        void setName(string name);
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
		void forceHandPosition(Position newPos, Display& game);
		void claimOnHand();
		Position getHandPosition();
		//////////////////////////////////

		/* Object Place */
		//////////////////////////////////
		void placeObject(uint16_t objectID);
		//////////////////////////////////

		void reset();
		void updateMiningUI();

		/* Serialize / Deserialize */
		//////////////////////////////////
		void serialize(fstream& file);
		void serialize(ofstream& file);
		void deserialize(fstream& file);
		void deserialize(ifstream& file);
		void deserialize(stringstream& file);
		//////////////////////////////////


    protected:

    private:
        int goldAmount_;
        int maxGoldAmount_;
        int manaAmount_;
        int maxManaAmount_;

		bool moved_;
		bool mined_;

		Position handPos;
		Position mineUIPos;

        string name_;

		UserInterface UI;

};
