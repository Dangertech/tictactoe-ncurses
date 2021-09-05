#include <vector>
#include <cstdlib>
#include <time.h>
#include <ncurses.h>
#include "vars.h"
#include "processing.h"

// Check if a player has won
int detect_win()
{
	int x_parse = 0, y_parse = 0;
	int player_checked;
	bool vert_won = false, hor_won = false, dia_right_won = false, dia_left_won = false;
	//weak_pixels.clear();
	// Repeat until the end of the grid is reached
	while (y_parse < grid_size_y && x_parse < grid_size_x)
	{
		///// Get a pixel to start counting from on
		
		while (get_grid_val(y_parse, x_parse) == 0)
		{
			if (y_parse == grid_size_y -1 && x_parse == grid_size_x -1)
				break;
			if (x_parse == grid_size_x-1)
			{
				x_parse = 0;
				y_parse++;
			}
			else
			{
				x_parse++;
			}
		}
		player_checked = get_grid_val(y_parse, x_parse);
		int ref_pixel_y = y_parse, ref_pixel_x = x_parse;
		 
		///// Check for wins vertically
		
		// Set vert_won to true and see if it passes all tests and stays true
		vert_won = true;
		for (int i=0; i<pixels_needed; i++)
		{
			// Variable for weak pixel detection:
			// Check if the streak can be skipped because it is impossible to win starting here
			if (ref_pixel_y <= grid_size_y - pixels_needed)
			{
				if (get_grid_val(ref_pixel_y + i, ref_pixel_x) != player_checked)
				{
					// If a pixel downwards that is in range of pixels_needed doesn't have the value of the current player, break the statement immediately and set vert_won to false
					vert_won = false;
					break;
				}
				 
			}
			else
			{
				vert_won = false;
			}
		}
		// If vert_won is still true, player_checked must have won!
		if (vert_won == true)
			return player_checked;
		 
		 
		///// Check for wins horizontally
		
		hor_won = true;
		for (int i=0; i<pixels_needed; i++)
		{
			if (ref_pixel_x <= grid_size_x - pixels_needed)
			{
				if (get_grid_val(ref_pixel_y, ref_pixel_x+i) != player_checked)
				{
					hor_won = false;
					break;
				}
				 
			}
			else
				hor_won = false;
		}
		 
		if (hor_won == true)
			return player_checked;
		 
		 
		///// Check for wins diagonally to the right
		 
		dia_right_won = true;
		for (int i=0; i<pixels_needed; i++)
		{
			if (ref_pixel_x <= grid_size_x - pixels_needed && ref_pixel_y <= grid_size_y - pixels_needed)
			{
				if (get_grid_val(ref_pixel_y+i, ref_pixel_x+i) != player_checked)
				{
					dia_right_won = false;
					break;
				}
			}
			else
				dia_right_won = false;
			 
		}
		 
		if (dia_right_won == true)
			return player_checked;
		 
		 
		///// Check for wins diagonally to the left
		 
		dia_left_won = true;
		for (int i=0; i<pixels_needed; i++)
		{
			if (ref_pixel_x <= grid_size_x + pixels_needed && ref_pixel_y <= grid_size_y - pixels_needed)
			{
				if (get_grid_val(ref_pixel_y+i, ref_pixel_x-i) != player_checked)
				{
					dia_left_won = false;
					break;
				}
			}
			else
				dia_left_won = false;
			 
		}
		 
		if (dia_left_won == true)
			return player_checked;
		 
		// Increase the parse to not get stuck on the same pixel again
		if (x_parse >= grid_size_x-1)
		{
			x_parse = 0;
			y_parse++;
		}
		else
		{
			x_parse++;
		}
		
	}

	// Check if all spaces on the field are full
	// Just reuse x_ and y_parse
	// NOT WORKING AT THE MOMENT, Game just hangs if all fields are full because it tries to calculate an impossible computer turn
	/*x_parse = 0, y_parse = 0;
	while (y_parse < grid_size_y)
	{
		if (x_parse > grid_size_x-1)
		{
			x_parse = 0;
			y_parse++;
		}
		else
		{
			x_parse++;
		}
		// If y_parse has just grown over the grid size, return -1;
		// While would stop in the next operation
		if (y_parse == grid_size_y -1)
			return -1;
		if (get_grid_val(y_parse, x_parse) == 0)
			break;
	}*/
	 
	// Return 0 if no one has won
	if (vert_won == false && hor_won == false && dia_right_won == false && dia_left_won == false)
		return 0;
	// Error return value so that the compiler doesn't complain
	return -2;
}


// Variables used by computer_turn()
int rand_y, rand_x;
int place_y, place_x;
int origin_y, origin_x;
int expansion, prev_expansion;

std::vector< std::vector <int> > weak_pixels;

// Print all entries in weak_pixels below each other
void print_weak_pixels(int y_pos, int x_pos)
{
	// Clear the lines to not show any outdated info
	for (int i = 0; i<8; i++)
	{
		mvprintw(y_pos + i, x_pos, "\n");
	}
	for (int i = 0; i < weak_pixels.size(); i++)
	{
		mvprintw(y_pos + i, x_pos, "Entry %d: Y: %d, X: %d", i, weak_pixels[i][0], weak_pixels[i][1]);
	}
}

// Function to push back X and Y locations to weak_pixels in a single statement
void push_to_weak(int y_val, int x_val)
{
	if (get_grid_val(y_val, x_val) == 0)
	{
		weak_pixels.push_back(std::vector<int>());
		weak_pixels[weak_pixels.size() - 1].push_back(y_val);
		weak_pixels[weak_pixels.size() - 1].push_back(x_val);
	}
}
 
// Get a random spot to start placing pixels
void get_start_spot()
{
	// Seed rand()
	srand(time(0));
	do
	{
		// Check if rand() can generate without crashing
		if( pixels_needed < grid.size())
			// Choose a location that has a lot of free space around it
			rand_y = rand() % (grid.size() - pixels_needed) + 1;
		else
			// If not, just don't care about placement in free space
			rand_y = rand() % (grid.size());
		 
		if( pixels_needed < grid[0].size())
			rand_x = rand() % (grid[0].size() - pixels_needed) + 1;
		else
			rand_x = rand() % (grid[0].size());
	}
	while(get_grid_val(rand_y, rand_x) != 0);
	 
	place_y = rand_y, place_x = rand_x;
	origin_y = rand_y, origin_x = rand_x;
	// Place the pixel
	set_grid_val(place_y, place_x, 2);
}

void computer_turn()
{
	// Get a random spot on the first turn
	if(turn == 0)
	{
		get_start_spot();
		// ALWAYS return after invoking get_start_spot() to prevent placing a pixel
		return;
	}
	
	 
	///// EXPANSION OF A PIXEL LINE
	 
	prev_expansion = expansion;
	expansion = 0;
	// Check to where the pixel line can be expanded
	// And return an integer value
	// Keep track of iterations, in every iteration the computer checks a line in one direction
	int iteration = 0;
	while (expansion == 0)
	{
		// Set two parsers to the origin point
		int parse_x = origin_x, parse_y = origin_y;
		// Checker variable that only remains false if the loop was successful and a pixel can be placed in that direction
		bool move_on = false;
		int loop = 0;
		for (loop = 0; loop < pixels_needed; loop++)
		{
			// Avoid Segfault
			if (parse_y < grid.size() && parse_x < grid[0].size())
			{
				// If a pixel is occupied by the enemy player, move on
				if (get_grid_val(parse_y, parse_x) == 1)
				{
					move_on = true;
				}
			}
			 
			// Check expansions based on the current iteration
			switch(iteration)
			{
				case 0:
					parse_x++;
					break;
				case 1:
					parse_x--;
					break;
				case 2:
					parse_y++;
					break;
				case 3:
					parse_y--;
					break;
				case 4:
					parse_x++; parse_y++;
					break;
				case 5: 
					parse_x--; parse_y++;
					break;
			}
		}
		 
		// If move_on is still false, this expansion will be successful
		if (move_on == false)
		{
			// Set expansion to the corresponding iteration value
			switch(iteration)
			{
				case 0:
					expansion = 1;
					break;
				case 1:
					expansion = -1;
					break;
				case 2:
					expansion = 2;
					break;
				case 3:
					expansion = -2;
					break;
				case 4:
					expansion = 3;
					break;
				case 5:
					expansion = -3;
					break;
			}
		}
		// Increase iteration
		iteration++; 
		// If the Iteration exceeds the limit, there is no possible expansion; Find a new one
		if (iteration > 5)
		{
			refresh();
			get_start_spot();
			break;
			return;
		}
	}

	///// DETECTION OF DANGEROUS (WEAK) PIXELS OF THE OTHER PARTY
	weak_pixels.clear();
	int y_parse = 0, x_parse = 0;
	while (y_parse < grid_size_y && x_parse < grid_size_x)
	{
		// Find a pixel to start with
		while (get_grid_val(y_parse, x_parse) != 1)
		{
			if (y_parse == grid_size_y - 1 && x_parse == grid_size_x - 1)
				break;
			if (x_parse == grid_size_x - 1)
			{
				x_parse = 0;
				y_parse++;
			}
			else
				x_parse++;
		}
		 
		// Repeat this 4 times for 4 checks
		for (int i = 0; i < 4; i++)
		{
			int iter = 0;
			while (1)
			{
				// Get the check direction to perform in this loop
				// The switch has to be calculated every while iteration because iter changes each time
				int op_y, op_x;
				switch (i)
				{
					// Vertical Check
					case 0: op_y = y_parse + iter, op_x = x_parse; break;
					// Horizontal Check
					case 1: op_y = y_parse, op_x = x_parse + iter; break;
					// Diagonal right
					case 2: op_y = y_parse + iter, op_x = x_parse + iter; break;
					// Diagonal left
					case 3: op_y = y_parse + iter, op_x = x_parse - iter; break;
				}
				 
				// Check according to the operations set
				if (op_y >= grid_size_y || op_x >= grid_size_x)
				{
					// If the opponents pixel line is running towards the end of the grid, block the opposite side
					if (iter == pixels_needed - 1)
					{
						int neg_y = y_parse, neg_x = x_parse;
						switch (i)
						{
							case 0: neg_y--; break;
							case 1: neg_x--; break;
							case 2: neg_y--, neg_x--; break;
							case 3: neg_y--, neg_x++; break;
						}
						push_to_weak(neg_y, neg_x);
					}
					break;
				}
				// If you find a pixel that is unoccupied by Player 1
				if (get_grid_val(op_y, op_x) != 1)
				{
					// Check if you already crossed pixels_needed - 1 pixels and if the value of this pixel is empty
					if (iter == pixels_needed - 1)
					{
						// Push the pixel into weak
						push_to_weak(op_y, op_x);
						// Also push the pixel at the other side into weak
						int neg_y = y_parse, neg_x = x_parse;
						switch (i)
						{
							case 0: neg_y--; break;
							case 1: neg_x--; break;
							case 2: neg_y--, neg_x--; break;
							case 3: neg_y--, neg_x++; break;
						}
						push_to_weak(neg_y, neg_x);
					}
					break;
				}
				 
				// Increase iteration
				iter++;
			}
		}
		 
		// Continue parser to not get stuck on the same pixel again
		if (x_parse >= grid_size_x-1)
		{
			x_parse = 0;
			y_parse++;
		}
		else
		{
			x_parse++;
		}
	}
	 
	//print_weak_pixels(20,2);
	if (weak_pixels.size() == 0)
	{
		// If you expanded in that direction earlier, continue
		if (expansion == prev_expansion)
		{
			switch(expansion)
			{
				case 1:
					place_y = place_y, place_x++;
					break;
				case -1:
					place_y = place_y, place_x--;
					break;
				case 2:
					place_y++, place_x = place_x;
					break;
				case -2:
					place_y--, place_x = place_x;
					break;
				// Diagonal expansions only work downwards currently
				case 3:
					place_y++, place_x++;
					break;
				case -3:
					place_y++, place_x--;
					break;
				// Failsafe; The game has to continue in case of a human error!
				default:
					get_start_spot();
					return;
					break;
			}
		}
		// If not, move back to origin and go on from there
		else
		{
			switch(expansion)
			{
				case 1: 
					place_y = origin_y, place_x = origin_x + 1;
					break;
				case -1:
					place_y = origin_y, place_x = origin_x - 1;
					break;
				case 2:
					place_y = origin_y + 1, place_x = origin_x;
					break;
				case -2:
					place_y = origin_y - 1, place_x = origin_x;
					break;
				case 3:
					place_y = origin_y + 1, place_x = origin_x + 1;
					break;
				case -3:
					place_y = origin_y + 1, place_x = origin_x - 1;
					break;
				default:
					get_start_spot();
					return;
					break;
			}
		}
	}
	else
	{
		// Place the pixel on the weak pixel
		place_y = weak_pixels[0][0], place_x = weak_pixels[0][1];
	}
	 
	 
	//mvprintw(max_y - 12, 2, "expansion: %d, prev_expansion: %d, place_y: %d, place_x: %d", expansion, prev_expansion, place_y, place_x); 
	// Try to continue a pixel line
	if (place_y < grid.size() && place_x < grid[0].size())
	{
		if (get_grid_val(place_y, place_x) == 0)
			set_grid_val(place_y, place_x, 2);
		else
			get_start_spot();
	}
	else
		get_start_spot();
}
