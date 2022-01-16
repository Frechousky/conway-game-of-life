#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ALIVE           1
#define DEAD            0 
#define DISPLAY_TIME    1 // iteration display time in seconds

typedef unsigned char byte;

struct GameOfLifeData
{
    int h;      // grid width 
    int w;      // grid height
    byte* grid; // keeps cells state (DEAD or ALIVE)
};
typedef struct GameOfLifeData GameOfLifeData;

GameOfLifeData* init(int w, int h, byte* grid) {
    GameOfLifeData* data = malloc(sizeof(GameOfLifeData));
    data->h = h;
    data->w = w;
    data->grid = grid;
    return data;
}

/**
 * @brief Get cell state (0 = DEAD, 1 = ALIVE)
 * 
 * @param grid cells state grid
 * @param i row index of cell
 * @param j column index of cell
 * @param w grid width
 * @return byte cell state (0 = DEAD, 1 = ALIVE)
 */
byte getCellState(byte grid[], int i, int j, int w) {
    return grid[j + w * i];
}

/**
 * @brief Set the Cell State object
 * 
 * @param grid 
 * @param i 
 * @param j 
 * @param w 
 * @param state 
 */
void setCellState(byte grid[], int i, int j, int w, byte state) {
    grid[j + w * i] = state;
}

/**
 * @brief Get number of alive neighbours for point at position (i, j). Checks up to 8 neighbours.
 * 
 * @param data 
 * @param i line index
 * @param j column index
 * @return int 
 */
int getAliveNeighboursCount(GameOfLifeData* data, int i, int j) {
    int aliveNeighboursCount = 0;
    // top neighbours
    if (i != 0) {
        // top left
        if (j != 0) aliveNeighboursCount += (int) getCellState(data->grid, i - 1, j - 1, data->w);
        // top
        aliveNeighboursCount += (int) getCellState(data->grid, i - 1, j, data->w);
        // top right
        if (j != data->w - 1) aliveNeighboursCount += (int) getCellState(data->grid, i - 1, j + 1, data->w);
    }
    // left
    if (j != 0) aliveNeighboursCount += (int) getCellState(data->grid, i, j - 1, data->w);
    // right
    if (j != data->w - 1) aliveNeighboursCount += (int) getCellState(data->grid, i, j + 1, data->w);
    // bottom
    if (i != data->h - 1) {
        // bottom left
        if (j != 0) aliveNeighboursCount += (int) getCellState(data->grid, i + 1, j - 1, data->w);
        // bottom
        aliveNeighboursCount += (int) getCellState(data->grid, i + 1, j, data->w);
        // bottom right
        if (j != data->w - 1) aliveNeighboursCount += (int) getCellState(data->grid, i + 1, j + 1, data->w);
    }
    return aliveNeighboursCount;
}

/**
 * @brief Update state according to game of life rules (see https://en.wikipedia.org/wiki/Conway's_Game_of_Life#Rules)
 * 
 * @param toUpdate game of life data to update
 */
void update(GameOfLifeData* toUpdate) {
    byte* newGrid = malloc(toUpdate->h * toUpdate->w * sizeof(byte));
    for (int i = 0; i < toUpdate->h; i++) {
        for (int j = 0; j < toUpdate->w; j++) {
            int aliveNeighbours = getAliveNeighboursCount(toUpdate, i, j);
            byte currentCellState = getCellState(toUpdate->grid, i, j, toUpdate->w);
            byte newState = DEAD;
            if (currentCellState == ALIVE && (aliveNeighbours == 2 || aliveNeighbours == 3)) {
                // Cells alive with 2 or 3 alive neighbours stays alive
                newState = ALIVE;
            } else if (currentCellState == DEAD && aliveNeighbours == 3) {
                // Dead cells with 3 alive neighbours comes back to life
                newState = ALIVE;
            }
            setCellState(newGrid, i, j, toUpdate->w, newState);
        }
    }
    free(toUpdate->grid);
    toUpdate->grid = newGrid;
}

/**
 * @brief Display current game of life state to terminal, 10x10 display example ('*' = alive cell) :
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
 * @param toDisplay current game of life state to display
 */
void display(GameOfLifeData* toDisplay) {
    // clear terminal
    printf("\e[1;1H\e[2J");
    
    // First line
    // => ' ___________________________________________________________'
    printf(" _____");
    for (int j = 1; j < toDisplay->w; j++) {
        printf("______");
    }
    printf("\n");

    for (int i = 0; i < toDisplay->h; i++) {
        // First line of cell boxes
        // => '|     |     |     |     |     |     |     |     |     |     |'
        printf("|");
        for (int j = 0; j < toDisplay->w; j++) {
            printf("     |");
        }
        printf("\n");

        // Middle line of cell boxes
        // => '|  *  |     |     |  *  |     |  *  |  *  |     |     |     |'
        printf("|");
        for (int j = 0; j < toDisplay->w; j++) {
            if (getCellState(toDisplay->grid, i, j, toDisplay->w) == ALIVE) {
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
        for (int j = 0; j < toDisplay->w; j++) {
            printf("_____|");
        }
        printf("\n");
    }
} 

int main(int argc, char const *argv[])
{
    int h = 10, w = 10;
    byte* grid = malloc(h * w * sizeof(byte));
    // init grid with random cell states
    for (int i = 0; i < h * w; i++) {
        grid[i] = (byte) (rand() & ALIVE);
    }
    GameOfLifeData* gameData = init(w, h, grid);
    display(gameData);
    sleep(DISPLAY_TIME);
    for (int i = 0; i < 5; i++) {
        update(gameData);
        display(gameData);
        sleep(DISPLAY_TIME);
    }
    free(gameData->grid);
    free(gameData);
    return 0;
}
