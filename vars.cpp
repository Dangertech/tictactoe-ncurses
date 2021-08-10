#include <ncurses.h>
#include "vars.h"
// Pack all important variables into a file to make them accessible

// Define print_details function
void Pixel::print_details(int row, int col)
{
	mvprintw(row, col, "Value: %d, Selected: %d, Pos_y: %d, Pos_x: %d", value, selected, pos_y, pos_x);
}

// Size of the grid (Here are default values that can be changed in the start menu)
int grid_size_y = 3, grid_size_x = 3, pixels_needed = 3;
// Characters that render.h uses to render the characters
char empty_pixel = ' ', unknown_pixel = '?', player_a_pixel = 'X', player_b_pixel = 'O';

int ch;

int max_y, max_x;

std::vector < std::vector<Pixel> > grid;
