
	#include "Pathfinder.h"
	string Pathfinder::toString() const
	{
	    stringstream ss;
		for(int layer = 0; layer< LAYER_SIZE; layer++){	
	    	for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					ss << maze_grid[layer][row][col];
					if(col < 4){
						ss << " ";
					}
				}
				ss << endl;
	    	}
			if(layer < 4)
			{
				ss << endl;
			}
	    	
		}
		return ss.str();
	}

	/*
	* createRandomMaze
	*
	* Generates a random maze and stores it as the current maze.
	*
	* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
	* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
	* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
	*/
	void Pathfinder::createRandomMaze()
	{//	cout << "making random maze" << endl; 

		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 5; j++){
				for(int k = 0; k < 5; k++){
					maze_grid[i][j][k] = rand() % 2;
				}
			}
		}
		maze_grid[0][0][0] = 1; 
		maze_grid[4][4][4] = 1;
		cout << "printing generated maze" << endl; 
		for(int i = 0; i < LAYER_SIZE; i++){
			for(int j = 0; j < ROW_SIZE; j++){
				for(int k = 0; k < COL_SIZE; k++){
					cout << maze_grid[i][j][k]; 
					if(k < 5){
						cout << " ";
					}
				}
				if(j < 5){
					cout << endl; 
				}
			}
			if(i < 5){
				cout << endl; 
			}
		}
	}
	//-----------------------------------------------------------------------------------------

	//Part 2-----------------------------------------------------------------------------------
	/*
	* importMaze
	*
	* Reads in a maze from a file with the given file name and stores it as the current maze.
	* Does nothing if the file does not exist or if the file's data does not represent a valid
	* maze.
	*
	* The file's contents must be of the format described above to be considered valid.
	*
	* Parameter:	file_name
	*				The name of the file containing a maze
	* Returns:		bool
	*				True if the maze is imported correctly; false otherwise
	*/
	bool Pathfinder::importMaze(string file_name)
	{ // check how many inputs
		//cout << "importMaze from "<<file_name<<endl;
		//cout << sizeof(maze_grid) << endl; 
		int temp_maze[LAYER_SIZE][COL_SIZE][ROW_SIZE];
		//ifstream test(file_name.c_str());

			ifstream file (file_name.c_str());
			if(file.fail())
				return false; 
			
			if (file.is_open()) {
				
				for(int layer = 0; layer < LAYER_SIZE; layer++ ){
					for(int row = 0; row < ROW_SIZE; row++) {
						string line;
						getline(file, line);
						if(line.empty()){
							getline(file, line); 
						}
						stringstream ss(line);
						for(int col = 0; col < COL_SIZE; col++) {
							int newString;
							ss >> newString;
							if(ss.fail())
								return false; 

					
						// cout << "["<<row<<"]["<<col<<"]="<<value<<endl;
							
							
							if((newString != 0) && (newString != 1)){ // check to make sure all values are 0s and 1s
								return false;
							}
							
							temp_maze[layer][row][col] = newString;
						}
					}
					if((layer < 4) && (file.eof())){ // checks to see if the matrix is too small 
						//cout << "maze is out of bounds" << endl; 
						return false; 
					}
					if(layer == 4 && !file.eof()){
						//cout << "maze is out of bounds" << endl; // checks to see if the matrix is too big
						return false;
					}
	
			}
		if(temp_maze[LAYER_SIZE-1][ROW_SIZE-1][COL_SIZE-1]!= 1){
						return false; // if there is no exit
					}
		for(int layer = 0; layer< LAYER_SIZE; layer++){	
	    	for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					maze_grid[layer][row][col] = temp_maze[layer][row][col]; 				
					}
				
	    	}
		
		}
		}
	    return(true);
	
	}
		

	//-----------------------------------------------------------------------------------------
	bool Pathfinder::find_maze_path(int grid[ROW_SIZE][COL_SIZE][LAYER_SIZE], int r, int c, int l){
		//cout << "finding maze path:" << endl; 
		//cout << this->toString(); 

		if(r < 0 || c < 0 || r>= ROW_SIZE || c >= COL_SIZE || l >= LAYER_SIZE){
			//cout << "out of bounds error" << endl; // check to see if the matrix is in bounds
			return false; 
		}
		else if(grid[l][c][r] != BACKGROUND){
			return false; 
		}
		else if(r == ROW_SIZE-1 && (c == COL_SIZE-1) && (l == LAYER_SIZE-1)){
			grid[l][c][r] = PATH; 
			solution.push_back("("+to_string(r)+", "+to_string(c)+", "+to_string(l)+")");
			return true; // and then exit the maze
		}
		else{
			// this will be the inductive step 
			grid[l][c][r] = PATH; // this will set where we have been to 3 to keep track of the path
			if(find_maze_path(grid,r-1, c, l) || find_maze_path(grid, r+1, c, l)
			 || find_maze_path(grid, r, c-1, l) || find_maze_path(grid, r, c+1, l)
			  || find_maze_path(grid, r, c, l-1) || find_maze_path(grid, r, c, l+1) ){
				solution.push_back("("+to_string(r)+", "+to_string(c)+", "+to_string(l)+")");
				return true; 
			  }else{
				grid[l][c][r] = TEMPORARY; 
				return false; 
			  }
		}

	}

	
	
	// bool Pathfinder::find_maze_path(int grid[ROW_SIZE][COL_SIZE][LAYER_SIZE], int r, int c, int l) {
	//   cout << "find_maze_path ["<<r<<"]["<<c<<"]"<<endl;
	//   cout << this->toString();
	//   if (r < 0 || c < 0 || r >= ROW_SIZE || c >= COL_SIZE || l < 0 || l >= LAYER_SIZE)
	//     return false;      // Cell is out of bounds.
	//   else if (grid[r][c] != BACKGROUND)
	//     return false;      // Cell is on barrier or dead end.
	//   else if (r == ROW_SIZE - 1 && c == COL_SIZE - 1 && l == LAYER_SIZE-1) {
	//     grid[r][c][l] = PATH;         // Cell is on path
	//     solution.push_back("("+to_string(r)+","+to_string(c)+","+to_string(l)+")");
	//     return true;               // and is maze exit.
	//   }
	//   else { 
	//     // Recursive case.
	//     // Attempt to find a path from each neighbor.
	//     // Tentatively mark cell as on path.
	//     grid[r][c] = PATH;
	//     if (find_maze_path(grid, r - 1, c)
	//         || find_maze_path(grid, r + 1, c)
	//         || find_maze_path(grid, r, c - 1)
	//         || find_maze_path(grid, r, c + 1 ) ) {
	//       solution.push_back("("+to_string(r)+","+to_string(c)+")");
	//       return true;
	//     }else {
	//       grid[r][c] = TEMPORARY;  // Dead end.
	//       return false;
	//     }
	//   }
	// }
	  //Part 3-----------------------------------------------------------------------------------
	/*
	* solveMaze
	*
	* Attempts to solve the current maze and returns a solution if one is found.
	*
	* A solution to a maze is a list of coordinates for the path from the entrance to the exit
	* (or an empty vector if no solution is found). This list cannot contain duplicates, and
	* any two consecutive coordinates in the list can only differ by 1 for only one
	* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
	* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
	* where x, y, and z are the integer coordinates of a cell.
	*
	* Understand that most mazes will contain multiple solutions
	*
	* Returns:		vector<string>
	*				A solution to the current maze, or an empty vector if none exists
	*/
	
	vector<string> Pathfinder::solveMaze()
	{	//cout << "solving maze" << endl; 
		int temp_maze[LAYER_SIZE][ROW_SIZE][COL_SIZE];
		for(int layer = 0; layer< LAYER_SIZE; layer++){	
	    	for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					temp_maze[layer][row][col] = maze_grid[layer][row][col]; 				
					}
				
	    	}
		
		}
		solution.clear(); 
		find_maze_path(temp_maze, 0, 0, 0 );
		reverse(solution.begin(), solution.end());
		for(auto s:solution) {
		//	cout <<s << endl;
		}
	    return solution;
	}
