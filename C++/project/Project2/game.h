#ifndef CASTLE_H_
#define CASTLE_H_

#include "room.h"
#include <vector>
using namespace std;

const int size = 3;

class Game
{
private:
	vector<vector<Room>> maze;
	void randomGenerate(int type);
public:
	Game();
	virtual ~Game();
	void gaming();
};


#endif
