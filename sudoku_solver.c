#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define NUM_THREADS 27
typedef struct {
	int row;
	int column;
} parameters;

void* check_row(void *param);
void* check_square(void *param);
void* check_column(void *param);

//Test sudoku puzzle
int sudoku[9][9] = { { 6, 5, 3, 1, 2, 8, 7, 9, 4 },
		{ 1, 7, 4, 3, 5, 9, 6, 8, 2 }, { 9, 2, 8, 4, 6, 7, 5, 3, 1 }, { 2, 8,
				6, 5, 1, 4, 3, 7, 9 }, { 3, 9, 1, 7, 8, 2, 4, 5, 6 }, { 5, 4,
				7, 6, 9, 3, 2, 1, 8 }, { 8, 6, 5, 2, 3, 1, 9, 4, 7 }, { 4, 1,
				2, 9, 7, 5, 8, 6, 3 }, { 7, 3, 9, 8, 4, 6, 1, 2, 5 } };

/*
 * @return 1 row if contains 1-9 else return 0
 */
void* check_row(void *param) {
	parameters *data = (parameters *) param;
	int startRow = data->row;
	int startColumn = data->column;
	int i, j = 1; //j will compare from 1-9
	//search from 1-9
	for (i = 0; i < 9; i++) {
		//for every match, reset and look for the next number
		if (sudoku[startColumn + i][startRow] == j) {
			j++;
			i = -1;
		}
	}
	//did not find number between 1-9
	if (j != 10)
		return (void*) 0;
	return (void*) 1;
}

/*
 * @return 1 column if contains 1-9 else return 0
 */
void* check_column(void *param) {
	parameters *data = (parameters *) param;
	int startRow = data->row;
	int startColumn = data->column;
	int i, j = 1; //j will compare from 1-9
	//search from 1-9
	for (i = 0; i < 9; i++) {
		//for every match, reset and look for the next number
		if (sudoku[startColumn][startRow + i] == j) {
			j++;
			i = -1;
		}
	}
	//did not find number between 1-9
	if (j != 10)
		return (void*) 0;
	return (void*) 1;
}
/*
 * @return 1 square if contains 1-9 else return 0
 */
void* check_square(void *param) {
	parameters *data = (parameters *) param;
	int startRow = data->row;
	int startColumn = data->column;
	int h, i, j = 1; //j will compare from 1-9
	//search from 1-9
	for (i = 0; i < 3; i++) {
		for (h = 0; h < 3; h++) {
			//for every match, reset and look for the next number
			if (sudoku[startColumn + h][startRow + i] == j) {
				j++;
				i = -1;
				h = -1;
			}
		}
	}
	//did not find number between 1-9
	if (j != 10)
		return (void*) 0;
	return (void*) 1;
}

int main(int argc, char* argv[]) {
	pthread_t workers[NUM_THREADS];
	int h, i, j;
	parameters *square[9];
	parameters *column[9];
	parameters *row[9];

	//print out sudoku puzzle
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			printf("%d, ", sudoku[i][j]);
		}
		printf("\n");
	}
	//sets parameters for columns and rows
	for (i = 0; i < 9; i++) {
		column[i] = (parameters *) malloc(sizeof(parameters));
		column[i]->row = 0;
		column[i]->column = i;
		row[i] = (parameters *) malloc(sizeof(parameters));
		row[i]->row = i;
		row[i]->column = 0;
	}
	//sets parameters for square
	for (h = 0; h < 9; h++) {
		for (i = 0; i < 9; i += 3) {
			for (j = 0; j < 9; j += 3) {
				square[h] = (parameters *) malloc(sizeof(parameters));
				square[h]->row = i;
				square[h]->column = j;
			}
		}
	}
	//initialize the threads
	for (i = 0; i < NUM_THREADS; i++) {
		if (i < 9) { //threads for
			pthread_create(&workers[i], NULL, check_column,
					(void*) column[i % 9]);
		} else if (i < 18) {
			pthread_create(&workers[i], NULL, check_square,
					(void*) square[i % 9]);
		} else {
			pthread_create(&workers[i], NULL, check_row, (void*) row[i % 9]);
		}
	}

	void *results[NUM_THREADS];
	//join the threads
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(workers[i], &results[i]);
	//check whether all puzzle is correct
	int succeed = 1;
	for (i = 0; i < NUM_THREADS; i++) {
		if (((intptr_t) results[i]) != 1) {
			succeed = 0;
		}
	}
	//Success or failure
	if (succeed == 1) {
		printf("valid Sudoku \n");
	} else
		printf("invalid Sudoku \n");
	printf("CS149 Sudoku from Edmond Chew \n");
	pthread_exit(NULL);
	return 0;
}
