#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAP_W 1024
#define MAP_H MAP_W
#define PW (MAP_W+2)
#define PH (MAP_H+2)
#define STEPS 1024

uint8_t map_a[PW*PH], map_b[PW*PH];

uint8_t *curr;
uint8_t *next;

void map_update() {
	for (int y = 1; y < MAP_H; y++) {
		for (int x = 1; x < MAP_W; x++) {
			int n = curr[(y-1)*PW+x-1] + curr[(y-1)*PW+x] + curr[(y-1)*PW+x+1]
			      + curr[y*PW+x-1] + curr[y*PW+x+1]
				  + curr[(y+1)*PW+x-1] + curr[(y+1)*PW+x] + curr[(y+1)*PW+x+1];
			next[y*PW+x] = (n==3) | (curr[y*PW+x] & (n==2));
		}
	}
}

void map_draw() {
    char buffer[(MAP_W+1) * MAP_H + 1];
    char *bp = buffer;

    for (int y = 1; y <= MAP_H; ++y) {
        for (int x = 1; x <= MAP_W; ++x) {
            *bp++ = curr[y*PW+x]? '#' : '.';
        }
        *bp++ = '\n';
    }
    *bp++ = '\0';

    fwrite(buffer, 1, bp - buffer, stdout);
    fflush(stdout);
}

int main() {
    curr = map_a;
    next = map_b;

    int center = PH/2*PW+PW/2;

    curr[center-PW] = 1;
    curr[center-1] = 1;
    curr[center] = 1;
    curr[center+PW] = 1;
    curr[center+PW+1] = 1;

    for (int step = 0; step < STEPS; step++) {
        map_update();
        //map_draw();

        uint8_t *tmp = curr;
        curr = next;
        next = tmp;
    }
    
    return 0;
}