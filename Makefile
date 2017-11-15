# Name of the executable created
Target = bresenham
# Main file
Sources = src/main.cpp

# Flags
CPPFLAGS = -I/usr/include
CXXFLAGS = -O3 -ffast-math -Wall
LDFLAGS  = -lGL -lGLU -lglut
DESTPATH = bin/$(Target)

# this is the same as typing
# g++ -I/usr/include -O3 -ffast-math main.cpp -o .bin/IrrlichtTest -lGL -lGLU -lglut
all:
	$(warning Building...)
	mkdir -p ./bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(Sources) -o $(DESTPATH) 