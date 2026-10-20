CXX = g++
CC = gcc

GLFW_INC = lib/glfw/include
GLFW_LIB = lib/glfw/build/src

IMGUI_INC = lib/imgui
IMGUI_SRC = $(IMGUI_INC)/imgui.cpp $(IMGUI_INC)/imgui_draw.cpp \
			$(IMGUI_INC)/imgui_tables.cpp $(IMGUI_INC)/imgui_widgets.cpp \
			$(IMGUI_INC)/backends/imgui_impl_glfw.cpp \
			$(IMGUI_INC)/backends/imgui_impl_opengl3.cpp \
			$(IMGUI_INC)/imgui_demo.cpp
IMGUI_OBJ = $(IMGUI_SRC:.cpp=.o)

CXXFLAGS = -std=c++17 -Wall -I$(GLFW_INC) -I$(IMGUI_INC) -Isrc -I.
CFLAGS = -Wall -I.
LDFLAGS = -L$(GLFW_LIB) -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework QuartzCore

TARGET = fps

all: $(TARGET)

$(TARGET): main.o glad.o $(IMGUI_OBJ)
	$(CXX) main.o glad.o $(IMGUI_OBJ) -o $(TARGET) $(LDFLAGS)

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o main.o

glad.o: src/glad.c
	$(CC) $(CFLAGS) -c src/glad.c -o glad.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f main.o glad.o $(TARGET)

run: $(TARGET) 
	./$(TARGET)


.PHONY: all clean