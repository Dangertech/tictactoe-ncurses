#include <ncurses.h>
#include <vector>
#include <string>
#include "vars.h"
#include "rendering.h"

// Make a 'New game' menu
void start_menu() 
{
	// Create a window that the menu will be displayed on
	WINDOW *menu_win;
	// Create a struct that holds option properties
	struct menu_options
	{
		std::string description;
		// Int if the option should have a value
		int display_value;
		// Text if the option should have different strings of text
		std::vector < std::string > display_text;
		int text_loc; // Pointer to the current entry to be displayed
		bool error;
	};
	 
	// Construct a vector with the options
	std::vector <menu_options> start_menu = 
	{ 
		{"Rows", grid_size_y,{}, 0, false}, 
		{"Columns", grid_size_x, {}, 0,  false}, 
		{"Pixels needed to win", pixels_needed, {}, 0, false}, 
		{"Player 1's Color:", 0, {"Green", "Red", "Blue", "Yellow"}, player_one_color - 1, false }, 
		{"Player 1's Icon:", 0, {"X", "O", "+", "-", "~", "@", "$", "#", "%"}, 0, false },
		{"Player 2's Color:", 0, {"Green", "Red", "Blue", "Yellow"}, player_two_color - 1, false },
		{"Player 2's Icon:", 0, {"X", "O", "+", "-", "~", "@", "$", "#", "%"}, 1, false }
	};
	
	int window_rows = start_menu.size() + 4, window_columns = 50;
	int y_borders = 2; // Space to be left out before the first entry gets rendered
	int x_origin = (max_x / 2) - ( window_columns / 2), y_origin = 10;
	int selected_entry = 0;
	menu_win = newwin( window_rows, window_columns, y_origin, x_origin); 
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
				if (selected_entry < start_menu.size()-1)
					selected_entry++;
				break;
			case KEY_LEFT:
				// If there is no text to be displayed, decrease the value of the int
				if (start_menu[selected_entry].display_text.size() == 0)
					start_menu[selected_entry].display_value--;
				// else, decrease the pointer to the current text entry
				else
				{
					start_menu[selected_entry].text_loc--;
					// Wrap around
					if (start_menu[selected_entry].text_loc < 0)
						start_menu[selected_entry].text_loc = start_menu[selected_entry].display_text.size() - 1;
				}
				break;
			case KEY_RIGHT:
				if (start_menu[selected_entry].display_text.size() == 0)
					start_menu[selected_entry].display_value++;
				else
				{
					start_menu[selected_entry].text_loc++;
					if (start_menu[selected_entry].text_loc == start_menu[selected_entry].display_text.size())
						start_menu[selected_entry].text_loc = 0;
				}
				break;
		}
		 
		 
		 
		 
		///// ERROR HANDLING
		 
		// Make a vector that holds possible error messages
		std::vector <std::string> error_msgs;
		error_msgs.clear();
		// Push errors back into the error vector
		// 
		// Rows and columns
		for (int i = 0; i < 2; i++)
		{
			  
			int val = start_menu[i].display_value;
			// I know, this is pretty bad code but it's just a little easter egg
			if (val < 2)
			{
				error_msgs.push_back("Sizes smaller than two are not supported!");
				error_msgs.push_back("You may experience crashes and unexpected behaviour!");
			}
			else if (val > 30 && val < 120)
			{
				error_msgs.push_back("You need a sufficiently large screen to properly display high grid sizes!");
				error_msgs.push_back("You may experience unexpected behaviour!");
			}
			else if (val > 120 && val < 235)
				error_msgs.push_back("I don't think there's a screen that large...");
			else if (val > 235 && val < 512)
				error_msgs.push_back("Ok, you're bored, right?");
			else if (val > 512 && val < 828)
				error_msgs.push_back("Are you trying to impress me through your waste of time?");
			else if (val > 828 && val < 1021)
				error_msgs.push_back("This is getting ridiculous.");
			else if (val > 1021 && val < 1372)
				error_msgs.push_back("You may run into some performance issues now, btw");
			else if (val > 1372 && val < 1845)
				error_msgs.push_back("I really don't have time for this.");
			else if (val > 1845)
				error_msgs.push_back("Don't you have hobbies or something!?");
			// Turn error to true in all cases from above
			if (val < 2 || val > 30)
				start_menu[i].error = true;
		} 
		 
		// pixels_needed
		int dis_pix = start_menu[2].display_value;
		if (dis_pix > start_menu[0].display_value && dis_pix > start_menu[1].display_value)
		{
			error_msgs.push_back("A player needs more pixels in a row to win than there are rows and columns.");
			error_msgs.push_back("It is impossible to win.");
			start_menu[2].error = true;
		}
		else if (dis_pix > start_menu[0].display_value || dis_pix > start_menu[1].display_value)
		{
			error_msgs.push_back("The grid size is not large enough in one direction to win.");
			error_msgs.push_back("A game might be harder to win.");
			start_menu[2].error = true;
		}
		 
		if (dis_pix < 2)
		{
			error_msgs.push_back("Pixel streaks lower than 2 may produce unexpected behaviour!");
			start_menu[2].error = true;
		}
		 
		// Pixel color
		if (start_menu[3].text_loc == start_menu[5].text_loc)
		{
			error_msgs.push_back("Having both players with the same color is not recommended");
			start_menu[3].error = true;
			start_menu[5].error = true;
		}
		 
		// Player icons
		if (start_menu[4].text_loc == start_menu[6].text_loc)
		{
			error_msgs.push_back("Having both players with the same icons is not recommended");
			start_menu[4].error = true;
			start_menu[6].error = true;
		}
		 
		 
		//// Print error messages
		erase();
		attron( COLOR_PAIR(2) );
		for (int i = 0; i < error_msgs.size(); i++)
		{
			mvprintw( y_origin + window_rows + 2 + i, x_origin + 3, error_msgs[i].c_str() );
		}
		attroff( COLOR_PAIR(2) );
		 
		 
		 
		///// MENU RENDERING
		 
		// Erase the entire window in case there was a menu entry with a higher digit count before than the one now
		// For example: A value was 10, now the user presses KEY_LEFT, 
		// but the menu shows 90 although it is nine, because the location of the '0' wasn't redrawn
		werase( menu_win );
		// Print a box around the menu window
		box(menu_win, 1, 0);
		 
		// Create a title and some decorations outside of the window
		mvcprintw( 3, "TIC TAC TOE");
		mvprintw( y_origin-1, x_origin, "Starting a new game:");
		mvwprintw(menu_win, window_rows - 1, window_columns - 10, "<Enter>");
		 
		// Render window contents
		for (int i = 0; i < start_menu.size(); i++)
		{
			if (selected_entry == i)
			{
				// Render the input indicator 
				// 
				int digit_size = 1;
				// If the display_text vector is empty, and the number should be shown
				if (start_menu[i].display_text.size() == 0)
				{
					if (start_menu[i].display_value > 9 || start_menu[i].display_value < 0)
						digit_size = 2;
					if (start_menu[i].display_value > 100 || start_menu[i].display_value < -9)
						digit_size = 3;
				}
				else
				{
					digit_size = start_menu[i].display_text[start_menu[i].text_loc].length();
				}
				mvwprintw( menu_win, i + y_borders, getmaxx(menu_win) - 10, "<");
				mvwprintw( menu_win, i + y_borders, getmaxx(menu_win) - 7 + digit_size, ">");
				// Activate the standout color pair
				wattron( menu_win, A_STANDOUT );
			}
			 
			// If the current entry is reported erroneous,
			// turn off the standout pair and activate the error color pair
			if (start_menu[i].error == true)
			{
				wattroff( menu_win, A_STANDOUT );
				wattron( menu_win, COLOR_PAIR(66) );
			}
			 
			mvwprintw( menu_win, i + y_borders, 2, start_menu[i].description.c_str() );
			// If the size of the text vector is empty, show the integer
			if (start_menu[i].display_text.size() == 0)
				mvwprintw( menu_win, i + y_borders, getmaxx(menu_win) - 8, "%d", start_menu[i].display_value );
			else
				// Else, show the text of the current entry
				mvwprintw( menu_win, i + y_borders, getmaxx(menu_win) - 8, start_menu[i].display_text[start_menu[i].text_loc].c_str() );
			// Turn the possible color pairs off
			wattroff( menu_win, A_STANDOUT );
			wattroff( menu_win, COLOR_PAIR(66) );
			// Reset error value to check if it is still true on the next while iteration
			start_menu[i].error = false;
		}
		refresh();
		wrefresh( menu_win );
		ch = getch();
	}
	// OK, this looks very ugly, but I couldn't find a better solution:
	// Assign every real variable the variable in the vector
	grid_size_y = start_menu[0].display_value;
	grid_size_x = start_menu[1].display_value;
	pixels_needed = start_menu[2].display_value;
	player_one_color = start_menu[3].text_loc + 1;
	player_one_pixel = start_menu[4].display_text[start_menu[4].text_loc].at(0);
	player_two_color = start_menu[5].text_loc + 1;
	player_two_pixel = start_menu[6].display_text[start_menu[6].text_loc].at(0);
	// Delete the window and clear the screen
	delwin( menu_win );
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
					attron(COLOR_PAIR(player_one_color));
					this_pixel = player_one_pixel;
					break;
				case 2:
					attron(COLOR_PAIR(player_two_color));
					this_pixel = player_two_pixel;
					break;
				default:
					this_pixel = unknown_pixel;
			}
			printw("%c", this_pixel);
			attroff(COLOR_PAIR(player_one_color));
			attroff(COLOR_PAIR(player_two_color));
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
