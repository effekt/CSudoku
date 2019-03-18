/*"I declare that the attached assignment is wholly my own work in accordance
with Seneca Academic Policy. No part of this assignment has been copied
manually or electronically from any other source (including web sites) or
distributed to other students."

Name   Jesse Wheeler
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 9

int genSudokuNumbers(int grid[]); //DONE
void genSudokuBoard(int grid[], int display[]); //DONE
void displaySudokuBoard(int grid[], int bigOh); //DONE
void printSudokuBoardToFile(int display[], const char *fileName, int difficulty);
void getRandNum(int low, int high, int *pn); //DONE
int checkDups(int nums[], int num, int n); //DONE
void clearArray(int nums[], int n); //DONE

int main() {
	srand(time(NULL));
	int i, grid[N*N];
	int iterations, count = 0;
	char file_name[25] = "sudoku.txt";
	int difficulty = 0;

	printf("\nPLAY IPC144 SUDOKU");
	//Request difficulty of game from user
	do {
		printf("\nChoose a difficulty:\n%-20s%-20s%-20s\n", "[ 1 ] Easy", "[ 2 ] Medium", "[ 3 ] Hard");
		scanf("%d", &difficulty);
	} while(difficulty <= 0 && difficulty >= 4);
	
	iterations = genSudokuNumbers(grid);
	displaySudokuBoard(grid, iterations);
	printSudokuBoardToFile(grid, file_name, difficulty);
	return 0;
}

void printSudokuBoardToFile(int display[], const char *fileName, int difficulty) {
	int test[9][9];
	int x = 0, y = 0, i = 0, startx = 0, starty = 0, show = 0, shown = 0;
	int count = 0;
	int numShown = 0;
	int numHidden = 0;
	FILE *sudFile = fopen(fileName, "w");
	
	//Initialize 2d array with 0-80 in correspondance to game array
	for (x = 0; x < N; x++) {
		for (y = 0; y < N; y++) {
			test[x][y] = count;
			count++;
		}
	}

	//Loop through each grid of 3x3 to determine which numbers to be hidden
	for (i; i < N; i++) {
		startx = i / 3 * 3;
		if (i != 0) {
			starty = i % 3 * 3;
		}
		shown = 0;
		numShown = 0;
		
		if (difficulty == 1) { //Easy - hide 3-4 numbers from each block
			while(numShown < 3 || numShown > 4) { 
				getRandNum(1, 4, &numShown);
			}
		}
		
		if (difficulty == 2) { //Medium - hide 4-6 numbers from each block
			while(numShown < 4 || numShown > 6) { 
				getRandNum(1, 6, &numShown);
			}
		}
		if (difficulty == 3) { //Hard - hide 6-8 numbers from each block
			while(numShown < 6 || numShown > 8) { 
				getRandNum(1, 8, &numShown);
			}
		}
		
		//Loop through 3x3 grid
		do {
			for (x = startx; x < startx + 3; x++) {
				if (shown != numShown) {
					for (y = starty; y < starty + 3; y++) {
						getRandNum(0, 2, &numHidden);
						if (numHidden == 1 && shown != numShown && test[x][y] != 333) {
							test[x][y] = 333; //set block to hidden
							shown++;
						}
					}
				}
			}
		} while (shown != numShown);
	}

	count = 0;
	
	//Print the game board to file based on numbers hidden previously. Comparing each
	//member of the 9x9 array to the game array.
	fprintf(sudFile, "+-----+-----+-----+\n");
	for (x = 0; x < N; x++) {
		for (y = 0; y < N; y++) {
			switch (y % 3) {
				case 0: if (test[x][y] != 333) { //if hidden, don't display
							fprintf(sudFile, "|%d", display[count]);
						} else {
							fprintf(sudFile, "| ");
						}
					break;
				case 1: if (test[x][y] != 333) {
							fprintf(sudFile, " %d ", display[count]);
						} else {
							fprintf(sudFile, "   ");
						}
					break;
				case 2: if (test[x][y] != 333) {
							fprintf(sudFile, "%d", display[count]);
						} else {
							fprintf(sudFile, " ");
						}
					break;
			}
			count++;
		}
		fprintf(sudFile, "|\n");
		if (count % 27 == 0 && x != 0) {
			fprintf(sudFile, "+-----+-----+-----+\n");
		}
	}
	fclose(sudFile);
}

void displaySudokuBoard(int grid[], int bigOh) {
	int i = 0;
	int count = 0;
	printf("+-----+-----+-----+\n");
	for (i; i < N*N; i++) {
		switch (count % 3) {
			case 0: printf("|%d", grid[i]);
				break;
			case 1: printf(" %d ", grid[i]);
				break;
			case 2: printf("%d", grid[i]);
				break;
		}
		count++;
		if (count % 9 == 0) {
			printf("|\n");
		}
		if (count % 27 == 0 && i != 0) {
                        printf("+-----+-----+-----+\n");
                }
	}
	printf("\nTotal Instructions:%d\n", bigOh);
}

int checkDups(int nums[], int num, int n) {
	int i = 0, valid = 0;

	//Loop through array to make sure no duplicates are present
	for (i; i < n; i++) {
		if (nums[i] == num) {
			valid = 1;
		}
	}
	return valid;
}

void clearArray(int nums[], int n) {
	int i = 0;

	//Set each member in nums[] to 0
	for (i; i < n; i++) {
		nums[i] = 0;
	}
}

void getRandNum(int low, int high, int *pn) {
	*pn = rand( ) % high + low; //Generate random number
}

int genSudokuNumbers(int grid[ ]) {
	   int c, i, j, rowNum, colNum, blockNum;
	   int test[N], dup, temp, valid, cnt, iterations=0;

	   for(i=0; i<N*N; i++) {     /* initializing the grid array to all 0's */
		  grid[i] = 0;
	   }

	   for(c=0; c<N*N; c++) {     /* main loop to generate N*N numbers for the grid */
			getRandNum(1, 9, &temp); /* generate random # from 1 to N inclusive */

			valid = dup = cnt = 0;
			while(!valid) {         /* keep looping as long as the 'valid' flag is false */
			clearArray(test, N);
					  /* the calculations below determine the row, col,
					 and block numbers (index 0 to 8) based on the
					 linear index variable 'c' */
			rowNum = c / N;
			colNum = c % N;
			blockNum = (rowNum / 3) * 3 + (colNum / 3);

					  /* now check to see if the number 'temp' is a
					 duplicate in the row, column, and block to which
					 'c' corresponds */

			for(j=0; j<colNum; j++) {    /* fill row (but only up to colNum) */
				test[j] = grid[rowNum*N+j];
			}
			dup += checkDups(test, temp, colNum);
			iterations++;

			 if(!dup) {                   /* row is valid, now check column */
				clearArray(test, N);
				for(j=0; j<rowNum; j++) { /* fill column (but only up to rowNum) */
					test[j] = grid[j*N + colNum];
					iterations++;
				}
				dup += checkDups(test, temp, rowNum);
				iterations++;

				if(!dup) {                /* column is valid now check block */
					clearArray(test, N);

					for(j=0; j<N; j++) {
						test[j] = grid[((blockNum/3)*N*3) + (colNum/3)*3 + (j/3)*N + j%3];
					}
								  /* equation used to generate array
								 coordinates for all N blocks
								 (i.e.)
								 0, 1, 2,   9, 10, 11,  18, 19, 20  {blk 0}
								 3, 4, 5,  12, 13, 14,  21, 22, 23  {blk 1}
								 6, 7, 8,  15, 16, 17,  24, 25, 26  {blk 2}

								 27, 28, 29, 36, 37, 38, 45, 46, 47 {blk 3}
								 30, 31, 32, 39, 40, 41, 48, 49, 50
								 33, 34, 35, 42, 43, 44, 51, 52, 53

								 54, 55, 56, 63, 64, 65, 72, 73, 74
								 57, 58, 59, 66, 67, 68, 75, 76, 77
								 60, 61, 62, 69, 70, 71, 78, 79, 80 {blk 8}
								  */
					dup += checkDups(test, temp, N);
					iterations++;
				}
			}
		 if(!dup) { /* no duplicates in row, column, or block, so number
				   can be inserted into the grid */
			grid[c] = temp;
			valid = 1;
			cnt = 0;
		 }
		 else {     /* duplicate number found, so reset flags and generate
				   new random number */
			getRandNum(1, 9, &temp);
			dup = 0;
			cnt++;
		 }
		 if(cnt > N*N) {
				/* if after N*N attempts, no possible value is found
				   then reset the entire array and start over
				   (brute force algorithm)
				   average runtime: 500000 iterations */
			dup = cnt = 0;
			valid = 1;
			clearArray(grid, N*N);
			temp = rand( ) % N + 1;
			c = -1; /* will be reset to 0 in for loop */
		}
	}
   }
   return iterations; /* 'iterations' must be updated within this function BEFORE it is returned */
}
