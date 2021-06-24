#include <iostream>
#include <string>
#include "player.h"
#include "game.h"
using namespace std;

Player::Player(int PosX, int PosY) : x(PosX), y(PosY), withPrincess(0) {};

Player::~Player() {};

bool Player::isWithPrincess()
{
    return withPrincess;
}

void Player::foundPrincess()
{
    withPrincess = 1;
}

int Player::getX()
{
    return x;
}

int Player::getY()
{
    return y;
}

void Player::update()
{
    int count = 0;
    string word = "";
    if (x > 0)
        ++count, word += "east ";
    if (y > 0)
        ++count, word += "up ";
    if (x < size - 1)
        ++count, word += "west ";
    if (y < size - 1)
        ++count, word += "down ";
    cout<<"There are " << count << " exits as: " << word << "\nPlease choose a direction?" << endl;
}

int Player::move(int offsetX, int offsetY)
{
    if (x + offsetX < 0 || x + offsetX >= size)
        return 1;
    if (y + offsetY < 0 || y + offsetY >= size)
        return 1;

    x += offsetX;
    y += offsetY;
    return 0;
}
