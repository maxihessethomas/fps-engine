CXX = g++
CC = gcc

GLFW_INC = lib/glfw/include
GLFW_LIB = lib/glfw/build/src

CXXFLAGS = -std=c++17 -Wall -I$(GLFW_INC) -I.
CFLAGS = -Wall -I.
LDFLAGS = -L$(GLFW_LIB) -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework QuartzCore

TARGET = fps

all: $(TARGET)

$(TARGET): main.o glad.o
	$(CXX) main.o glad.o -o $(TARGET) $(LDFLAGS)

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o main.o

glad.o: src/glad.c
	$(CC) $(CFLAGS) -c src/glad.c -o glad.o

clean:
	rm -f main.o glad.o $(TARGET)

run: $(TARGET) 
	./$(TARGET)


.PHONY: all clean