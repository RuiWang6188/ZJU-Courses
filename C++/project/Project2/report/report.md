# Report of Project2

## Requirement

Design a CLI game *Adventure*, in which the player explore the rooms of a castle to find the princess and take her out of it. But when he meet the monster, the game will be over. The room of the princess and the monster is randomly generated.

There're a few restrictions:

- At least three different kinds of room;
- At least five rooms;
- The room with monster or princess is randomly set.

## Implementation

### Player Class

First, I design the maze to be a $3\times 3$ matrix. The player class includes these properties. `x` and `y` indicates the location of the player in the maze, and `withPrincess` records whether the player has found the princess.

```c++
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
```

For the member functions, the usage of each function is listed below:

- `Player()`: constructor, where `x` and `y` is the initial location of the player
- `~Player()`: destructor
- `isWithPrincess()`: get the status of the member variable `withPrincess`
- `foundPrincess()`: set the member variable `withPrincess` with true, used when the player first get in the room of princess.
- `getX()` and `getY()`: return the location of the player
- `update()`: traverse the 4 directions and tell the player which direction(s) he can go next. 
-  `move()`: move the location of the player (i.e. set `x` and `y`)

### Room Class

In the `room.h`, I specified the room types, including `NORMAL_ROOM`, `MONSTER_ROOM`, `PRINCESS_ROOM` and `LOBBY_ROOM`. Note that `LOBBY_ROOM` is the entrance of the player as well as the destination when the player found the princess.

 The room class is defined as follows, the member variable `type` is used to indicate the room type.

```c++
class Room
{
private:
	int type;
public:
	Room();
	~Room();
	int update(Player &p);
	void setType(int type);
	int getType();

};
```

For the member functions, the usage of each function is listed below:

- `Room()`: constructor
- `~Room()`: destructor
- `setType()`: set the type of the room, used when initializing the maze. 
- `getType()`: return the type of the room.
- `update()`:  used to check what will happen in the room of the player, and print the result on the screen. There're a few possibilities
  - NORMAL_ROOM: nothing will happen
  - MONSTER_ROOM: player meet the monster, game failed
  - PRINCESS_ROOM: player meet the princess, update the player's status, and output some message
  - LOBBY_ROOM: output the player's location, check if the player has found the princess. If so, game succeeded.

### Game Class

The game class is defined as follows:

```C++
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
```

It generates the maze (i.e. setting each room type), and control the main game flow logic. You can view the code for details.

##  Compile and Run

To compile and run the game, read the README.md, it includes much more detailed instructions.