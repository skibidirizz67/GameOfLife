#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define MAP_W 1024
#define MAP_H MAP_W
#define PW (MAP_W+2)
#define PH (MAP_H+2)
#define MAP_SB ((PW*PH+7)/8)

#define STEPS 1024
#define SLEEP 500000

uint8_t map_a[MAP_SB], map_b[MAP_SB];
uint8_t *curr, *next;

static inline void bit_init(size_t id) {
    curr[id/8] |= (1<<(id%8));
}
static inline void bit_set(size_t id) {
    next[id/8] |= (1<<(id%8));
}
static inline void bit_clear(size_t id) {
    next[id/8] &= ~(1<<(id%8));
}
static inline uint8_t bit_get(size_t id) {
    uint8_t cell = curr[id/8] & (1<<(id%8));
    return cell != 0;
}

size_t base;
void map_update(uint8_t *__restrict cur, uint8_t *__restrict nxt) {
    memset(next, 0, MAP_SB);
	for (int y = 1; y < MAP_H; y++) {
		for (int x = 1; x < MAP_W; x++) {
			int n = bit_get((y-1)*PW+x-1) + bit_get((y-1)*PW+x) + bit_get((y-1)*PW+x+1)
			      + bit_get(y*PW+x-1) + bit_get(y*PW+x+1)
				  + bit_get((y+1)*PW+x-1) + bit_get((y+1)*PW+x) + bit_get((y+1)*PW+x+1);
			if ((n==3) | (bit_get(y*PW+x) & (n==2))) bit_set(y*PW+x);
		}
	}
}

void map_draw() {
    static char buffer[(MAP_W+1) * MAP_H + 1];
    char *bp = buffer;

	//printf("\033[H");
    for (int y = 1; y < MAP_H; y++) {
        for (int x = 1; x < MAP_W; x++) {
            *bp++ = bit_get(y*PW+x)? '#' : '.';
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
    bit_init(center-PW);
    bit_init(center-1);
    bit_init(center);
    bit_init(center+PW);
    bit_init(center+PW+1);
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