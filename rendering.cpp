#include <ncurses.h>
#include <vector>
#include <string>
#include "vars.h"
#include "rendering.h"

////// START MENU


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
std::vector <menu_options> menu_container = 
{ 
	{"Rows", grid_size_y,{}, 0, false}, 
	{"Columns", grid_size_x, {}, 0,  false}, 
	{"Pixels needed to win", pixels_needed, {}, 0, false}, 
	{"Gamemode", 0, {"Singleplayer", "Multiplayer"}, 0, false },
	{"Player 1's Color", 0, {"Green", "Red", "Blue", "Yellow"}, player_one_color - 1, false }, 
	{"Player 1's Icon", 0, {"X", "O", "+", "-", "~", "@", "$", "#", "%"}, 0, false },
	{"Player 2's Color", 0, {"Green", "Red", "Blue", "Yellow"}, player_two_color - 1, false },
	{"Player 2's Icon", 0, {"X", "O", "+", "-", "~", "@", "$", "#", "%"}, 1, false },
	{"Starting Player", 0, {"You", "Opponent"}, starting_player, false}
};

// Function to render the menu later on with colors and characters
// matching the ones set by the player(s)
void xoxoprint(int line, std::string xoxo_str)
{
	int start_x = (max_x/2) - (xoxo_str.length() / 2);
	move(line, start_x);
	for (int i = 0; i<xoxo_str.length(); i++)
	{
		int temp_p_one_col = menu_container[4].text_loc + 1;
		int temp_p_two_col = menu_container[6].text_loc + 1;
		if (xoxo_str[i] == 'X')
		{
			// Print player_one's color and character
			attron(COLOR_PAIR(temp_p_one_col));
			addch(menu_container[5].display_text[menu_container[5].text_loc].at(0));
			attroff(COLOR_PAIR(temp_p_two_col));
		}
		else if (xoxo_str[i] == 'O')
		{
			// Print player_two's color and character
			attron(COLOR_PAIR(temp_p_two_col));
			addch(menu_container[7].display_text[menu_container[7].text_loc].at(0));
			attroff(COLOR_PAIR(temp_p_two_col));
		}
		else
		{
			// Print the remaining characters in the color
			// of the starting player
			if (menu_container[8].text_loc == 0)
				attron(COLOR_PAIR(temp_p_one_col));
			else if (menu_container[8].text_loc == 1)
				attron(COLOR_PAIR(temp_p_two_col));
			addch(xoxo_str[i]);
			attroff(COLOR_PAIR(temp_p_one_col));
			attroff(COLOR_PAIR(temp_p_two_col));
		}
	}
}

// Make a 'New game' menu
int start_menu() 
{
	// Create a window that the menu will be displayed on
	WINDOW *menu_win;

	
	int window_rows = menu_container.size() + 4, window_columns = 50;
	int x_origin = (max_x / 2) - ( window_columns / 2), y_origin = 13;
	int y_borders = 2; // Space to be left out before the first entry gets rendered
	int dis_val_start = window_columns - 16; // At which column to start rendering the entries
	int selected_entry = 0;
	menu_win = newwin( window_rows, window_columns, y_origin, x_origin); 
	//Draw everything until the user confirms with 'Enter'
	while (ch != 10 && ch != ' ')
	{
		switch (ch)
		{
			case KEY_UP: case 'k': case 'w':
				if (selected_entry > 0)
					selected_entry--;
			break;
			case KEY_DOWN: case 'j': case 's':
				if (selected_entry < menu_container.size()-1)
					selected_entry++;
			break;
			case KEY_LEFT: case 'h': case 'a':
				// If there is no text to be displayed, decrease the value of the int
				if (menu_container[selected_entry].display_text.size() == 0)
					menu_container[selected_entry].display_value--;
				// else, decrease the pointer to the current text entry
				else
				{
					menu_container[selected_entry].text_loc--;
					// Wrap around
					if (menu_container[selected_entry].text_loc < 0)
						menu_container[selected_entry].text_loc = menu_container[selected_entry].display_text.size() - 1;
				}
			break;
			case KEY_RIGHT: case 'l': case 'd':
				if (menu_container[selected_entry].display_text.size() == 0)
					menu_container[selected_entry].display_value++;
				else
				{
					menu_container[selected_entry].text_loc++;
					if (menu_container[selected_entry].text_loc == menu_container[selected_entry].display_text.size())
						menu_container[selected_entry].text_loc = 0;
				}
			break;
			case 'q':
				return 1;
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
			  
			int val = menu_container[i].display_value;
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
				menu_container[i].error = true;
		} 
		 
		// pixels_needed
		int dis_pix = menu_container[2].display_value;
		if (dis_pix > menu_container[0].display_value && dis_pix > menu_container[1].display_value)
		{
			error_msgs.push_back("A player needs more pixels in a row to win than there are rows and columns.");
			error_msgs.push_back("It is impossible to win.");
			menu_container[2].error = true;
		}
		else if (dis_pix > menu_container[0].display_value || dis_pix > menu_container[1].display_value)
		{
			error_msgs.push_back("The grid size is not large enough in one direction to win.");
			error_msgs.push_back("A game might be harder to win.");
			menu_container[2].error = true;
		}
		 
		if (dis_pix < 2)
		{
			error_msgs.push_back("Pixel streaks lower than 2 may produce unexpected behaviour!");
			menu_container[2].error = true;
		}
		 
		// Pixel color
		if (menu_container[4].text_loc == menu_container[6].text_loc)
		{
			error_msgs.push_back("Having both players with the same color is not recommended");
			menu_container[4].error = true;
			menu_container[6].error = true;
		}
		 
		// Player icons
		if (menu_container[5].text_loc == menu_container[7].text_loc)
		{
			error_msgs.push_back("Having both players with the same icons is not recommended");
			menu_container[5].error = true;
			menu_container[7].error = true;
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
		// I swear this looks halfway decent in the actual menu
		
		// xoxoprint converts the string here to the matching color
		// and character for each player
		// The X's and O's here are just "placeholders"
		xoxoprint( 3, " _____ ___ ____   _____  _    ____   _____ ___  _____ ");
		xoxoprint( 4, "|XOXXO|OXO/XOXO| |OOXOX|/X\\  /XOOX| |OXXOX/OXO\\|XXOXO|");
		xoxoprint( 5, "  |O|  |X|X|       |O| /O_X\\|X|       |X||O| |O|OXO|  ");
		xoxoprint( 6, "  |X|  |O|X|___    |X|/OXOXX\\O|___    |O||O| |O|O|___ ");
		xoxoprint( 7, " |X|  |OO\\OOXX|   |X/O/   \\O\\OXXO|   |X| \\XXO/|OXXOX|");
		
		mvprintw( y_origin-1, x_origin, "Starting a new game:");
		mvwprintw(menu_win, window_rows - 1, window_columns - 10, "<Enter>");
		// Error message if the terminal is too small
		attron(COLOR_PAIR(66));
		if (max_y < 25 || max_x < 70)
			mvprintw(0, 0, "Terminal too small for tictactoe-ncurses.\n You may experience rendering issues");
		attroff(COLOR_PAIR(66));
		 
		// Render window contents
		for (int i = 0; i < menu_container.size(); i++)
		{
			if (selected_entry == i)
			{
				// Render the input indicator 
				 
				 
				// Compute the position the right indicator has to render on
				int digit_size = 1;
				// If the display_text vector is empty, and the number should be shown
				if (menu_container[i].display_text.size() == 0)
				{
					if (menu_container[i].display_value > 9 || menu_container[i].display_value < 0)
						digit_size = 2;
					if (menu_container[i].display_value > 100 || menu_container[i].display_value < -9)
						digit_size = 3;
				}
				else
				{
					digit_size = menu_container[i].display_text[menu_container[i].text_loc].length();
				}
				mvwprintw( menu_win, i + y_borders, dis_val_start - 2, "<");
				mvwprintw( menu_win, i + y_borders, dis_val_start + 1+ digit_size, ">");
				// Activate the standout color pair
				wattron( menu_win, A_STANDOUT );
			}
			 
			// If the current entry is reported erroneous,
			// turn off the standout pair and activate the error color pair
			if (menu_container[i].error == true)
			{
				wattroff( menu_win, A_STANDOUT );
				wattron( menu_win, COLOR_PAIR(66) );
			}
			 
			mvwprintw( menu_win, i + y_borders, 2, menu_container[i].description.c_str() );
			// If the size of the text vector is empty, show the integer
			if (menu_container[i].display_text.size() == 0)
				mvwprintw( menu_win, i + y_borders, dis_val_start, "%d", menu_container[i].display_value );
			else
				// Else, show the text of the current entry
				mvwprintw( menu_win, i + y_borders, dis_val_start, menu_container[i].display_text[menu_container[i].text_loc].c_str() );
			// Turn the possible color pairs off
			wattroff( menu_win, A_STANDOUT );
			wattroff( menu_win, COLOR_PAIR(66) );
			// Reset error value to check if it is still true on the next while iteration
			menu_container[i].error = false;
		}
		refresh();
		wrefresh( menu_win );
		ch = getch();
	}
	// OK, this looks very ugly, but I couldn't find a better solution:
	// Assign every real variable the variable in the vector
	grid_size_y = menu_container[0].display_value;
	grid_size_x = menu_container[1].display_value;
	pixels_needed = menu_container[2].display_value;
	gamemode = menu_container[3].text_loc;
	player_one_color = menu_container[4].text_loc + 1;
	player_one_pixel = menu_container[5].display_text[menu_container[5].text_loc].at(0);
	player_two_color = menu_container[6].text_loc + 1;
	player_two_pixel = menu_container[7].display_text[menu_container[7].text_loc].at(0);
	starting_player = menu_container[8].text_loc + 1;
	// Delete the window and clear the screen
	delwin( menu_win );
	erase();
	return 0;
}

int win_menu()
{
	// Create a new window
	WINDOW *menu_win;
	// Construction variables
	int des_rows = 20, des_cols = 50;
	int y_borders = 7, x_borders = 8;
	int window_rows, window_columns;
	// Determine window sizes
	if (max_y > des_rows + y_borders*2)
		window_rows = des_rows;
	else
		window_rows = max_y - y_borders*2;
	 
	if (max_x > des_cols + x_borders*2)
		window_columns = des_cols;
	else
		window_columns = max_x - x_borders*2;
	int x_origin = (max_x / 2) - ( window_columns / 2), y_origin = y_borders; // Center window horizontally
	menu_win = newwin( window_rows, window_columns, y_origin, x_origin);
	 
	int selected = 0;
	
	 
	// Input
	ch = 0;
	while (ch != 10 && ch != ' ') 
	{
		///// PROCESSING INPUT
		//
		switch (ch)
		{
			case 113:
				delwin(menu_win);
				return 0;
				break;
			case KEY_UP: case 'k':
				selected = 0;
				break;
			case KEY_DOWN: case 'j':
				selected = 1;
				break;
		}
		
		 
		///// RENDERING
		 
		// The most basic renderer because there are only two options
		werase( menu_win );
		if (selected == 0)
		{
			// "Restart" stands out
			// Indicators
			mvwprintw( menu_win, window_rows - 6, window_columns / 2 - 5, "<" );
			mvwprintw( menu_win, window_rows - 6, window_columns / 2 + 5, ">" );
			wattron(menu_win, A_STANDOUT);
			mvwprintw( menu_win, window_rows - 6, window_columns / 2 - 3, "Restart" );
			wattroff(menu_win, A_STANDOUT);
			mvwprintw( menu_win, window_rows - 5, window_columns / 2 - 2, "Quit" );
		}
		else if (selected == 1)
		{
			// "Quit" stands out
			// Indicators
			mvwprintw( menu_win, window_rows - 5, window_columns / 2 - 4, "<" );
			mvwprintw( menu_win, window_rows - 5, window_columns / 2 + 3, ">" );
			mvwprintw( menu_win, window_rows - 6, window_columns / 2 - 3, "Restart" );
			wattron(menu_win, A_STANDOUT);
			mvwprintw( menu_win, window_rows - 5, window_columns / 2 - 2, "Quit" );
			wattroff(menu_win, A_STANDOUT);
		}
		 
		 
		// Make an outline box in the color of the current player
		int color;
		if (current_player == 1)
			color = player_one_color;
		else if (current_player == 2)
			color = player_two_color;
		wattron(menu_win, COLOR_PAIR(color));
		box(menu_win, 0, 0);
		wattroff(menu_win, COLOR_PAIR(color));
		 
		 
		// Win message in yellow
		wattron(menu_win, COLOR_PAIR(4));
		if (gamemode == 0 && current_player == 2)
		{
			mvwprintw( menu_win, 5, window_columns / 2 - 8, "The computer has won!" );
		}
		else if (current_player != -1)
		{
			mvwprintw( menu_win, 5, window_columns / 2 - 8, "Player %d has won!", current_player );
		}
		else
			mvwprintw( menu_win, 5, window_columns / 2 - 14, "The grid is full. It's a tie!" );
		 
		wattroff(menu_win, COLOR_PAIR(4));
		 
		// Some random deco
		wattron( menu_win, COLOR_PAIR(4) );
		mvwprintw( menu_win, 0, 0, "/");
		mvwprintw( menu_win, 0, window_columns - 1, "\\");
		wattroff( menu_win, COLOR_PAIR(4) );
		 
		 
		refresh();
		wrefresh(menu_win);
		 
		// Get input character
		ch = getch(); 
	}
	 
	 
	if (selected == 0) // 'Restart' selected
	{
		// erase the menu on the screen, delete the pointer and render the grid to fill spaces that might have been overwritten
		werase(menu_win);
		wrefresh(menu_win);
		delwin(menu_win);
		render_grid();
		return 1;
	}
	else if (selected == 1) // 'Quit' selected
		return 0;
	// An error must have happened
	delwin(menu_win);
	return -5;
}

void help_text(int start_y, int start_x)
{
	// Gamemode
	mvprintw(start_y, start_x, "You are playing a ");
	attron(A_BOLD); 
	if (gamemode == 0)
		printw("Singleplayer");
	else if (gamemode == 1)
		printw("Multiplayer");
	else
		printw("Oopstheremusthavebeenanerrorbecauseihavenofuckingideowhatgamemodeyourein");
	attroff(A_BOLD);
	printw(" game.\n");
	for (int i = 0; i < start_x; i++)
		printw(" ");
	 
	// Players turn
	if (gamemode == 0)
		printw("It is your turn. The computer doesn't have to think like you inferior humans.\n\n");
	else if (gamemode == 1)
	{
		printw("It is Player ");
		attron(A_BOLD); printw("%d", current_player); attroff(A_BOLD);
		printw("'s turn.\n");
	}
	for (int i = 0; i < start_x; i++)
		printw(" ");
	 
	// pixels_needed
	printw("You need ");
	attron(A_BOLD); printw("%d ", pixels_needed); attroff(A_BOLD);
	printw("pixels in a row to win.\n");
	for (int i = 0; i < start_x; i++)
		printw(" ");
	 
	// Use the Vi keys [ hjkl ] or [ WASD ] to move the cursor.
	printw("Use the Vi keys ");
	attron(A_BOLD); printw("[ hjkl ] "); attroff(A_BOLD);
	printw("or ");
	attron(A_BOLD); printw("[ wasd ] "); attroff(A_BOLD);
	printw("to move the cursor.\n");
	for (int i = 0; i < start_x; i++)
		printw(" ");
	 
	// Confirm your choice with [ Space ] or [ Enter ].
	printw("Confirm your choice with ");
	attron(A_BOLD); printw("[ Space ] "); attroff(A_BOLD);
	printw("or ");
	attron(A_BOLD); printw("[ Enter ]"); attroff(A_BOLD);
	printw(".\n");
	for (int i = 0; i < start_x; i++)
		printw(" ");
	// Quit with [ q ].
	printw("Quit with ");
	attron(A_BOLD); printw("[ q ]"); attroff(A_BOLD);
	printw(".");
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
	if (current_player == 1)
		attron(COLOR_PAIR(player_one_color));
	else if (current_player == 2)
		attron(COLOR_PAIR(player_two_color));
	make_rectangle(start_y, start_x, start_y + grid.size() * 2, start_x + grid[0].size() * 6);
	if (current_player == 1)
		attron(COLOR_PAIR(player_one_color));
	else if (current_player == 2)
		attron(COLOR_PAIR(player_two_color));
	attroff(COLOR_PAIR(current_player));
	mvprintw(start_y + grid.size() * 2, start_x + 2, "Turn: %d/%d", turn, (grid.size() * grid[0].size())/2);
}

// Function for printing completely centered
void mvcprintw( int pos_y, std::string to_print )
{
	int length = to_print.length();
	mvprintw(pos_y, (max_x / 2) - (length / 2), to_print.c_str());
}
