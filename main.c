/*****************************
* Class: CS 107, Spr 2018
* Univerity of Illinois at Chicago
* System: OnlineGBD, MacOS *
* @author Chuangyi Zhang
* @version April 29, 2018


*From Wikipedia https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
*
* The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells, 
each of which is in one of two possible states, alive or dead, or "populated" or "unpopulated". 
Every cell interacts with its eight neighbours, which are the cells that are horizontally, 
vertically, or diagonally adjacent. At each step in time, the following transitions occur:
*
* 
1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*
* The initial pattern constitutes the seed of the system. 
The first generation is created by applying the above rules simultaneously to every cell in 
the seedbbirths and deaths occur simultaneously, and the discrete moment at which this happens 
is sometimes called a tick (in other words, each generation is a pure function of the preceding one). 
The rules continue to be applied repeatedly to create further generations.
* 
* Class: CS 107, Spr 2018
* Univerity of Illinois at Chicago
* System: OnlineGBD, MacOS *
* @author Chuangyi Zhang
* @version April 29, 2018

*******************************************************************************/
#include <stdio.h>

struct node
{
  int rows;
  int columns;
  int *grid;			// a pointer to the grid that should be of size rows*columns
  struct node *next_step;	// a pointer to the node that holds the grid for the next time step
};

// This function reads data from file pointer input and return a dynamically created node with the data 
// and the number of time steps to simulate in numsteps
struct node *
createInitialNode (FILE * input, int *numsteps)
{
  int r = 0;
  int c = 0;

  struct node *initialNode = NULL;
  initialNode = (struct node *) malloc (sizeof (struct node));
  initialNode->next_step = NULL;

  fscanf (input, "%d", numsteps);	// store the file input for numsteps

  fscanf (input, "%d", &initialNode->rows);	// store the file input for rows 

  fscanf (input, "%d", &initialNode->columns);	// store the file input for columns

  initialNode->grid =
    (int *) calloc (initialNode->rows * initialNode->columns, sizeof (int));

  while (fscanf (input, "%d %d", &r, &c) != EOF)
    {
      if (r > (initialNode->rows) - 1)
	{
	  printf ("ERROR with initial point values\n");	// check if it out of the gridside bound then return ERROR
	  return NULL;
	}
      if (c > (initialNode->columns) - 1)
	{			/// check if it out of the gridside bound then return ERROR
	  printf ("ERROR with initial point values\n");
	  return NULL;
	}
      int position = (initialNode->columns) * r + c;
      *(initialNode->grid + position) = 1;
    }

  return initialNode;
}

//This function adds a struct node to the end of the list pointed to by begin for the next time step, 
//so use data in the last node in the list to create the new timestep. 
void
nextStep (struct node *begin)
{

  int numOfAliveNeighbors = 0;

  struct node *temp = begin;
  temp = (struct node *) malloc (sizeof (struct node));

  temp->rows = begin->rows;
  temp->columns = begin->columns;
  temp->next_step = NULL;
  temp->grid = (int *) calloc (begin->columns * begin->rows, sizeof (int));


  struct node *newNode = begin;
  while (newNode->next_step != NULL)
    {
      newNode = newNode->next_step;
    }

  newNode->next_step = temp;

// check the neighbors if it is alive or die for next step 
    for (int r = 0; r < begin->rows; r++){
        for (int c = 0; c < begin->columns; c++){
	        numOfAliveNeighbors = neighbors (newNode, r, c);
	        int position = (temp->columns) * r + c;
	        if (*(newNode->grid + (temp->columns) * r + c) == 1){
	                switch (numOfAliveNeighbors)  {
		                case 2:
		                    *(temp->grid + position) = 1;	// number of neighbors is 2 alive 
		                     break;
		                case 3:
		                    *(temp->grid + position) = 1;	// number of  neighbors is 3  alive 
                             break;
		                default:
		                    *(temp->grid + position) = 0;	// die if have less than 2 or larger than 3
		                     break;
		 }
	}
	else{
	     if (numOfAliveNeighbors == 3)	// if it surround by 3 neighbors it becomes alive 
		*(temp->grid + position) = 1;
	    }
	}
    }
  return;

}

// print out all the grids contained in the list pointed to by begin.

void printList (struct node *begin) {
    int position;
    while (begin != NULL){
      for (int r = 0; r < begin->rows; r++){
	    for (int c = 0; c < begin->columns; c++){
	      position = (begin->columns) * r + c;
	      printf ("%d", *(begin->grid + position));
	    }
	  printf ("\n");
	}
      begin = begin->next_step;	// go to the next list 
      printf ("\n");
    }
}


 // This function return the number of alive neighbor
int neighbors (struct node *begin, int r, int c) {
    int numOfAliveNeighbors = 0;
    if ((r == 0 && c == 0) || (r == 0 && c == (begin->columns - 1))
      || (r == (begin->rows - 1) && c == 0) || (r == (begin->rows - 1) //check the position at the corner 
						&& c == (begin->columns - 1)))
    numOfAliveNeighbors = cornerNeighbors (begin, r, c);

  // check the position at the edge
  else if ((r == 0 && c <= (begin->columns - 2)) || (r == (begin->rows - 1) && c <= (begin->columns - 2)) || (r <= (begin->rows - 2) && c == 0) || (r <= (begin->rows - 2) && c == (begin->columns - 1)))	//check if the cell is by the border
    numOfAliveNeighbors = borderNeighbor (begin, r, c);

  // check the normal position
  else
    {
      numOfAliveNeighbors =
	*(begin->grid + (begin->columns) * (r - 1) + (c - 1)) +
	*(begin->grid + (begin->columns) * (r - 1) + (c)) + *(begin->grid +
							      (begin->
							       columns) * (r -
									   1)
							      + (c + 1)) +
	*(begin->grid + (begin->columns) * (r) + (c - 1));
      numOfAliveNeighbors =
	numOfAliveNeighbors + *(begin->grid + (begin->columns) * (r) +
				(c + 1)) + *(begin->grid +
					     (begin->columns) * (r + 1) + (c -
									   1))
	+ *(begin->grid + (begin->columns) * (r + 1) + (c)) + *(begin->grid +
								(begin->
								 columns) *
								(r + 1) + (c +
									   1));
    }
  return numOfAliveNeighbors;
}

// this fucntion return the number of alive neighbor by the corner 
int cornerNeighbors (struct node *begin, int r, int c) {
    int numOfAliveNeighbors = 0;

  // top left corner 
    if (r == 0 && c == 0)
        numOfAliveNeighbors =
        *(begin->grid + (begin->columns) * (r + 1) + (c)) + *(begin->grid +
							    (begin->columns) *
							    (r) + (c + 1)) +
        *(begin->grid + (begin->columns) * (r + 1) + (c + 1));

  // top right corner 
    else if (r == 0 && c == (begin->columns - 1))
        numOfAliveNeighbors =
        *(begin->grid + (begin->columns) * (r) + (c - 1)) + *(begin->grid +
							    (begin->columns) *
							    (r + 1) + (c -
								       1)) +
        *(begin->grid + (begin->columns) * (r + 1) + (c));

  // button left corner
    else if (r == (begin->rows - 1) && c == 0)
        numOfAliveNeighbors =
        *(begin->grid + (begin->columns) * (r - 1) + (c)) + *(begin->grid +
							    (begin->columns) *
							    (r) + (c + 1)) +
        *(begin->grid + (begin->columns) * (r - 1) + (c + 1));

  // button right corner 
    else if (r == (begin->rows - 1) && c == (begin->columns - 1))
        numOfAliveNeighbors =
        *(begin->grid + (begin->columns) * (r - 1) + (c - 1)) + *(begin->grid +
								(begin->
								 columns) *
								(r) + (c -
								       1)) +
        *(begin->grid + (begin->columns) * (r - 1) + (c));
  return numOfAliveNeighbors;
}

// this function return the number of alive neighbor by border 
    int borderNeighbor (struct node *begin, int r, int c)
{

  int numOfAliveNeighbors = 0;	//initialize the numOfAliveNeighbors

  // top border 
  if (r == 0)
    numOfAliveNeighbors =
      *(begin->grid + (begin->columns) * (r) + (c - 1)) + *(begin->grid +
							    (begin->columns) *
							    (r + 1) + (c -
								       1)) +
      *(begin->grid + (begin->columns) * (r + 1) + (c)) + +*(begin->grid +
							     (begin->
							      columns) * (r +
									  1) +
							     (c + 1)) +
      *(begin->grid + (begin->columns) * (r) + (c + 1));

  //bottom border
  else if (r == (begin->rows - 1))
    numOfAliveNeighbors =
      *(begin->grid + (begin->columns) * (r - 1) + (c - 1)) + *(begin->grid +
								(begin->
								 columns) *
								(r - 1) +
								(c)) +
      *(begin->grid + (begin->columns) * (r - 1) + (c + 1)) + +*(begin->grid +
								 (begin->
								  columns) *
								 (r) + (c -
									1)) +
      *(begin->grid + (begin->columns) * (r) + (c + 1));

  //left border
  else if (c == 0)
    numOfAliveNeighbors =
      *(begin->grid + (begin->columns) * (r - 1) + (c)) + *(begin->grid +
							    (begin->columns) *
							    (r + 1) + (c)) +
      *(begin->grid + (begin->columns) * (r - 1) + (c + 1)) + +*(begin->grid +
								 (begin->
								  columns) *
								 (r) + (c +
									1)) +
      *(begin->grid + (begin->columns) * (r + 1) + (c + 1));
  // right border
  else if (c == (begin->columns - 1))
    numOfAliveNeighbors =
      *(begin->grid + (begin->columns) * (r - 1) + (c)) + *(begin->grid +
							    (begin->columns) *
							    (r + 1) + (c)) +
      *(begin->grid + (begin->columns) * (r - 1) + (c - 1)) + +*(begin->grid +
								 (begin->
								  columns) *
								 (r) + (c -
									1)) +
      *(begin->grid + (begin->columns) * (r + 1) + (c - 1));

  return numOfAliveNeighbors;
}


int
main ()
{
  FILE *inFile;
  int numsteps;
  char *fileName = (char *) malloc (50 * sizeof (char));
  struct node *begin = NULL;
  struct node *temp = (struct node *) malloc (sizeof (struct node));

  printf (" Welcome to Conway's game of Life\n");
  printf ("please enter filename:\n");

  scanf ("%s", &fileName);

  inFile = fopen (&fileName, "r");

  if (inFile == NULL)
    {
      printf ("ERROR opening filename %s", &fileName);	// if cannot open the file that return error and exit programs
      return -1;
    }

  begin = createInitialNode (inFile, &numsteps);	// initialize the node 

  for (int i = 0; i < numsteps; i++)
    {
      nextStep (begin);		// run the next step for number of steps 
    }

  printList (begin);		//print out the grid 

  return 0;
}
