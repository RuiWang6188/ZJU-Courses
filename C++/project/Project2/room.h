#ifndef ROOM_H_
#define ROOM_H_
#include "player.h"

const int NORMAL_ROOM 	= 0;
const int MONSTER_ROOM	= 1;
const int PRINCESS_ROOM	= 2;
const int LOBBY_ROOM 	= 3;

class Room
{
private:
	int type;
public:

	Room();
	~Room();
	int update(Player &[]);
	void setType(int type);
	int getType();

};


#endif
