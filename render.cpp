// Code for rendering the grid
#include <ncurses.h>
#include <vector>

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
