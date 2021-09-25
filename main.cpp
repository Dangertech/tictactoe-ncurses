#include <ncurses.h>
#include <string>
#include <vector>
#include "vars.h"
#include "rendering.h"
#include "processing.h"

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
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	// Error color pair
	init_pair(66, COLOR_WHITE, COLOR_RED);
	 
	// Get the initial screen size
	max_x = getmaxx(stdscr);
	max_y = getmaxy(stdscr);
	
	
	///////////////// Make a "New Game" Menu
	
	if (start_menu() == 1)
	{
		endwin();
		return 0;
	}
	
	mvprintw(max_y - 3, max_x - 20, "starting_player: %d", starting_player);
	
	 
	 
	///////////////// Main game loop
	 
	bool restart = true;
	while (restart == true) // Game Loop overspanning all games in this session
	{
		////// Set up game
		 
		ch = 0; // Clear ch before next use;
		current_player = starting_player;
		turn = 0;
		 
		// Update the screen size every restart
		max_x = getmaxx(stdscr);
		max_y = getmaxy(stdscr);
		grid.clear();
		 
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
		// Select the first pixel so that the player can easily see that there is some kind of cursor he can move
		set_grid_sel(0, 0, true);
		y_index = 0, x_index = 0; // Also reset indices for the selection of human_turn()
		
		///// Actual game
		 
		render_grid(); // Render the field once to show the grid before the user presses something
		 
		while (1) // Loop while the main game is running
		{
			if (current_player == 1 || gamemode == 1)
			{
				// Let the human make a turn 
				if (human_turn() == 1) // Returns 1 if the user presses 'q' to quit
				{
					endwin();
					return 0;
				}
				 
				if (gamemode == 1) // Advance in multiplayer
				{
					if (current_player == 1)
						current_player = 2;
					else if (current_player == 2)
						current_player = 1;
				}
				else // Let the computer make a turn in singleplayer
					current_player = 2;
			}
			
			 
			// Check if a player has won
			
			int win = detect_win();
			if (win == 0)
				mvprintw(max_y -8, 2, "No player has won");
			else if (win == -1)
				mvprintw(max_y -8, 2, "The field is full! The game has finished with a tie!");
			else
			{
				mvprintw(max_y - 8, 2, "Player %d has won!", win);
				current_player = win;
				break;
			}
			 
			if (gamemode == 0 && current_player == 2) // Singleplayer logic
			{
				computer_turn();
				win = detect_win();
				current_player = 1; // Set current player to the real one again
				turn++;
				if (win == 2)
				{
					mvprintw(max_y - 8, 2, "The computer has won!");
					current_player = win;
					break;
				}
				else if (win == -1)
					mvprintw(max_y - 8, 2, "The field is full! The game has finished with a tie!");
			} 
			 
			render_grid();
		}
		 
		render_grid();
		int win_menu_state = win_menu();
		if (win_menu_state == 0)
		{
			restart = false;
		}
	}
	endwin();
	return 0;
}
