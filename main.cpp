#include <iostream>
#include <ncurses.h>
#include <menu.h>
#include <string>
#include <vector>
#include <vars.h>
#include <render.cpp>

int main()
{
	//Initialize ncurses
	initscr();
	// Standard attributes
	noecho();
	keypad(stdscr, TRUE);
	start_color();
	curs_set(0);
	// Initialize Color pairs
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	// Get the initial screen size
	max_x = getmaxx(stdscr);
	max_y = getmaxy(stdscr);
	
	
	///////////////// Make a 'New game' menu
	
	// Create a window that the menu will be displayed on
	WINDOW *menu;
	// Names for the options on the left side of the window
	std::vector <std::string> display_options { "Rows", "Columns", "Pixels needed to win" };
	// Values to display on the right side of the window
	std::vector <int> display_values { grid_size_y, grid_size_x, pixels_needed };
	
	int window_rows = display_options.size() + 4, window_columns = 50;
	int y_borders = 2; // Space to be left out before the first entry gets rendered
	int x_origin = (max_x / 2) - ( window_columns / 2), y_origin = 10;
	int selected_entry = 0;
	menu = newwin( window_rows, window_columns, y_origin, x_origin); 
	//Draw everything until the user confirms with 'Enter'
	while (ch != 10)
	{
		switch (ch)
		{
			case KEY_UP:
				if (selected_entry > 0)
					selected_entry--;
				break;
			case KEY_DOWN:
				if (selected_entry < display_options.size()-1)
					selected_entry++;
				break;
			case KEY_LEFT:
				if (display_values[selected_entry] > 2)
					display_values[selected_entry]--;
				break;
			case KEY_RIGHT:
				if (display_values[selected_entry] < 30)
					display_values[selected_entry]++;
				break;
		}
		// Check if selected_entry got out of bounds
		// Create a title and some decorations outside of the window
		mvprintw( max_y - 5, 2, "%d", selected_entry);
		mvcprintw( 3, "TIC TAC TOE");
		mvprintw( y_origin-1, x_origin, "Starting a new game:");
		mvprintw( y_origin + window_rows, x_origin, "Press 'Enter' to confirm your choices!");
		// Erase the entire window in case there was a menu entry with a higher digit count before than the one now
		// For example: A value was 10, now the user presses KEY_LEFT, but the menu shows 90 although it is nine, because the location of the '0' wasn't redrawn
		werase( menu );
		// Print a box around the menu
		box(menu, 1, 0);
		for (int i = 0; i < display_options.size(); i++)
		{
			if (selected_entry == i)
			{
				mvwprintw( menu, i + y_borders, getmaxx(menu)-10, "<");
				mvwprintw( menu, i + y_borders, getmaxx(menu)-6, ">");
				wattron( menu, A_STANDOUT );
			}
			mvwprintw( menu, i + y_borders, 2, display_options[i].c_str() );
			mvwprintw( menu, i + y_borders, getmaxx(menu) - 8, "%d", display_values[i] );
			wattroff( menu, A_STANDOUT );
		}
		refresh();
		wrefresh( menu );
		ch = getch();
	}
	// OK, this looks very ugly, but I couldn't find a better solution:
	// Assign every real variable the variable in the vector
	grid_size_y = display_values[0];
	grid_size_x = display_values[1];
	pixels_needed = display_values[2];
	// Delete the window and clear the screen
	delwin( menu );
	erase();
	
	
	///////////////// Set up game
	
	// Push all Pixels into the grid vector
	for (int y = 0; y < grid_size_y; y++)
	{
		grid.push_back(std::vector<Pixel>());
		for (int x = 0; x < grid_size_x; x++)
		{
			Pixel pixel;
			pixel.pos_y = y;
			pixel.pos_x = x;
			grid[y].push_back(pixel);
		}
	}
	// Debug occupations to test around
	//grid[0][0].value = 1;
	//grid[1][0].value = 2; 
	// Select the first pixel so that the player can easily see that there is some kind of cursor he can move
	grid[0][0].selected = true;
	render_grid( 1, 2);
	int y_index = 0, x_index = 0;
	// Clear ch before next use;
	ch = 0;
	bool is_done = false;
	 
	 
	///////////////// Main game loop starts here, the 1 will probably be replaced with a winning condition
	
	while (1 || is_done == false /* Selection must pass the check down below*/)
	{
		// Clear ch so that ch in't 10 and the while loop gets skipped
		ch = 0;
		 
		// Player selects Pixel to occupy
		while (ch != 10)
		{
			// Grab input
			ch = getch();
			mvprintw(max_y-3, 2, "%d", ch);
			// Process arrow key input
			switch(ch)
			{
				case KEY_DOWN: case 'j': case 's':
					y_index++; break;
				case KEY_UP: case 'k': case 'w':
					y_index--; break;
				case KEY_LEFT: case 'h': case 'a':
					x_index--; break;
				case KEY_RIGHT: case 'l': case 'd':
					x_index++; break;
			}
			
			// Deselect all pixels
			for (int i = 0; i < grid.size(); i++)
			{
				for (int j = 0; j < grid[i].size(); j++)
				{
					grid[i][j].selected = false;
				}
			}
			 
			// Check if indices should reset to another position because they got out of bounds
			if (y_index < 0)
				y_index = 0; 
			if (y_index > grid.size()-1) 
				y_index = grid.size() -1; 
			if (x_index < 0) 
				x_index = 0;
			if (x_index > grid[0].size()-1) 
				x_index = grid[0].size() -1; 
			  
			// Select the Pixel with the current index
			mvprintw(max_y -5, 2, "y_index: %d, x_index: %d, grid.size(): %d, grid[0].size: %d", y_index, x_index, grid.size(), grid[0].size());
			grid[y_index][x_index].selected = true;
			grid[y_index][x_index].print_details(max_y-4, 2);
			// Render it!
			render_grid(1, 2);
		 
		}
		// Check if that pixel is unoccupied
		if (grid[y_index][x_index].value == 0)
		{
			// Occupy the pixel with player 1's value after the user presses 'Enter'
			grid[y_index][x_index].value = 1;
			is_done = true;
		}
		else
		{
			// The loop continues until the player tries to occupy a valid pixel
			is_done = false;
		}
		render_grid(1,2);

		////////// Check if a player has won
		int x_parse = 0, y_parse = 0;
		int player_checked;
		bool has_won = false;
		while (y_parse < grid_size_y && x_parse < grid_size_x)
		{
			///// Get a pixel to start counting from on
			
			while (grid[y_parse][x_parse].value == 0)
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
			player_checked = grid[y_parse][x_parse].value;
			mvprintw(max_y-6, 2, "The first detected pixel is on Y: %d, X: %d, by player %d", y_parse, x_parse, player_checked);
			refresh();
			int ref_pixel_y = y_parse, ref_pixel_x = x_parse;
			has_won = true;
			 
			 
			///// Check for wins vertically
			
			for (int i=0; i<pixels_needed; i++)
			{
				if (ref_pixel_y <= grid_size_y - pixels_needed)
				{
					if (grid[ref_pixel_y + i][ref_pixel_x].value != player_checked)
					{
						// If a pixel downwards that is in range of pixels_needed doesn't have the value of the current player, break the statement immediately and set has_won to false
						has_won = false;
						break;
					}
				}
				else
				{
					has_won = false;
				}
				// If the player is just before winning, store the pixel to block for the opponent to use
				if (i == pixels_needed-1)
				{
					int weak_pixel_y = ref_pixel_y + i;
					int weak_pixel_x = ref_pixel_x;
				}
			}
			// If has_won is still true, player_checked must have won!
			if (has_won == true)
				mvprintw(max_y - 12, 2, "Player %d has won! (Vertical)", player_checked);
			 
			 
			///// Check for wins horizontally
			
			has_won = true;
			for (int i=0; i<pixels_needed; i++)
			{
				if (ref_pixel_x <= grid_size_x - pixels_needed)
				{
					if (grid[ref_pixel_y][ref_pixel_x+i].value != player_checked)
					{
						has_won = false;
						break;
					}
				}
				else
				{
					has_won = false;
				}
				if (i == pixels_needed-1)
				{
					int weak_pixel_y = ref_pixel_y;
					int weak_pixel_x = ref_pixel_x + i;
				}
			}
			 
			if (has_won == true)
				mvprintw(max_y - 11, 2, "Player %d has won! (Horizontal)", player_checked);
			 
			if (x_parse >= grid_size_x-1)
			{
				x_parse = 0;
				y_parse++;
			}
			else
			{
				x_parse++;
			}
			mvprintw(max_y - 1, 2, "x_parse: %d, y_parse: %d", x_parse, y_parse);
			refresh();
		}
	}

	endwin();
	return 0;
}
