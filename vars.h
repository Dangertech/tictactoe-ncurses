// Pack all important variables into a header file to make them accessible
#include <vector>

// Class for each Tile on the grid
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
std::vector < std::vector<Pixel> > grid;
