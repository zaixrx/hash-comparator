main.out: main.c
	cc -o main.out main.c -O3 -Wall -Wextra -lssl -lcrypto

