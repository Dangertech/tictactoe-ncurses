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
	
	start_menu();
	
	 
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
	//set_grid_val(0, 0, 1);
	//set_grid_val(0, 1, 2); 
	// Select the first pixel so that the player can easily see that there is some kind of cursor he can move
	set_grid_sel(0, 0, true);
	int y_index = 0, x_index = 0;
	// Clear ch before next use;
	ch = 0;
	bool is_done = false;
	current_player = 1;
	render_grid(); // Render the field once to show the grid before the user presses something
	 
	 
	///////////////// Main game loop
	
	while (1)
	{
		do
		{
			// Clear ch so that ch in't 10 and the while loop gets skipped
			ch = 0;
			 
			// Player selects Pixel to occupy
			while (ch != 10)
			{
				// Grab input
				ch = getch();
				mvprintw(max_y-3, 2, "%d", ch);
				if (ch == 113) // If 'q' is pressed, quit immediately
				{
					endwin();
					return 0;
				}
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
						set_grid_sel(i, j, false);
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
				  
				mvprintw(max_y -5, 2, "y_index: %d, x_index: %d, grid.size(): %d, grid[0].size: %d", y_index, x_index, grid.size(), grid[0].size());
				// Select the Pixel with the current index
				set_grid_sel(y_index, x_index, true);
				// Print the details of the currently selected pixel
				grid[y_index][x_index].print_details(max_y-4, 2);
				// Render it!
				render_grid();
			}
			 
			// Check if that pixel is unoccupied
			if (get_grid_val(y_index, x_index) == 0)
			{
				// Occupy the pixel with the current_player after the user presses 'Enter'
				set_grid_val(y_index, x_index, current_player);
				is_done = true;
			}
			else
				// The loop continues until the player tries to occupy a valid pixel
				is_done = false;
			render_grid();
		} while (is_done == false);
		 
		 
		// Set current_player to the next one
		if (current_player == 1)
			current_player = 2;
		else if (current_player == 2)
		{
			current_player = 1;
			turn++; // Increase the turn count if the 2nd player (no matter if computer or human) has made their turn
		}
		 
		////////// Check if a player has won
		
		int win = detect_win();
		if (win == 0)
		{
			mvprintw(max_y -8, 2, "No player has won");
			if (gamemode == 0) // Singleplayer logic
			{
				computer_turn();
				win = detect_win();
				current_player = 1; // Set current player to the real one again
				turn++;
				if (win == 2)
					mvprintw(max_y - 8, 2, "The computer has won!");
				else if (win == -1)
					mvprintw(max_y - 8, 2, "The field is full! The game has finished with a tie!");
			}
		}
		else if (win == -1)
			mvprintw(max_y -8, 2, "The field is full! The game has finished with a tie!");
		else
		{
			mvprintw(max_y - 8, 2, "Player %d has won!", win);
			current_player = win;
			break;
		}
		 
		render_grid();
	}
	
	int win_menu_state = win_menu();
	if (win_menu_state == 0)
	{
		endwin();
		return 0;
	}
	else if (win_menu_state == 1)
	{
		// Replace this with the restart trigger
		endwin();
		return 1;
	}
}
