// Check if a player has won
int detect_win()
{
	int x_parse = 0, y_parse = 0;
	int player_checked;
	bool vert_won = false, hor_won = false, dia_won = false;
	while (y_parse < grid_size_y && x_parse < grid_size_x)
	{
		///// Get a pixel to start counting from on
		
		while (grid[y_parse][x_parse].value == 0)
		{
			if (y_parse == grid_size_y -1 && x_parse == grid_size_x -1)
				break;
			if (x_parse == grid_size_x-1)
			{
				x_parse = 0;
				y_parse++;
			}
			else
			{
				x_parse++;
			}
		}
		player_checked = grid[y_parse][x_parse].value;
		int ref_pixel_y = y_parse, ref_pixel_x = x_parse;
		 
		 
		///// Check for wins vertically
		
		// Set vert_won to true and see if it passes all tests and stays true
		vert_won = true;
		for (int i=0; i<pixels_needed; i++)
		{
			if (ref_pixel_y <= grid_size_y - pixels_needed)
			{
				if (grid[ref_pixel_y + i][ref_pixel_x].value != player_checked)
				{
					// If a pixel downwards that is in range of pixels_needed doesn't have the value of the current player, break the statement immediately and set vert_won to false
					vert_won = false;
					break;
				}
			}
			else
			{
				vert_won = false;
			}
			// If the player is just before winning, store the pixel to block for the opponent to use
			// Not sure about that yet though
			/*if (i == pixels_needed-1)
			{
				int weak_pixel_y = ref_pixel_y + i;
				int weak_pixel_x = ref_pixel_x;
			}*/
		}
		// If vert_won is still true, player_checked must have won!
		if (vert_won == true)
			return player_checked;
		 
		 
		///// Check for wins horizontally
		
		hor_won = true;
		for (int i=0; i<pixels_needed; i++)
		{
			if (ref_pixel_x <= grid_size_x - pixels_needed)
			{
				if (grid[ref_pixel_y][ref_pixel_x+i].value != player_checked)
				{
					hor_won = false;
					break;
				}
			}
			else
			{
				hor_won = false;
			}
			/*if (i == pixels_needed-1)
			{
				int weak_pixel_y = ref_pixel_y;
				int weak_pixel_x = ref_pixel_x + i;
			}*/
		}
		 
		if (hor_won == true)
			return player_checked;
		 
		 
		///// Check for wins diagonally
		 
		dia_won = true;
		for (int i=0; i<pixels_needed; i++)
		{
			if (ref_pixel_x <= grid_size_x -pixels_needed && ref_pixel_y <= grid_size_y - pixels_needed)
			{
				if (grid[ref_pixel_y +i][ref_pixel_x+i].value != player_checked)
				{
					dia_won = false;
					break;
				}
			}
			else
			{
				dia_won = false;
			}
		}
		 
		if (dia_won == true)
			return player_checked;
		 
		if (x_parse >= grid_size_x-1)
		{
			x_parse = 0;
			y_parse++;
		}
		else
		{
			x_parse++;
		}
		
	}
	if (vert_won == false && hor_won == false && dia_won == false)
		return 0;
	// Error return value so that the compiler doesn't complain
	return -1;
}
