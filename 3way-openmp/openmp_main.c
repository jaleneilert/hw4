#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

#define NUM_THREADS 8      // Number of threads to use
#define LINES_TO_READ 1000000 // How many lines to read (change for each number of lines)
#define MAX_LINES_IN_BATCH 1000

// Declaration of print_results
void print_results(long offset);

// Global arrays and variables
char **char_array;    // Array of strings (lines from the file)
int *max_values;      // Array to store max ASCII values per line
long total_lines = 0; // Total number of lines read from the file
long lines_in_batch;

// PROCESS BATCH OPEN MP
// This function processes a batch of lines and finds the max ASCII value in each line
// It uses Open MP to gather results from all processes
void process_batch_openmp(long offset)
{
    #pragma omp parallel for shared(lines_in_batch, char_array, max_values)
    for (long i = 0; i < lines_in_batch; i++)
    {
        int max = 0;
        for (char *p = char_array[i]; *p != '\0'; p++)
        {
            if ((int)(*p) > max)
            {
                max = (int)(*p);
            }
        }

        max_values[i] = max;
    }

    print_results(offset);
    
}

// PRINT RESULTS (and free memory)
// Output format: line_number: max_ascii_value
void print_results(long offset)
{
    for (long i = 0; i < lines_in_batch; i++)
    {
        printf("%ld: %d\n", i + offset, max_values[i]);
        free(char_array[i]);
    }
}

// INITIALIZE ARRAYS
// Reads the file line-by-line into dynamically allocated char_array
long init_arrays(FILE *fp, long lines_remaining)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t char_array_size = MAX_LINES_IN_BATCH;

    long lines_read = 0;
    // keep track of memory usage to reduce freezing

    // Allocate initial space for char_array
    char_array = malloc(char_array_size * sizeof(char *));
    max_values = malloc(char_array_size * sizeof(int));

    if (char_array == NULL || max_values == NULL)
    {
        perror("Error allocating memory for char_array or max_values");
        exit(1);
    }

    // Read each line from the file
    while ((read = getline(&line, &len, fp)) != -1)
    {
        // Resize char_array if necessary
        if (lines_read >= (long)char_array_size)
        {
            char_array_size *= 2;
            char **temp = realloc(char_array, char_array_size * sizeof(char *));
            int *temp_max = realloc(max_values, char_array_size * sizeof(int));
            if (temp == NULL || temp_max == NULL)
            {
                perror("Error reallocating memory");
                exit(1);
            }
            char_array = temp;
            max_values = temp_max;

            // Initialize memory for new character and integers being read in
            memset(char_array + lines_read, 0, (char_array_size - lines_read) * sizeof(char *));
            memset(max_values + lines_read, 0, (char_array_size - lines_read) * sizeof(int));
        }
        // Allocate space for the line and copy it in
        char_array[lines_read] = malloc(read + 1);

        if (char_array[lines_read] == NULL)
        {
            perror("Error allocating line memory");
            exit(1);
        }

        // load pages into RAM
        memset(char_array[lines_read], 0, read + 1);
        snprintf(char_array[lines_read], read + 1, "%s", line);
        lines_read++;

        if (lines_read >= lines_remaining)
            break;
    }

    // Clean up
    free(line);
    return lines_read;
}

// MAIN FUNCTION
// Controls program flow: setup, thread management, output, cleanup
int main(int argc, char *args[])
{

    // Structs to hold the time and usage
    struct timespec start, end;
    struct rusage usage;


    // Check the number of arguments
    if (argc != 2)
    {
        perror("Too many arguments, please use 2");
        exit(1);
    }

    // Set the number of threads for Open MP
    omp_set_num_threads(NUM_THREADS);

    // Start the clock
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Read the wiki_dump.txt file
    FILE *fp;
    char *file_path = "/homes/dan/625/wiki_dump.txt";

    fp = fopen(file_path, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    // number of lines read so far
    long total_lines = 0;

    // while not end of file, process the wiki dump
    while (total_lines < LINES_TO_READ)
    {
        long lines_remaining = LINES_TO_READ - total_lines;
        long batch_size = lines_remaining < MAX_LINES_IN_BATCH ? lines_remaining : MAX_LINES_IN_BATCH;

        // most likely need to get rid of init arrays for a function that distributes lines to all the workers
        lines_in_batch = init_arrays(fp, batch_size);

        // if 0 lines are in the batch, we are at the end of the file
        if (lines_in_batch == 0)
        {
            break;
        }

        process_batch_openmp(total_lines);

        total_lines += lines_in_batch;

        free(char_array);
        free(max_values);
    }

    // Close and Free Memory
    fclose(fp);


    // Get the end time and CPU Usage
    clock_gettime(CLOCK_MONOTONIC, &end);
    getrusage(RUSAGE_SELF, &usage);

    // Open text file to write the resulting clock time and cpu usage
    char *file_path_2 = args[1];
    FILE *fp2 = fopen(file_path_2, "w");
    if (fp2 == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    // Calculate the difference between the start and end time of printing all the max ASCII Characters
    double diff_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (1e9);

    // Calcualte the total cpu time
    double cpu_time = usage.ru_utime.tv_sec + usage.ru_stime.tv_sec + usage.ru_utime.tv_usec / (1e6) + usage.ru_stime.tv_usec / (1e6);
    double cpu_percent = 100 * cpu_time / diff_time;

    // Write the stats to the text file
    fprintf(fp2, "Time: %.6fs CPU: %.2f%%", diff_time, cpu_percent);

    fclose(fp2);

    return 0;
}
