#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

#define NUM_THREADS 8 // Number of threads to use
#define INITIAL_SIZE 1000000 // Initial allocation size for lines
#define MAX_LINES_IN_BATCH 2000
#define MEMORY_LIMIT 60 // Memory limit in MB
#define BYTES_LIMIT (60 * 1024 * 1024) // max number of bytes we want to read in at a time

// Global arrays and variables
char **char_array; // Array of strings (lines from the file)
int *max_values; // Array to store max ASCII values per line
long total_lines = 0; // Total number of lines read from the file
long lines_in_batch;

// THREAD WORKER FUNCTION
// Each thread executes this function. It processes a chunk of the lines
// and computes the maximum ASCII value per line.
void *thread_worker(void *arg) 
{
    long thread_id = (long)arg;

    // Divide the work among threads evenly
    //long lines_per_thread = total_lines / NUM_THREADS;
    long lines_per_thread = lines_in_batch / NUM_THREADS;
    long start = thread_id * lines_per_thread;
    long end = (thread_id == NUM_THREADS - 1) ? lines_in_batch : start + lines_per_thread;

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

// PRINT RESULTS (and free memory)
// Output format: line_number: max_ascii_value
void print_results(long offset) 
{
    for (long i = 0; i < lines_in_batch; i++) 
    {
        printf("%ld: %d\n", i+offset, max_values[i]);
        free(char_array[i]);
    }
}

// Process the data in batches
// Gives the program the ability read in file even when the program has only 1GB of memory available
void process_batch(long offset)
{
    pthread_t threads[NUM_THREADS];

    for(long i = 0; i < NUM_THREADS; i++)
    {
         if (pthread_create(&threads[i], NULL, thread_worker, (void *)i) != 0) 
        {
            perror("Error creating thread");
            exit(1);
        }
    }
    for(long i = 0; i < NUM_THREADS; i++)
    {
        // Wait for all threads to join
        pthread_join(threads[i], NULL);
    }

    print_results(offset);
}

// INITIALIZE ARRAYS
// Reads the file line-by-line into dynamically allocated char_array
long init_arrays(FILE *fp) 
{
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t char_array_size = MAX_LINES_IN_BATCH;

    long lines_read = 0;
    //keep track of memory usage to reduce freezing
    long memory_usage = 0;

    // Allocate initial space for char_array
    char_array = malloc(char_array_size * sizeof(char *));
    max_values = malloc(char_array_size * sizeof(int));

    if (char_array == NULL || max_values == NULL) 
    {
        perror("Error allocating memory for char_array or max_values");
        exit(1);
    }

    //account for malloc
    memory_usage += char_array_size * (sizeof(char*) + sizeof(int));
    
   
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
            memory_usage += char_array_size * (sizeof(char*) + sizeof(int));

            // Initialize memory for new character and integers being read in
            memset(char_array + lines_read, 0, (char_array_size - lines_read) * sizeof(char*));
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
        memory_usage += read + 1;
        lines_read++;

        // Need to break out of the loop early so we can stop reading and grab more memory
        if(memory_usage >= BYTES_LIMIT)
        {
            break;
        }
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
    if(argc > 2)
    {
        perror("Too many arguments, please use 2");
        exit(1);
    }

    // Start the clock
    clock_gettime(CLOCK_MONOTONIC, &start);

    //Read the wiki_dump.txt file
    FILE *fp;
    char *file_path = "/homes/dan/625/wiki_dump.txt";

    fp = fopen(file_path, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    
   long offset = 0;

   //while not end of file, process the wiki dump
   while(1)
   {
        lines_in_batch = init_arrays(fp);

        //if 0 lines are in the batch, we are at the end of the file 
        if(lines_in_batch == 0)
            break;
        process_batch(offset);
        offset += lines_in_batch;
   }

    // Close and Free Memory
    fclose(fp);
    free(char_array);
    free(max_values);

    // Get the end time and CPU Usage
    clock_gettime(CLOCK_MONOTONIC, &end);
    getrusage(RUSAGE_SELF, &usage);

    // Open text file to write the resulting clock time and cpu usage
    char *file_path_2 = args[1];
    FILE *fp2 = fopen(file_path_2, "w");
    if(fp2 == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    // Calculate the difference between the start and end time of printing all the max ASCII Characters
    double diff_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/(1e9);

    // Calcualte the total cpu time
    double cpu_time = usage.ru_utime.tv_sec + usage.ru_stime.tv_sec + usage.ru_utime.tv_usec/(1e6) + usage.ru_stime.tv_usec/(1e6);
    double cpu_percent = 100 * cpu_time / diff_time;

    // Write the stats to the text file
    fprintf(fp2, "Time: %.6fs CPU: %.2f%%", diff_time, cpu_percent);

    fclose(fp2);
    return 0;
}

