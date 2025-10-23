main.out: main.c
	cc -o main.out main.c -O3 -Wall -Wextra -lssl -lcrypto
	echo "built release optimized build -> ./main.out"

debug: main.debug.out
	echo "built debug build -> ./main.debug.out"

main.debug.out: main.c
	cc -o main.debug.out main.c -Og -Wall -Wextra -lssl -lcrypto -g

