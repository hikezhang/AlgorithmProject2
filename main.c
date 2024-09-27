#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
typedef struct {
    int x;
    int y;
} Point;
int compare_by_x(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    return p1->x - p2->x;
}

Point* merge(Point* left, int left_size, Point* right, int right_size, int* staircase_size) {
    Point* staircase = malloc((left_size + right_size) * sizeof(Point));
    int i = 0, j = 0, k = 0;

    while (i < left_size) {
        staircase[k++] = left[i++];
    }
    while (j < right_size) {
        if (right[j].y > staircase[k - 1].y) {
            staircase[k++] = right[j];
        }
        j++;
    }

    *staircase_size = k;
    return staircase;
}

Point* compute_staircase(Point* points, int n, int* staircase_size) {
    if (n == 1) {
        Point* staircase = malloc(sizeof(Point));
        staircase[0] = points[0];
        *staircase_size = 1;
        return staircase;
    }

    // if n > 1 , split the set into 2 subsets
    int mid = n / 2;
    int left_size, right_size;

    // recursively compute the pareto optimal points in both sides
    Point* left_staircase = compute_staircase(points, mid, &left_size);
    Point* right_staircase = compute_staircase(points + mid, n - mid, &right_size);

    // merge Pareto optimal points in both sides
    Point* staircase = merge(left_staircase, left_size, right_staircase, right_size, staircase_size);

    free(left_staircase);
    free(right_staircase);

    return staircase;
}

int main(void) {
    int sizes[] = {100, 1000, 10000, 100000, 1000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_runs = 5;  // Number of runs for each size

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        double total_time = 0;
        int total_staircase_size = 0;

        for (int run = 0; run < num_runs; run++) {
            Point* points = malloc(n * sizeof(Point));

            // Generate random points
            srand((unsigned int)time(NULL) + run);
            for (int i = 0; i < n; i++) {
                points[i].x = rand() % INT_MAX;
                points[i].y = rand() % INT_MAX;
            }

            clock_t start = clock();
            int staircase_size;
            Point* staircase = NULL;
                    staircase = compute_staircase(points, n, &staircase_size);
            

            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000000.0;

            total_time += cpu_time_used;
            total_staircase_size += staircase_size;

            free(staircase);
            free(points);
        }

        printf("Input size: %d\n", n);
        printf("Average time taken: %f microseconds\n", total_time / num_runs);
        printf("Average staircase size: %f\n\n", (double)total_staircase_size / num_runs);
    }

    return 0;
}
