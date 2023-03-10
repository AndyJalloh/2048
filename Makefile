# Macros
CC = gcc
CFLAGS = -std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes
LDFLAGS = -lm -lncurses

PROG = the2048
SRCS = main.c display.c grid2048.c
OBJS = $(SRCS:%.c=%.o)

# Executables
all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Phony
.PHONY: all clean run

clean:
	rm -f $(PROG) $(OBJS)

run: $(PROG)
	./$(PROG)

runwin: $(PROG)
	$(PROG).exe

display.o: display.c display.h grid2048.h
grid2048.o: grid2048.c grid2048.h
main.o: main.c grid2048.h display.h
