## Build

### Linux
```cc main.c -O3 -funroll-loops -march=native```

## Funny notes
* `restrict` pointers in `map_update` make huge difference
* compiler is smart enough to pre-calculate some things (see `int n`)

## TODO
* cycled map
* optimize drawing and bit arrays
