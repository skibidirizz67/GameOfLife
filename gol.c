#include <stdio.h>
#include <time.h>
#include <string.h>

#define W 16
#define H 16

#define TPS 1
const int DELAY = 1000 / TPS;

char map[H*W];
char tmp[H*W];

void sleep(int ms) {
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

void init() {
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			map[y*W + x] = '.';
		}
	}

	int center = H/2*W+W/2;
	
	map[center-1] = '#';
	map[center] = '#';
	map[center+1] = '#';
}

void draw() {
	char buffer[H*W+H+1];
	for (int y = 0; y < (H+1); y++) {
		for (int x = 0; x < W; x++) {
			buffer[y*W + x] = '0';
		}
	}
	
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			buffer[y*W+x+y] = map[y*W + x];
		}
		buffer[(y+1)*W+y] = '\n';
	}
	buffer[H*W] = '\0';
	printf("%s\n", buffer);
}

int count_nbs(int id) {
	int nbs = 0;

	for (int y = -1; y < 2; y++) {
		for (int x = -1; x < 2; x++) {
			if (tmp[id+W*y+x] == '#') nbs++;
		}
	}
	if (tmp[id] == '#') nbs--;

	return nbs;
}

void update_cell(int id) {
	int nbs = count_nbs(id);
	if (map[id] == '.') {
		if (nbs == 3) map[id] = '#';
	}
	else if (map[id] == '#') {
		if (nbs < 2 || nbs > 4) map[id] = '.';
	}
	else {
		map[id] = '.';
	}
}

void update_map() {
	strncpy(tmp, map, H*W);
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			update_cell(y*W+x);
		}
	}	
}

int main() {
	init();

	for (int i = 0; i < 2; i++) {
		draw();
		update_map();

		//sleep(DELAY);
	}
		
	return 0;
}
