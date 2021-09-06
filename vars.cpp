#include <ncurses.h>
#include "vars.h"
// Pack all important variables into a file to make them accessible

// Characters that render.h uses to render the characters
char empty_pixel = ' ', unknown_pixel = '?', player_one_pixel = 'X', player_two_pixel = 'O';
int player_one_color = 1, player_two_color = 2;

int max_y, max_x;

// Size of the grid (Here are default values that can be changed in the start menu)
int grid_size_y = 3, grid_size_x = 3, pixels_needed = 3;


// Define print_details function
void Pixel::print_details(int row, int col)
{
	mvprintw(row, col, "Value: %d, Selected: %d, Pos_y: %d, Pos_x: %d", value, selected, pos_y, pos_x);
}

void Pixel::set_value(int my_value)
{
	value = my_value;
}

int Pixel::get_value()
{
	return value;
}

void Pixel::set_selected(bool my_selected)
{
	selected = my_selected;
}

bool Pixel::get_selected()
{
	return selected;
}


std::vector < std::vector<Pixel> > grid;


// Function to get and set Pixel properties safely by avoiding out of bound access

int get_grid_val(int pos_y, int pos_x)
{
	if (pos_y < grid_size_y && pos_x < grid_size_x && pos_y >= 0 && pos_x >= 0)
		return grid[pos_y][pos_x].get_value();
	else
	{
		mvprintw( max_y - 9, 2, "ERROR! Tried to get value of nonexistent pixel: Y: %d X: %d", pos_y, pos_x);
		return 0;
	}
}

void set_grid_val(int pos_y, int pos_x, int val)
{
	if (pos_y < grid_size_y && pos_x < grid_size_x && pos_y >= 0 && pos_x >= 0)
	{
		grid[pos_y][pos_x].set_value(val);
	}
	else
		mvprintw( max_y - 10, 2, "ERROR! Tried to set value of nonexistent pixel: Y: %d X: %d", pos_y, pos_x);
}

int get_grid_sel(int pos_y, int pos_x)
{
	if (pos_y < grid_size_y && pos_x < grid_size_x && pos_y >= 0 && pos_x >= 0)
		return grid[pos_y][pos_x].get_selected();
	else
	{
		mvprintw( max_y - 11, 2, "ERROR! Tried to get selected of nonexistent pixel: Y: %d X: %d", pos_y, pos_x);
		return 0;
	}
}
 
void set_grid_sel(int pos_y, int pos_x, bool selected)
{
	if (pos_y < grid_size_y && pos_x < grid_size_x && pos_y >= 0 && pos_x >= 0)
		grid[pos_y][pos_x].set_selected(selected);
	else
		mvprintw( max_y - 12, 2, "ERROR! Tried to set selected of nonexistent pixel: Y: %d X: %d", pos_y, pos_x);
}

int turn = 0;

int ch;
