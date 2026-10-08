CXX = clang++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

game: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp $(LDFLAGS) -o game
