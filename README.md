## Build

### Linux
```cc main.c -O3 -funroll-loops -march=native```

## Funny notes
* despite being so simple it is actually pretty fast (one generation of 10000*10000 computes in ~75ms on i5-7400)
* `restrict` pointers in `map_update` make huge difference
* compiler is smart enough to pre-calculate some things (see `int n`)

## TODO
* cycled map
* optimize drawing and bit arrays
