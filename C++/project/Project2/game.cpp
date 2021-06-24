#include "game.h"
#include "room.h"
#include "player.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;
Game::Game()
{
    srand(time(NULL));
	maze.resize(size);
	for (vector < vector <Room> > :: iterator p = maze.begin(); p != maze.end(); p++)
    {
        for (int i = 0; i != size; ++i)
        {
            Room room;
            p->push_back(room);
        }
    }
    randomGenerate(MONSTER_ROOM);
    randomGenerate(PRINCESS_ROOM);
    maze[1][1].setType(LOBBY_ROOM);
}

Game::~Game() {};

void Game::randomGenerate(int type)
{
    while (1)
    {
        int x = rand() % size;
        int y = rand() % size;
        if (maze[x][y].getType() == NORMAL_ROOM)
        {
            maze[x][y].setType(type);
            break;
        }
    }
}

void Game::gaming()
{
    Player player(1, 1);
    while (1)
    {
        cout << endl;
        int x = player.getX();
        int y = player.getY();
        if (maze[x][y].update(player)){
            cout<<"Game Over!\n";
            return ;
        }

        player.update();
        int offsetX = 0, offsetY = 0;
        while(1){
            cout<<"Enter your command: ";
            cout << "go ";
            string pos;
            cin >>  pos;
            if (pos == "up")
                offsetY = -1;
            else if (pos == "down")
                offsetY = 1;
            else if (pos == "east")
                offsetX = -1;
            else if (pos == "west")
                offsetX = 1;
            else {
                cout << "Invalid direction, please input again!" << endl;
                continue;
            }
            if (player.move(offsetX, offsetY)){
                cout << "Invalid direction, please input again!" << endl;
                continue;
            }
            if (player.getX() == x && player.getY() == y){
                cout<<"You went to a wrong direction, please input!" << endl;
                continue;
            }
            break;
        }
    }

}
