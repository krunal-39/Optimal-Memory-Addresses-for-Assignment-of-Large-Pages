.PHONY: all clean run
all: main

main: main.c work.h libwork.so Makefile
	gcc -g -Wall -Wextra -Werror -O2 $< -L. -l:libwork.so -o $@

run: main
	./main ${SRNO}

clean: Makefile
	-@rm main
