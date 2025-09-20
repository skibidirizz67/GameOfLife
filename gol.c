#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAP_W 16
#define MAP_H 16
#define MAP_S (MAP_W * MAP_H)
#define MAP_SB ((MAP_S + 7) / 8)

uint8_t map_a[MAP_SB];
uint8_t map_b[MAP_SB];

uint8_t *curr;
uint8_t *next;

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

static inline int safe_neighbour(int x, int y) {
    return (y+MAP_H)%MAP_H*MAP_W+(x+MAP_W)%MAP_W;
}
static inline int count_neighbours(int x, int y) {
    int count = 0;
    if (bit_get(safe_neighbour(x-1, y-1))) count++;
    if (bit_get(safe_neighbour(x, y-1))) count++;
    if (bit_get(safe_neighbour(x+1, y-1))) count++;
    if (bit_get(safe_neighbour(x-1, y))) count++;
    if (bit_get(safe_neighbour(x+1, y))) count++;
    if (bit_get(safe_neighbour(x-1, y+1))) count++;
    if (bit_get(safe_neighbour(x, y+1))) count++;
    if (bit_get(safe_neighbour(x+1, y+1))) count++;
    return count;
}

void map_update() {
    memset(next, 0, MAP_SB);
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            size_t id = (size_t)(y*MAP_W + x);
            int n = count_neighbours(x, y);
            if (bit_get(id) && (n == 2 || n == 3)) bit_set(id);
            else if (n == 3) bit_set(id);
        }
    }
}

void map_draw() {
    char buffer[(MAP_W+1) * MAP_H + 1];
    char *bp = buffer;

    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            *bp++ = bit_get(y*MAP_W+x)==1? '#' : '.';
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

    int center = MAP_H/2*MAP_W+MAP_W/2;

    bit_init(center-MAP_W);
    bit_init(center-1);
    bit_init(center);
    bit_init(center+MAP_W);
    bit_init(center+MAP_W+1);

    for (int step = 0; step < 2; step++) {
        map_update();
        map_draw();

        uint8_t *tmp = curr;
        curr = next;
        next = tmp;
    }
    
    return 0;
}