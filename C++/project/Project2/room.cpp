#include "room.h"
#include <iostream>
using namespace std;

Room::Room()
{
    type = NORMAL_ROOM;
};

Room::~Room() {};

void Room::setType(int t)
{
	type = t;
}

int Room::getType()
{
    return type;
}

int Room::update(Player& player)
{
    switch(type){
        case NORMAL_ROOM:
            cout << "You went into a normal room, nothing happened here." << endl;
            break;
        case MONSTER_ROOM:
            cout <<"You went into a room with monster, and you died..." << endl;
            cout << "Game Failed!" << endl;
            return 1;
        case PRINCESS_ROOM:
            if(!player.isWithPrincess())
                cout << "Yes! You have found the princess." << endl;
                player.foundPrincess();
                cout << "Now take her back to the lobby!" << endl;
            break;
        case LOBBY_ROOM:
            cout << "Welcome to the lobby." << endl;
            if(player.isWithPrincess()){
                cout << "Yes! You made it! You successfully rescued the princess out of the maze!" << endl;
                cout << "Game Success!" << endl;
                return 1;
            }
            break;
    }
	return 0;
};
