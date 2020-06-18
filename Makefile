LIBS=-lSDL2

minesweeper: main.cpp
    $(CXX) main.cpp render.cpp game.cpp -o minesweeper $(LIBS)