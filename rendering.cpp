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

void render_title(int start_y, int start_x)
{
	// Render the title
	// If we have ASCII art, here will be ASCII art
	move(start_y, start_x);
	int middle = ((grid[0].size() * 6) / 2);
	for (int i = 0; i < grid[0].size(); i++)
	{
		printw("======");
	}
	std::string title = "TIC TAC TOE";
	mvprintw(start_y + 1, (middle - (title.length() / 2)) + start_x, "%s\n",title.c_str());
	move(start_y + 2, start_x);
	for (int i = 0; i < grid[0].size(); i++)
	{
		printw("======");
	}
	printw("\n");
}

// Function for drawing a rectangle without requiring to spawn a window
void make_rectangle(int start_y, int start_x, int end_y, int end_x)
{
	mvhline(start_y ,start_x, 0, end_x-start_x);
	mvhline(end_y, start_x, 0, end_x-start_x);
	mvvline(start_y, start_x, 0, end_y-start_y);
	mvvline(start_y, end_x, 0, end_y-start_y);
	mvaddch(start_y, start_x, ACS_ULCORNER);
	mvaddch(end_y, start_x, ACS_LLCORNER);
	mvaddch(start_y, end_x, ACS_URCORNER);
	mvaddch(end_y, end_x, ACS_LRCORNER);
}

// Function for rendering the grid
void render_grid()
{
	render_title(1, 2);
	int start_y = 5, start_x = 2;
	int y_pos, x_pos;
	// Render the actual grid
	for (int y = 0; y < grid.size(); y++)
	{
		move(start_y + y * 2 + 1, start_x + 1); 
		for (int x = 0; x < grid[0].size(); x++)
		{
			printw(" ");
			// Render selected pixel on the left
			if (get_grid_sel(y, x) == true)
				printw(">");
			else
				printw(" ");
			// Render pixel value
			char this_pixel;
			switch (get_grid_val(y, x))
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
			if (get_grid_sel(y, x) == true)
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
			getyx(stdscr, y_pos, x_pos);
			move(y_pos, start_x + 1);
			for (int x = 0; x < grid[0].size()-1; x++)
				printw("------");
			// Print one character less on the last x position
			printw("-----");
		}
		// Repeat on a new line
		printw("\n");
	}
	make_rectangle(start_y, start_x, start_y + grid.size() * 2, start_x + grid[0].size() * 6);
	mvprintw(start_y + grid.size() * 2, start_x + 2, "Turn: %d/%d", turn, (grid.size() * grid[0].size())/2);
}

// Function for printing completely centered
void mvcprintw( int pos_y, std::string to_print )
{
	int length = to_print.length();
	mvprintw(pos_y, (max_x / 2) - (length / 2), to_print.c_str());
}
