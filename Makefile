EXECUTABLE = minesweeper
SRCDIR = src
OBJDIR = obj
INCDIR = include
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SRC))
INC = $(wildcard $(INCDIR)/*.hpp)

CXXFLAGS = -O3 -std=c++11 -Werror -Wall -Wextra -pedantic

ifeq ($(OS),Windows_NT)
	LIBS =-lmingw32 -lSDL2main
	CXXFLAGS +=-mwindows
endif

LIBS +=-lSDL2

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) -o $(EXECUTABLE) $(OBJ) $(LIBS) $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INC) | obj
	$(CXX) $(CXXFLAGS) -o $@ -c $<

obj:
ifeq ($(OS),Windows_NT)
	@mkdir $@
else
	@mkdir -p $@
endif

run:
	@./$(EXECUTABLE)

clean:
ifeq ($(OS),Windows_NT)
	del /f /q $(subst /,\,$(OBJ)) $(EXECUTABLE).exe
else
	rm -f $(OBJ) $(EXECUTABLE)
endif
