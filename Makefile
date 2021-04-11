EXECUTABLE = minesweeper
SRCDIR = src
OBJDIR = obj
INCDIR = include
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SRC))
INC = $(wildcard $(INCDIR)/*.hpp)
BUILT_OBJ = $(wildcard $(OBJDIR)/*.o)
CXXFLAGS = -O3 -std=c++11 -Werror -Wall -Wextra -pedantic

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) -o $(EXECUTABLE) $(OBJ) -lSDL2 $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INC)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

run:
	@./$(EXECUTABLE)

clean:
	rm $(BUILT_OBJ) $(EXECUTABLE)
