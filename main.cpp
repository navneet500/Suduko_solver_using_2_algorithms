#include<bits/stdc++.h>
using namespace std;
class SudokuGrid{

	int grid[9][9]; 

	public:SudokuGrid(){
		menu();
	}

	private:void menu(){

		cout<<"\n======================\n";
		cout<<"    Sudoku Solver\n";
		cout<<"======================\n\n";

		cout<<"Welcome to Sudoku Solver!\n";
		cout<<"Before we start, you will have to input the puzzle into this program.\n\n";
		cout<<"You can either:-\n";
		cout<<"\t1. Input the puzzle by entering the values manually. (Enter 1)\n";
		cout<<"\t2. Input the puzzle by reading a file with values in it. (Enter 2)\n";
		cout<<"\t   The file must not have a name > 20 characters.\n";
		cout<<"\t   The file must be in the same directory as this C++ file.\n";
		cout<<"\t   The file must have all 81 values seperated with spaces.\n";
		cout<<"\t   Blank cells must be filled in as 0 (eg; 1 0 0  2 0 0 ...).\n";
		cout<<"\t   --> ";

		int option;
		cin>>option;

		if(option==1) readFromConsole();
		else if(option==2) readFromFile();
		else{
			while(true){
				cout<<"\nYou seem to have entered an invalid option. Try again.\n";
				cout<<"\t   --> ";
				cin>>option;

				if(option==1) readFromConsole();
				else if(option==2) readFromFile();
				else continue;

				break;
			}
		}

	}

	private:void readFromConsole(){
		cout<<"\nEnter the specified value when prompted.\n";
		cout<<"Enter 0 if cell is empty.\n\n";

		int rowIter, colIter;

		for(rowIter=0; rowIter<9; rowIter++){ 
			for(colIter=0; colIter<9; colIter++){
				int enteredValue;

				cout<<"Enter value for cell["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
				cin>>enteredValue;

				if(!(enteredValue>=0 && enteredValue<=9)){ 
					while(true){ 
						cout<<"Oops! You seem to have entered a wrong value! Try again.\n";
						cout<<"Enter value for cell ["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
						cin>>enteredValue;

						if(enteredValue>=0 && enteredValue<=9) break;
					}
				}

				grid[rowIter][colIter]=enteredValue;
			}
			cout<<"-------\n"; 
		}
		cout<<endl;
	}


	private:void readFromFile(){

		char filename[30]; 

		cout<<"\nEnter the name of the file that contains the Sudoku Puzzle.\n";
		cout<<"\t   --> ";
		cin>>filename;

		ifstream input_file; 
		input_file.open(filename,ios::in);

		int rowIter, colIter;

		for(rowIter=0; rowIter<9; rowIter++){ 
			for(colIter=0; colIter<9; colIter++){
				int readValue;

				input_file>>readValue;

				if(!(readValue>=0 && readValue<=9)){ 
					cout<<"\nValue "<<((rowIter*9)+colIter+1)<<" in "<<filename;
					cout<<" seems to be wrong! Correct the value and try again!\n";
					exit(EXIT_FAILURE);
				}

				grid[rowIter][colIter]=readValue;

				if(input_file.eof()) break; 
			}
		}

		

		input_file.close();
		cout<<endl;
	}


	public:void setCellValue(int row, int col, int key){
		grid[row][col]=key;
	}

	public:int getCellValue(int row, int col){
		int cellValue=grid[row][col];
		return cellValue;
	}



	public:void displayGrid(){

		cout<<"++=====================================++";
		int rowIter, colIter;

		for(rowIter=0; rowIter<9; rowIter++){
			int cellIter=1;

			cout<<"\n||";
			for(colIter=0; colIter<9; colIter++){
				if(cellIter==3){
					cout<<" ";
					if(grid[rowIter][colIter] != 0)
						cout<<grid[rowIter][colIter]<<" ";
					else
						cout<<"  ";
					cout<<"||";
					cellIter=1;
				}
				else{
					cout<<" ";
					if(grid[rowIter][colIter] != 0)
						cout<<grid[rowIter][colIter]<<"  ";
					else
						cout<<"   ";
					cellIter++;
				}
			}

			if(rowIter%3!=2) cout<<"\n++-----------++-----------++-----------++";
			else cout<<"\n++=====================================++";
		}

		cout<<" ";
	}

};



class SudokuSolver{

	int recursiveCount;
	SudokuGrid grid; 
	public:SudokuSolver(){
		recursiveCount=0;

		displayGrid();
		cout<<"\n\nCalculating possibilities...\n";
		cout<<"Backtracking across puzzle....\n";
		cout<<"Validating cells and values...\n\n";

		solve();

		displayGrid();
		cout<<"\n\n";
	}


	private:bool cellValueValid(int row, int col, int keyValue){
		int rowIter, colIter;


		for(rowIter=0; rowIter<9; rowIter++){
			if(rowIter == row)
				continue;

			if(grid.getCellValue(rowIter, col) == keyValue)
				return false;
		}

		for(colIter=0; colIter<9; colIter++){
			if(colIter == col)
				continue;

			if(grid.getCellValue(row, colIter) == keyValue)
				return false;
		}

		if( !ThreeByThreeGridValid(row, col, keyValue) )
			return false;


		return true;
	}

	private:bool ThreeByThreeGridValid(int row, int col, int keyValue){
		int rowIter, colIter;

		int s_start[3] = {0, 3, 6};
		int s_end[3] = {3, 6, 9};
		int s_r = int(row / 3);
		int s_c = int(col / 3);


		for(rowIter = s_start[s_r]; rowIter < s_end[s_r]; rowIter++){
			for(colIter = s_start[s_c]; colIter < s_end[s_c]; colIter++){
				if(rowIter == row && colIter == col){
					continue;
				}

				if(grid.getCellValue(rowIter, colIter) == keyValue){
					return false;
				}
			}
		}

		return true;
	}


	private:bool gridSolved(){

		int flag;

		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				if(grid.getCellValue(i, j) == 0)
					return false;

				flag = cellValueValid(i, j, grid.getCellValue(i, j));

				if(flag == 0){
					return false;
				}
			}
		}

		return true;
	}



	private:bool singleCellSolve(){

		statsIncrement(); 

	
		if(gridSolved())
			return true;


		int rowIter, colIter, keyValueIter;

		for(rowIter=0; rowIter<9; rowIter++){
			for(colIter=0; colIter<9; colIter++){

				if(grid.getCellValue(rowIter, colIter) == 0){

				
					for(keyValueIter=1; keyValueIter<10; keyValueIter++){

						if(cellValueValid(rowIter, colIter, keyValueIter)){

							grid.setCellValue(rowIter, colIter, keyValueIter);

			
							if(singleCellSolve())
								return true;

							
							grid.setCellValue(rowIter, colIter, 0);
						}
					}

				
					if(grid.getCellValue(rowIter, colIter) == 0)
						return false;
				}
			}
		}


		return false;

	}


	private:void solve(){
		bool success=singleCellSolve();

		statsPrint();
		if(success)
			cout<<"QED. Your puzzle has been solved!\n\n";		// quod erat demonstrandum [what was to be shown]
		else
			cout <<"There is not any correct answer for this puzzle :( \n";
	}


	private:void displayGrid(){
		grid.displayGrid();
	}

	private:void statsIncrement(){
		recursiveCount++;
	}


	public:void statsPrint(){
		cout<<"\nThe singleCellSolve() function was recursively called "<<recursiveCount<<" times.\n";
	}

};

#define MAX_K 1000
//#define SIZE 16

#define SIZE 9
//#define SIZE 4

struct Node {

	Node *left;
	Node *right;
	Node *up;
	Node *down;
	Node *head;
	
	int size;		//used for Column header
	int rowID[3];	//used to identify row in order to map solutions to a sudoku grid
					//ID Format: (Candidate, Row, Column)
};

const int SIZE_SQUARED = SIZE*SIZE;
const int SIZE_SQRT = sqrt((double)SIZE);
const int ROW_NB = SIZE*SIZE*SIZE;
const int COL_NB = 4 * SIZE*SIZE;

struct Node Head;
struct Node* HeadNode = &Head;
struct Node* solution[MAX_K];
struct Node* orig_values[MAX_K];
bool matrix[ROW_NB][COL_NB] = { { 0 } };
bool isSolved = false;
void MapSolutionToGrid(int Sudoku[][SIZE]);
void PrintGrid(int Sudoku[][SIZE]);

clock_t timer, timer2;

//===============================================================================================================//
//---------------------------------------------DLX Functions-----------------------------------------------------//
//===============================================================================================================//

void coverColumn(Node* col) {
	col->left->right = col->right;
	col->right->left = col->left;
	for (Node* node = col->down; node != col; node = node->down) {
		for (Node* temp = node->right; temp != node; temp = temp->right) {
			temp->down->up = temp->up;
			temp->up->down = temp->down;
			temp->head->size--;
		}
	}
}

void uncoverColumn(Node* col) {
	for (Node* node = col->up; node != col; node = node->up) {
		for (Node* temp = node->left; temp != node; temp = temp->left) {
			temp->head->size++;
			temp->down->up = temp;
			temp->up->down = temp;
		}
	}
	col->left->right = col;
	col->right->left = col;
}

void search(int k) {

	if (HeadNode->right == HeadNode) {
		timer2 = clock() - timer;
		int Grid[SIZE][SIZE] = { {0} };
		MapSolutionToGrid(Grid);
		PrintGrid(Grid);
		std::cout << "Time Elapsed: " << (float)timer2 / CLOCKS_PER_SEC << " seconds.\n\n";
		std::cin.get(); //Pause console
		timer = clock();
		isSolved = true;
		return;
	}

	//Choose Column Object Deterministically: Choose the column with the smallest Size
	Node* Col = HeadNode->right;
	for (Node* temp = Col->right; temp != HeadNode; temp = temp->right)
		if (temp->size < Col->size)
			Col = temp;

	coverColumn(Col);

	for (Node* temp = Col->down; temp != Col; temp = temp->down) {
		solution[k] = temp;
		for (Node* node = temp->right; node != temp; node = node->right) {
			coverColumn(node->head);
		}

		search(k + 1);

		temp = solution[k];
		solution[k] = NULL;
		Col = temp->head;
		for (Node* node = temp->left; node != temp; node = node->left) {
			uncoverColumn(node->head);
		}
	}

	uncoverColumn(Col);
}

//===============================================================================================================//
//----------------------Functions to turn a Sudoku grid into an Exact Cover problem -----------------------------//
//===============================================================================================================//

//--------------------------BUILD THE INITIAL MATRIX CONTAINING ALL POSSIBILITIES--------------------------------//
void BuildSparseMatrix(bool matrix[ROW_NB][COL_NB]) {

	//Constraint 1: There can only be one value in any given cell
	int j = 0, counter = 0;
	for (int i = 0; i < ROW_NB; i++) { //iterate over all rows
		matrix[i][j] = 1;
		counter++;
		if (counter >= SIZE) {
			j++;
			counter = 0;
		}
	}

	//Constraint 2: There can only be one instance of a number in any given row
	int x = 0;
	counter = 1;
	for (j = SIZE_SQUARED; j < 2 * SIZE_SQUARED; j++) {
		for (int i = x; i < counter*SIZE_SQUARED; i += SIZE)
			matrix[i][j] = 1;

		if ((j + 1) % SIZE == 0) {
			x = counter*SIZE_SQUARED;
			counter++;
		}
		else
			x++;
	}

	//Constraint 3: There can only be one instance of a number in any given column
	j = 2 * SIZE_SQUARED;
	for (int i = 0; i < ROW_NB; i++)
	{
		matrix[i][j] = 1;
		j++;
		if (j >= 3 * SIZE_SQUARED)
			j = 2 * SIZE_SQUARED;
	}

	//Constraint 4: There can only be one instance of a number in any given region
	x = 0;
	for (j = 3 * SIZE_SQUARED; j < COL_NB; j++) {

		for (int l = 0; l < SIZE_SQRT; l++) {
			for (int k = 0; k<SIZE_SQRT; k++)
				matrix[x + l*SIZE + k*SIZE_SQUARED][j] = 1;
		}

		int temp = j + 1 - 3 * SIZE_SQUARED;

		if (temp % (int)(SIZE_SQRT * SIZE) == 0)
			x += (SIZE_SQRT - 1)*SIZE_SQUARED + (SIZE_SQRT - 1)*SIZE + 1;
		else if (temp % SIZE == 0)
			x += SIZE*(SIZE_SQRT - 1) + 1;
		else
			x++;
	}
}

//-------------------BUILD A TOROIDAL DOUBLY LINKED LIST OUT OF THE SPARSE MATRIX-------------------------//
void BuildLinkedList(bool matrix[ROW_NB][COL_NB]) {

	Node* header = new Node;
	header->left = header;
	header->right = header;
	header->down = header;
	header->up = header;
	header->size = -1;
	header->head = header;
	Node* temp = header;

	//Create all Column Nodes
	for (int i = 0; i < COL_NB; i++) {
		Node* newNode = new Node;
		newNode->size = 0;
		newNode->up = newNode;
		newNode->down = newNode;
		newNode->head = newNode;
		newNode->right = header;
		newNode->left = temp;
		temp->right = newNode;
		temp = newNode;
	}

	int ID[3] = { 0,1,1 };
	//Add a Node for each 1 present in the sparse matrix and update Column Nodes accordingly
	for (int i = 0; i < ROW_NB; i++) {
		Node* top = header->right;
		Node* prev = NULL;

		if (i != 0 && i%SIZE_SQUARED == 0) {
			ID[0] -= SIZE - 1;
			ID[1]++;
			ID[2] -= SIZE - 1;
		}
		else if (i!= 0 && i%SIZE == 0) {
			ID[0] -= SIZE - 1;
			ID[2]++;
		}
		else {
			ID[0]++;
		}

		for (int j = 0; j < COL_NB; j++, top = top->right) {
			if (matrix[i][j]) {
				Node* newNode = new Node;
				newNode->rowID[0] = ID[0];
				newNode->rowID[1] = ID[1];
				newNode->rowID[2] = ID[2];
				if (prev == NULL) {
					prev = newNode;
					prev->right = newNode;
				}
				newNode->left = prev;
				newNode->right = prev->right;
				newNode->right->left = newNode;
				prev->right = newNode;
				newNode->head = top;
				newNode->down = top;
				newNode->up = top->up;
				top->up->down = newNode;
				top->size++;
				top->up = newNode;
				if (top->down == top)
					top->down = newNode;
				prev = newNode;
			}
		}
	}

	HeadNode = header;
}

//-------------------COVERS VALUES THAT ARE ALREADY PRESENT IN THE GRID-------------------------//
void TransformListToCurrentGrid(int Puzzle[][SIZE]) {
	int index = 0;
	for(int i = 0 ; i<SIZE; i++ )
		for(int j = 0 ; j<SIZE; j++)
			if (Puzzle[i][j] > 0) {
				Node* Col = NULL;
				Node* temp = NULL;
				for (Col = HeadNode->right; Col != HeadNode; Col = Col->right) {
					for (temp = Col->down; temp != Col; temp = temp->down)
						if (temp->rowID[0] == Puzzle[i][j] && (temp->rowID[1] - 1) == i && (temp->rowID[2] - 1) == j)
							goto ExitLoops;
				}
ExitLoops:		coverColumn(Col);
				orig_values[index] = temp;
				index++;
				for (Node* node = temp->right; node != temp; node = node->right) {
					coverColumn(node->head);
				}

			}

}

//===============================================================================================================//
//----------------------------------------------- Print Functions -----------------------------------------------//
//===============================================================================================================//

void MapSolutionToGrid(int Sudoku[][SIZE]) {
	
	for (int i = 0; solution[i] != NULL; i++) {
			Sudoku[solution[i]->rowID[1]-1][solution[i]->rowID[2]-1] = solution[i]->rowID[0];
	}
	for (int i = 0; orig_values[i] != NULL; i++) {
		Sudoku[orig_values[i]->rowID[1] - 1][orig_values[i]->rowID[2] - 1] = orig_values[i]->rowID[0];
	}
}

//---------------------------------PRINTS A SUDOKU GRID OF ANY SIZE---------------------------------------------//
void PrintGrid(int Sudoku[][SIZE]){
	std::string ext_border = "+", int_border = "|";
	int counter = 1;
	int additional = 0;
	if (SIZE > 9)
		additional = SIZE;
	for (int i = 0; i < ((SIZE +SIZE_SQRT - 1) * 2 +additional+ 1); i++) {
		ext_border += '-';
		
		if (i > 0 && i % ((SIZE_SQRT*2+SIZE_SQRT*(SIZE>9)+1)*counter + counter-1) == 0) {
			int_border += '+';
			counter++;
		}
		else
			int_border += '-';
	}
	ext_border += '+';
	int_border += "|";

	std::cout << ext_border << std::endl;
	for (int i = 0; i<SIZE; i++){
		std::cout << "| ";
		for (int j = 0; j<SIZE; j++){
			if (Sudoku[i][j] == 0)
				std::cout << ". ";
			else
				std::cout << Sudoku[i][j] << " ";
			if (additional > 0 && Sudoku[i][j]<10)
				std::cout << " ";
			if ((j+1)%SIZE_SQRT == 0)
				std::cout << "| ";
		}
		std::cout << std::endl;
		if ((i + 1) % SIZE_SQRT == 0 && (i+1)<SIZE)
			std::cout << int_border << std::endl;
	}
	std::cout << ext_border << std::endl << std::endl;
}

//--------------------------------------------------------------------------------//

void SolveSudoku(int Sudoku[][SIZE]) {
	timer = clock();
	BuildSparseMatrix(matrix);
	BuildLinkedList(matrix);
	TransformListToCurrentGrid(Sudoku);
	search(0);
	if (!isSolved)
		cout << "No Solution!" << endl;
	isSolved = false;
}

int main()
{
    cout<<"ENTER YOUR CHOICE : "<<endl<<"1 . SOLVE SUDUKO USING BACKTRACKING"<<endl<<"2 . SOLVE SUDUKO USING DANCING LINKS ALGORITHM"<<endl;
    int xx;
    cin>>xx;
    switch (xx)
{
    case 1: 
    {
        SudokuSolver ss;
        break;
    }
        
    case 2: 
    {
        int Puzzle[9][9] =		{	{ 0,0,0,  0,0,0,  0,0,0 },
									{ 0,0,0,  0,0,3,  0,8,5 },
									{ 0,0,1,  0,2,0,  0,0,0 },

									{ 0,0,0,  5,0,7,  0,0,0 },
									{ 0,0,4,  0,0,0,  1,0,0 },
									{ 0,9,0,  0,0,0,  0,0,0 },

									{ 5,0,0,  0,0,0,  0,7,3 },
									{ 0,0,2,  0,1,0,  0,0,0 },
									{ 0,0,0,  0,4,0,  0,0,9 }
								};



	int EmptyPuzzle[SIZE][SIZE] = { {0} };

	SolveSudoku(Puzzle);
        break;
    }

}


}