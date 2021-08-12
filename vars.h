#pragma once
#include <vector>

// Characters that rendering.h uses to render the characters
extern char empty_pixel, unknown_pixel, player_a_pixel, player_b_pixel;

extern int max_x, max_y;

// Size of the grid 
extern int grid_size_y, grid_size_x, pixels_needed;

// Class for each Tile on the grid
class Pixel
{
	public:
		int value;
		int selected;
		int pos_y, pos_x;
		// DEBUG Print the details of a pixel
		void print_details(int,int);
};

// Multidimensional Vector that holds each Pixel
extern std::vector < std::vector<Pixel> > grid;

// Turn count
extern int turn;

extern int ch;
