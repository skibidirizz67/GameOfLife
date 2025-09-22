// gcc main.c -O3 -funroll-loops -march=native && time ./a.out
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define MAP_W 1024
#define MAP_H MAP_W
#define PW (MAP_W+2)
#define PH (MAP_H+2)

#define STEPS 1024
#define SLEEP 500000

uint8_t map_a[PW*PH], map_b[PW*PH];
uint8_t *curr, *next;

void map_update(uint8_t *__restrict cur, uint8_t *__restrict nxt) {
	for (int y = 1; y < MAP_H; y++) {
		for (int x = 1; x < MAP_W; x++) {
			int n = cur[(y-1)*PW+x-1] + cur[(y-1)*PW+x] + cur[(y-1)*PW+x+1]
			      + cur[y*PW+x-1] + cur[y*PW+x+1]
				  + cur[(y+1)*PW+x-1] + cur[(y+1)*PW+x] + cur[(y+1)*PW+x+1];
			nxt[y*PW+x] = (n==3) | (cur[y*PW+x] & (n==2));
		}
	}
}

void map_draw() {
    static char buffer[(MAP_W+1) * MAP_H + 1];
    char *bp = buffer;

	//printf("\033[H");
    for (int y = 1; y < MAP_H; y++) {
        for (int x = 1; x < MAP_W; x++) {
            *bp++ = curr[y*PW+x]? '#' : '.';
        }
        *bp++ = '\n';
    }
    *bp++ = '\0';

    fwrite(buffer, 1, bp - buffer, stdout);
    fflush(stdout);
}

void map_init() {
	curr = map_a;
    next = map_b;

	int center = PH/2*PW+PW/2;
    curr[center-PW] = 1;
    curr[center-1] = 1;
    curr[center] = 1;
    curr[center+PW] = 1;
    curr[center+PW+1] = 1;
}

int main() {
    map_init();

    for (int step = 0; step < STEPS; step++) {
        map_update(curr, next);
		//map_draw();
        uint8_t *tmp = curr; curr = next; next = tmp;
		//usleep(SLEEP);
    }

    return 0;
}