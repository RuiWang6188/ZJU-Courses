CC = g++
OBJ = main.o player.o room.o game.o 

.PHONY: target clean

target: project2
# mkdir build && cd build


project2: $(OBJ)
	$(CC) $(OBJ) -o project2 

main.o: main.cpp game.h room.h player.h 
	$(CC) -c main.cpp

game.o: game.cpp room.h player.h 
	$(CC) -c game.cpp

player.o: player.cpp game.h
	$(CC) -c player.cpp 

room.o: room.cpp player.h
	$(CC) -c room.cpp

clean:
	rm -f *.o
	rm -f project2
