//Made by: Aiden Ross
//For Purdue University Fort Wayne CS47200 w/ Dr. Liu

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_MEALS 10
#define PHILOSOPHER_NUM 5
#define MAX_THINK_EAT_SEC 4

enum { THINKING, HUNGRY, EATING } state[PHILOSOPHER_NUM];
pthread_mutex_t mutex;
pthread_cond_t condition[PHILOSOPHER_NUM];
int meals_eaten[PHILOSOPHER_NUM] = {0};
int running = 1; // Flag to control simulation duration

// Initialize resources
void initialize() {
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < PHILOSOPHER_NUM; i++) {
        state[i] = THINKING;
        pthread_cond_init(&condition[i], NULL);
    }
}

// Test if a philosopher can eat
void test(int i) {
    if (state[i] == HUNGRY &&
        state[(i + 4) % PHILOSOPHER_NUM] != EATING &&
        state[(i + 1) % PHILOSOPHER_NUM] != EATING) {
        state[i] = EATING;
        pthread_cond_signal(&condition[i]); // Wake up philosopher
    }
}

// Pickup forks
void pickup(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    test(i); // Try to eat
    while (state[i] != EATING) {
        pthread_cond_wait(&condition[i], &mutex); // Wait until able to eat
    }
    pthread_mutex_unlock(&mutex);
}

// Put down forks
void putdown(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = THINKING; // Finished eating, now thinking
    test((i + 4) % PHILOSOPHER_NUM); // Check left neighbor
    test((i + 1) % PHILOSOPHER_NUM); // Check right neighbor
    pthread_mutex_unlock(&mutex);
}

// Philosopher thread routine
void* philo_run(void* param) {
    int id = *(int*)param;
    free(param); // Free dynamically allocated memory

    int left_fork = (id + 4) % PHILOSOPHER_NUM; // Left fork index
    int right_fork = id; // Right fork index

    while (running && (meals_eaten[id] < MAX_MEALS)) {
        // Think
        sleep((rand() % MAX_THINK_EAT_SEC + 1)); // Thinking time

        // Pickup forks
        pickup(id);

        // Eat and log
        meals_eaten[id]++;

        // Simulate eating
        sleep((rand() % MAX_THINK_EAT_SEC + 1));

        // Put down forks
        putdown(id);
    }

    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <run_time>\n", argv[0]);
        return -1;
    }

    int run_time = atoi(argv[1]);
    pthread_t philosophers[PHILOSOPHER_NUM];
    srand(time(NULL));

    initialize();

    // Create philosopher threads
    for (int i = 0; i < PHILOSOPHER_NUM; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        if (pthread_create(&philosophers[i], NULL, philo_run, id) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return -1;
        }
    }

    // Let the simulation run for the specified time (run_time)
    sleep(run_time);
    running = 0; // Signal threads to stop

    // Wait for all threads to finish
    for (int i = 0; i < PHILOSOPHER_NUM; i++) {
        if (pthread_join(philosophers[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return -1;
        }
    }

    // Calculate statistics
    int min_meals = meals_eaten[0], max_meals = meals_eaten[0], total_meals = 0;
    for (int i = 0; i < PHILOSOPHER_NUM; i++) {
        if (meals_eaten[i] < min_meals) min_meals = meals_eaten[i];
        if (meals_eaten[i] > max_meals) max_meals = meals_eaten[i];
        total_meals += meals_eaten[i];
    }
    double avg_meals = total_meals / (double)PHILOSOPHER_NUM;

    printf("\nRun Time: %d seconds\n", run_time);
    printf("Minimum meals: %d\n", min_meals);
    printf("Maximum meals: %d\n", max_meals);
    printf("Average meals: %.2f\n\n", avg_meals);

    // Clean up resources
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < PHILOSOPHER_NUM; i++) {
        pthread_cond_destroy(&condition[i]);
    }

    return 0;
}
