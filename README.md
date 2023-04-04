# Conway Game of Life

[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) implementation in C.

## Requires

* `make`
* `gcc`

## Optionnal

* `valgrind`
* `gengetopt`

## Usage

* `make` or `make gameoflife` to build `gameoflife` binary
* `make run` or `./gameoflife` to run game of life
* `./gameoflife --help` FMI about CLI options
* `make clean` to delete `gameoflife` binary
* `make valgrind` to check for memory leaks
* `make gengetopt` to generate [cmdline.c](cmdline.c)/[cmdline.h](cmdline.h) files with [GNU Gengetopt](https://www.gnu.org/software/gengetopt/gengetopt.html) using [gengetopt.conf](gengetopt.conf) config file
