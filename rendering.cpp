#include <ncurses.h>
#include <vector>
#include <string>
#include "vars.h"
#include "rendering.h"

// Make a 'New game' menu
void start_menu() 
{
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
}

// Function for rendering the grid
void render_grid(int start_y, int start_x)
{
	move(start_y, 0);
	for (int y = 0; y < grid.size(); y++)
	{
		for (int i = 0; i < start_x; i++)
			printw(" ");
		for (int x = 0; x < grid[0].size(); x++)
		{
			printw(" ");
			// Render selected pixel on the left
			if (grid[y][x].selected == true)
				printw(">");
			else
				printw(" ");
			// Render pixel value
			char this_pixel;
			switch (grid[y][x].value)
			{
				case 0:
					this_pixel = empty_pixel;
					break;
				case 1:
					attron(COLOR_PAIR(1));
					this_pixel = player_a_pixel;
					break;
				case 2:
					attron(COLOR_PAIR(2));
					this_pixel = player_b_pixel;
					break;
				default:
					this_pixel = unknown_pixel;
			}
			printw("%c", this_pixel);
			attroff(COLOR_PAIR(1));
			attroff(COLOR_PAIR(2));
			// Render selected pixel on the right
			if (grid[y][x].selected == true)
				printw("<");
			else
				printw(" ");
			// Render separator
			if (x != grid[0].size() - 1)
			{
				printw(" |");
			}
		}
		// Begin a new line and render the break
		printw("\n");
		if (y != grid.size() - 1)
		{
			for (int i = 0; i < start_x; i++)
				printw(" ");
			for (int x = 0; x < grid[0].size(); x++)
				printw("------");
		}
		// Repeat on a new line
		printw("\n");
	}
}

// Function for printing completely centered
void mvcprintw( int pos_y, std::string to_print )
{
	int length = to_print.length();
	mvprintw(pos_y, (max_x / 2) - (length / 2), to_print.c_str());
}
