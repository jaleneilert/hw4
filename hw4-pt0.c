#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 4

#define INITIAL_SIZE 1000000

// Mutext for char_counts
pthread_mutex_t mutexSum;

// Char array to hold the lines from the file
char **char_array;

/// @brief Initialize the char_array and char_counts
/// @details char_array is filled with lines from the input file
void init_arrays()
{
	// Iniitalize variables
	FILE *fp;
	char *file_path = "/homes/dan/625/wiki_dump.txt";
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	ssize_t line_num = 0;

	// Init pthread mutex
	pthread_mutex_init(&mutexSum, NULL);

	// Allocate memory for char_array here
	// Also keep track of array size incase it needs to be resized when reading lines in
	size_t char_array_size = INITIAL_SIZE;
	char_array = malloc(char_array_size * sizeof(char *));

	// Check if char_array was allocated successfully
	if (char_array == NULL)
	{
		perror("Error allocating memory for char_array");
		exit(1);
	}

	// Open file for reading
	fp = fopen(file_path, "r");

	// Check if file opened successfully
	if (fp == NULL)
	{
		perror("Error opening file");
		exit(1);
	}

	// Read each line in to char_array
	while ((read = getline(&line, &len, fp)) != -1)
	{

		// Checks if we need to resize the char_array, if it does it reallocates with twice the size
		if (line_num >= char_array_size)
		{
			char_array_size *= 2;
			char **temp = realloc(char_array, char_array_size * sizeof(char *));

			if (temp == NULL)
			{
				perror("Error reallocating memory for char_array");
				exit(1);
			}

			char_array = temp;
		}

		// Allocate memory for each line in char_array
		char_array[line_num] = malloc(read + 1);

		// Check if it was allocated
		if (char_array[line_num] == NULL)
		{
			perror("Error allocating memory");
			exit(1);
		}

		// Copy the line into char_array, +1 for null terminator
		snprintf(char_array[line_num], read + 1, "%s", line);

		line_num++;
	}

	// Resize char_array to the number of lines read for mem efficiency
	char **temp = realloc(char_array, line_num * sizeof(char *));

	if (temp == NULL)
	{
		perror("Error reallocating memory for char_array");
		exit(1);
	}

	char_array = temp;

	// Clean up:
	free(line);
	fclose(fp);
	return;
}

void count_array()
{
	char theChar;
	int i, j, charLoc;

	for (i = 0; i < ARRAY_SIZE; i++)
	{
		for (j = 0; j < STRING_SIZE; j++)
		{
			theChar = char_array[i][j];
			charLoc = ((int)theChar) - 97;
			char_counts[charLoc]++;
		}
	}
}

void print_results()
{
	int i, total = 0;

	for (i = 0; i < 26; i++)
	{
		total += char_counts[i];
		printf(" %c %d\n", (char)(i + 97), char_counts[i]);
	}
	printf("\nTotal characters:  %d\n", total);
}

main()
{
	init_arrays();
	count_array();
	print_results();
}
