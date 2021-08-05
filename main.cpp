#include <ncurses.h>
#include <vector>
#include <vars.h>
#include <render.h>

int main()
{
	//Initialize ncurses
	initscr();
	// Standard attributes
	noecho();
	keypad(stdscr, TRUE);
	start_color();
	// Initialize Color pairs
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	// Get the initial screen size
	max_x = getmaxx(stdscr);
	max_y = getmaxy(stdscr);
	
	//TODO: Make a 'New game' menu
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
	grid[0][0].value = 1;
	 
	grid[0][0].selected = true;
	render_grid( 1, 2);
	int y_index = 0, x_index = 0;
	int ch;
	// Player selects Pixel to occupy
	while (ch != 10)
	{
		// Grab input
		ch = getch();
		mvprintw(max_y-3, 2, "%d", ch);
		// Process arrow key input
		int down_key = 258, up_key = 259, left_key = 260, right_key = 261;
		switch(ch)
		{
			case 258:
				y_index++; break;
			case 259:
				y_index--; break;
			case 260:
				x_index--; break;
			case 261:
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

	endwin();
	return 0;
}
