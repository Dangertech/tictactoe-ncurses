#include <ncurses.h>
#include <vector>
#include <vars.h>

/*// Class for each Tile on the grid
class Pixel
{
	public:
		int value;
		int selected;
		int pos_y, pos_x;
		// DEBUG Print the details of a pixel
		void print_details(int row,int col)
		{
			mvprintw(row, col, "Value: %d, Selected: %d, Pos_y: %d, Pos_x: %d", value, selected, pos_y, pos_x);
		}
};

int grid_size_y = 3, grid_size_x = 3;
char empty_pixel = ' ', unknown_pixel = '?', player_a_pixel = 'X', player_b_pixel = 'O';
int max_y, max_x;

// Multidimensional Vector that holds each Pixel
std::vector < std::vector<Pixel> > grid;*/

#include <render.h>

/*void render_grid(int start_y, int start_x)
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
}*/

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
