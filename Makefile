CFLAGS = -ggdb3 -Iinc `pkg-config --cflags lvgl`
LDFLAGS = `pkg-config --libs lvgl sdl2` -llvgl_thorvg -lm -pthread

SRC = $(shell find src -type f -name "*.c")
CPP_SRC = $(shell find src -type f -name "*.cpp")
OBJ = $(patsubst %.c,%.o,$(SRC))
CPP_OBJ = $(patsubst %.cpp,%.o,$(CPP_SRC))
DEP = $(patsubst %.o,%.d,$(OBJ) $(CPP_OBJ))

TARGET = a.out

all: $(OBJ) $(CPP_OBJ)
	g++ -o $(TARGET) $^ $(LDFLAGS)

clean:
	rm -rf $(TARGET) $(OBJ) $(CPP_OBJ) $(DEP)

%.o: %.c
	gcc $(CFLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	g++ $(CFLAGS) -MMD -c -o $@ $<

-include $(DEP)