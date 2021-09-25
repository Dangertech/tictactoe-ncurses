#pragma once
#include <vector>

extern bool dbg; // Debug variable

// Characters that rendering.h uses to render the characters
extern int gamemode;
extern int starting_player;
extern int current_player; // Variable to get the current player, set in the main loop
extern char empty_pixel, unknown_pixel, player_one_pixel, player_two_pixel;
extern int player_one_color, player_two_color;

extern int max_x, max_y;

// selection values used by human_turn(), have to be reset in main()
extern int y_index, x_index;

// Size of the grid 
extern int grid_size_y, grid_size_x, pixels_needed;

// Class for each Tile on the grid
class Pixel
{
	private:
		int value;
		int selected;
	public:
		int pos_y, pos_x;
		 
		void set_value(int);
		int get_value();
		void set_selected(bool);
		bool get_selected();
		// DEBUG Print the details of a pixel
		void print_details(int,int);
};

// Multidimensional Vector that holds each Pixel
extern std::vector < std::vector<Pixel> > grid;

// Get and set grid values without causing a segfault
int get_grid_val(int, int);
void set_grid_val(int, int, int);
int get_grid_sel(int, int);
void set_grid_sel(int, int, bool);

// Turn count
extern int turn;

extern int ch;
