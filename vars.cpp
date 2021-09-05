#include <ncurses.h>
#include "vars.h"
// Pack all important variables into a file to make them accessible

// Define print_details function
void Pixel::print_details(int row, int col)
{
	mvprintw(row, col, "Value: %d, Selected: %d, Pos_y: %d, Pos_x: %d", value, selected, pos_y, pos_x);
}

void Pixel::set_value(int value)
{
	Pixel.value = value;
}

int Pixel::get_value()
{
	return Pixel.value;
}

void Pixel::set_selected(bool selected)
{
	Pixel.selected = selected;
}

bool Pixel::get_selected()
{
	return Pixel.selected;
}

// Size of the grid (Here are default values that can be changed in the start menu)
int grid_size_y = 3, grid_size_x = 3, pixels_needed = 3;
// Characters that render.h uses to render the characters
char empty_pixel = ' ', unknown_pixel = '?', player_a_pixel = 'X', player_b_pixel = 'O';

int ch;

int max_y, max_x;

std::vector < std::vector<Pixel> > grid;

// Function to get and set Pixel properties safely by avoiding out of bound access

void set_grid_val(int pos_y, int pos_x, int val)
{
	if (pos_y < grid_size_y && pos_x < grid_size_x)
	{
		grid[pos_y][pos_x].set_value(val);
	}
	else
		mvprintw( max_y - 10, 2, "ERROR! Tried to set value of nonexistent pixel: Y: %d X: %d", pos_y, pos_x);
}
 
int get_grid_val(int pos_y, int pos_x)
{
	if (pos_y < grid_size_y && pos_x < grid_size_x)
		return grid[pos_y][pos_x].get_value();
	else
	{
		mvprintw( max_y - 9, 2, "ERROR! Tried to get value of nonexistent pixel: Y: %d X: %d", pos_y, pos_x);
		return -1;
	}
}

int turn = 0;
