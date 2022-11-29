CC := gcc
CFLAGS := -I. -lm

.PHONY: all
all:
	make p1
	make p2

p1: p1.c
p2: p2.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean: 
	rm -f p1 p2