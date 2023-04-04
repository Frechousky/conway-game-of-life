#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cmdline.h"

#define ALIVE 1
#define DEAD 0
#define DISPLAY_TIME 1 // iteration display time in seconds

#define get_cell_state(i, j, data) data->grid[j + data->w * (i)]
#define set_cell_state(i, j, data, v) data->grid[j + data->w * (i)] = v

typedef unsigned char byte;

struct GameOfLifeData {
  int h;      // grid height
  int w;      // grid width
  byte *grid; // keeps cells state (DEAD or ALIVE)
};
typedef struct GameOfLifeData GameOfLifeData_t;

/**
 * @brief Generate random game of life grid (i.e. each cell has a random ALIVE
 * or DEAD state) of size w * h
 *
 * @param w grid with
 * @param h grid height
 * @return byte* grid (must be free'd by caller)
 */
byte *generate_random_grid(int w, int h) {
  byte *grid = (byte *)malloc(h * w * sizeof(byte));
  for (int i = 0; i < h * w; i++) {
    grid[i] = (byte)(rand() & ALIVE);
  }
  return grid;
}

/**
 * @brief Convenient method to create GameOfLifeData_t
 *
 * @param w
 * @param h
 * @param grid
 * @return GameOfLifeData_t* data (must be free'd by caller)
 */
GameOfLifeData_t *init(int w, int h, byte *grid) {
  GameOfLifeData_t *data = (GameOfLifeData_t *)malloc(sizeof(GameOfLifeData_t));
  data->h = h;
  data->w = w;
  data->grid = grid;
  return data;
}

/**
 * @brief Convenient method to free GameOfLifeData_t
 *
 * @param d data to free
 */
void free_data(GameOfLifeData_t *d) {
  free(d->grid);
  free(d);
}

/**
 * @brief Get number of alive neighbours for cell at position (i, j) in grid,
 * checks up to 8 neighbours.
 *
 * @param data
 * @param i line index
 * @param j column index
 * @return int
 */
int get_alive_neighbours_count(GameOfLifeData_t *data, int i, int j) {
  int count = 0;
  if (i != 0) {
    if (j != 0)
      count += (int)get_cell_state(i - 1, j - 1, data); // top left
    count += (int)get_cell_state(i - 1, j, data);       // top
    if (j != data->w - 1)
      count += (int)get_cell_state(i - 1, j + 1, data); // top right
  }
  if (j != 0)
    count += (int)get_cell_state(i, j - 1, data); // left
  if (j != data->w - 1)
    count += (int)get_cell_state(i, j + 1, data); // right
  if (i != data->h - 1) {
    if (j != 0)
      count += (int)get_cell_state(i + 1, j - 1, data); // bottom left
    count += (int)get_cell_state(i + 1, j, data);       // bottom
    if (j != data->w - 1)
      count += (int)get_cell_state(i + 1, j + 1, data); // bottom right
  }
  return count;
}

/**
 * @brief Update state according to game of life rules (see
 * https://en.wikipedia.org/wiki/Conway's_Game_of_Life#Rules)
 *
 * @param old game of life data to update
 */
void update(GameOfLifeData_t **old) {
  GameOfLifeData_t *upd =
      init((*old)->w, (*old)->h,
           (byte *)malloc((*old)->h * (*old)->w * sizeof(byte)));
  for (int i = 0; i < (*old)->h; i++) {
    for (int j = 0; j < (*old)->w; j++) {
      int nghbrs_cnt = get_alive_neighbours_count(*old, i, j);
      byte old_state = get_cell_state(i, j, (*old));
      byte upd_state = DEAD;
      if (old_state == ALIVE && (nghbrs_cnt == 2 || nghbrs_cnt == 3)) {
        // Cells alive with 2 or 3 alive neighbours stays alive
        upd_state = ALIVE;
      } else if (old_state == DEAD && nghbrs_cnt == 3) {
        // Dead cells with 3 alive neighbours comes back to life
        upd_state = ALIVE;
      }
      set_cell_state(i, j, upd, upd_state);
    }
  }
  free_data(*old);
  *old = upd;
}

/**
 * @brief Display current game of life state to terminal, 10x10 display example
 * ('*' = alive cell) :
 *   ___________________________________________________________
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |  *  |     |     |     |     |     |     |     |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |  *  |     |  *  |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |     |  *  |     |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |     |  *  |  *  |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |  *  |     |     |  *  |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |     |  *  |     |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |     |     |  *  |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |     |     |     |     |     |     |     |     |     |     |
 *  |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *
 * @param data current game of life state to display
 */
void display(GameOfLifeData_t *data) {
  // clear terminal
  printf("\e[1;1H\e[2J");

  // First line
  // => ' ___________________________________________________________'
  printf(" _____");
  for (int j = 1; j < data->w; j++) {
    printf("______");
  }
  printf("\n");

  for (int i = 0; i < data->h; i++) {
    // First line of cell boxes
    // => '|     |     |     |     |     |     |     |     |     |     |'
    printf("|");
    for (int j = 0; j < data->w; j++) {
      printf("     |");
    }
    printf("\n");

    // Middle line of cell boxes
    // => '|  *  |     |     |  *  |     |  *  |  *  |     |     |     |'
    printf("|");
    for (int j = 0; j < data->w; j++) {
      if (get_cell_state(i, j, data) == ALIVE) {
        printf("  *  ");
      } else {
        printf("     ");
      }
      printf("|");
    }
    printf("\n");

    // Bottom line of cell boxes
    // => '|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|'
    printf("|");
    for (int j = 0; j < data->w; j++) {
      printf("_____|");
    }
    printf("\n");
  }
}

int main(int argc, char **argv) {
  struct gengetopt_args_info args;
  cmdline_parser(argc, argv, &args);
  GameOfLifeData_t *data =
      init(args.width_arg, args.height_arg,
           generate_random_grid(args.width_arg, args.height_arg));
  for (int i = 0; i < args.iter_arg; i++) {
    display(data);
    sleep(args.display_time_arg);
    if (i < args.iter_arg - 1) {
      update(&data);
    }
  }
  free_data(data);
  return 0;
}
