EXECUTABLE = minesweeper
SRCDIR = src
OBJDIR = obj
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SRC))
CXXFLAGS = -O3 -std=c++11 -Werror -Wall -Wextra -pedantic

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) -o $(EXECUTABLE) $(OBJ) -lSDL2 $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

run:
	./$(EXECUTABLE)

clean:
	rm $(OBJ) $(EXECUTABLE)
