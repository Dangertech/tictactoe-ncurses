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
		
		int win = detect_win();
		if (win == 0)
			mvprintw(max_y -8, 2, "No player has won");
		else if (win == -1)
			mvprintw(max_y -8, 2, "An error occured during the detection of the winning state!");
		else
			mvprintw(max_y - 8, 2, "Player %d has won!", win);
	}

	endwin();
	return 0;
}
