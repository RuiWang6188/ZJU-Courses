#ifndef PLAYER_H_
#define PLAYER_H_

class Player
{
private:
    int x, y;
    bool withPrincess;
public:
    Player(int x, int y);
    virtual ~Player(void);
    bool isWithPrincess(void);
    void foundPrincess(void);
    int getX(void);
    int getY(void);
    void update(void);
    int move(int offsetX, int offsetY);

};

#endif
