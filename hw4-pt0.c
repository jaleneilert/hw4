#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 4 // Number of threads to use
#define INITIAL_SIZE 1000000 // Initial allocation size for lines

// Global arrays and variables
char **char_array; // Array of strings (lines from the file)
int *max_values; // Array to store max ASCII values per line
long total_lines = 0; // Total number of lines read from the file

// THREAD WORKER FUNCTION
// Each thread executes this function. It processes a chunk of the lines
// and computes the maximum ASCII value per line.
void *thread_worker(void *arg) 
{
    long thread_id = (long)arg;

    // Divide the work among threads evenly
    long lines_per_thread = total_lines / NUM_THREADS;
    long start = thread_id * lines_per_thread;
    long end = (thread_id == NUM_THREADS - 1) ? total_lines : start + lines_per_thread;

    // Process each line assigned to this thread
    for (long i = start; i < end; i++) 
    {
        int max = 0;

        // Go through each character in the line to find the max ASCII value
        for (char *p = char_array[i]; *p != '\0'; p++) 
        {
            if ((int)(*p) > max) 
            {
                max = (int)(*p);
            }
        }

        // Store the max value for this line
        max_values[i] = max;
    }

    // Exit thread
    pthread_exit(NULL);
}

// INITIALIZE ARRAYS
// Reads the file line-by-line into dynamically allocated char_array
void init_arrays() 
{
    FILE *fp;
    char *file_path = "/homes/dan/625/wiki_dump.txt";
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t char_array_size = INITIAL_SIZE;

    // Allocate initial space for char_array
    char_array = malloc(char_array_size * sizeof(char *));
    if (char_array == NULL) 
    {
        perror("Error allocating memory for char_array");
        exit(1);
    }

    // Open the input file
    fp = fopen(file_path, "r");
    if (fp == NULL) 
    {
        perror("Error opening file");
        exit(1);
    }

    // Read each line from the file
    while ((read = getline(&line, &len, fp)) != -1) 
    {
        // Resize char_array if necessary
        if (total_lines >= char_array_size) 
        {
            char_array_size *= 2;
            char **temp = realloc(char_array, char_array_size * sizeof(char *));
            if (temp == NULL) 
            {
                perror("Error reallocating memory");
                exit(1);
            }
            char_array = temp;
        }

        // Allocate space for the line and copy it in
        char_array[total_lines] = malloc(read + 1);
        if (char_array[total_lines] == NULL) 
        {
            perror("Error allocating line memory");
            exit(1);
        }

        snprintf(char_array[total_lines], read + 1, "%s", line);
        total_lines++;
    }

    // Clean up
    free(line);
    fclose(fp);
}

// PRINT RESULTS
// Output format: line_number: max_ascii_value
void print_results() 
{
    for (long i = 0; i < total_lines; i++) 
    {
        printf("%ld: %d\n", i, max_values[i]);
    }
}

// MAIN FUNCTION
// Controls program flow: setup, thread management, output, cleanup
int main() 
{
    pthread_t threads[NUM_THREADS];

    // Step 1: Read in the file
    init_arrays();

    // Step 2: Allocate memory for results array
    max_values = malloc(total_lines * sizeof(int));
    if (max_values == NULL) 
    {
        perror("Error allocating memory for max_values");
        exit(1);
    }

    // Step 3: Create threads
    for (long i = 0; i < NUM_THREADS; i++) 
    {
        if (pthread_create(&threads[i], NULL, thread_worker, (void *)i) != 0) 
        {
            perror("Error creating thread");
            exit(1);
        }
    }

    // Step 4: Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    // Step 5: Output the results
    print_results();

    // Step 6: Free allocated memory
    for (long i = 0; i < total_lines; i++) 
    {
        free(char_array[i]);
    }
    free(char_array);
    free(max_values);

    return 0;
}
