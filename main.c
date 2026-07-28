#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

extern void calc_accel(double* input, int* output, int num_cars);

void gen_data(double* input, int num_cars) {
    for (int i = 0; i < num_cars; i++) {
        input[i * 3]     = (double)(rand() % 2000) / 10.0;
        input[i * 3 + 1] = (double)(rand() % 3000) / 10.0;
        input[i * 3 + 2] = 1.0 + (double)(rand() % 291) / 10.0;
    }
}

void c_calc_accel(double* input, int* output, int num_cars) {
    for (int i = 0; i < num_cars; i++) {
        double vi = input[i * 3];
        double vf = input[i * 3 + 1];
        double t  = input[i * 3 + 2];

        double delta_v = (vf - vi) * 1000.0 / 3600.0;
        double accel = delta_v / t;
        output[i] = (int)round(accel);
    }
}

int verify_out(int* asm_out, int* c_out, int num_cars) {
    int is_correct = 1;
    for (int i = 0; i < num_cars; i++) {
        if (asm_out[i] != c_out[i]) {
            printf("  MISMATCH at car %d: asm = %d, C = %d\n",
                   i + 1, asm_out[i], c_out[i]);
            is_correct = 0;
        }
    }
    return is_correct;
}

void run_bench(void) {
    int sizes[]    = {10, 100, 1000, 10000};
    int num_sizes  = 4;
    int num_runs   = 30;

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    printf("\n");
    printf("===========================================================\n");
    printf("                    BENCHMARK RESULTS\n");
    printf("===========================================================\n");
    printf("  %-10s | %-20s | %-12s\n", "Y (cars)", "Avg Time (ms)", "Correct?");
    printf("  ---------- | -------------------- | ------------\n");

    srand(42);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];

        double* input    = (double*)malloc(n * 3 * sizeof(double));
        int*    output   = (int*)malloc(n * sizeof(int));
        int*    expected = (int*)malloc(n * sizeof(int));

        if (!input || !output || !expected) {
            printf("  Memory allocation failed for Y = %d. Skipping.\n", n);
            free(input); free(output); free(expected);
            continue;
        }

        gen_data(input, n);

        double total_ms = 0.0;
        for (int r = 0; r < num_runs; r++) {
            LARGE_INTEGER start, end;
            QueryPerformanceCounter(&start);
            calc_accel(input, output, n);
            QueryPerformanceCounter(&end);
            total_ms += (double)(end.QuadPart - start.QuadPart)
                           / (double)freq.QuadPart * 1000.0;
        }

        double avg_ms = total_ms / num_runs;

        c_calc_accel(input, expected, n);
        int correct = verify_out(output, expected, n);

        printf("  %-10d | %-20.6f | %-12s\n",
               n, avg_ms, correct ? "PASSED" : "FAILED");

        free(input);
        free(output);
        free(expected);
    }

    printf("===========================================================\n");
    printf("  Each size was run %d times. Times shown are averages.\n", num_runs);
    printf("===========================================================\n");
}

int main(void) {
    int y;

    printf("===========================================================\n");
    printf("         Car Acceleration Calculator (C + x86-64 ASM)\n");
    printf("===========================================================\n\n");

    printf("Enter number of cars (Y): ");
    if (scanf("%d", &y) != 1 || y <= 0) {
        printf("Invalid input. Y must be a positive integer.\n");
        return 1;
    }

    double* input    = (double*)malloc(y * 3 * sizeof(double));
    int*    output   = (int*)malloc(y * sizeof(int));
    int*    expected = (int*)malloc(y * sizeof(int));

    if (!input || !output || !expected) {
        printf("Memory allocation failed.\n");
        free(input); free(output); free(expected);
        return 1;
    }

    printf("Enter all %d values (Vi, Vf, T for each car) separated by spaces or commas:\n", y * 3);
    for (int i = 0; i < y * 3; i++) {
        if (scanf("%lf", &input[i]) != 1) {
            printf("Invalid input at value %d.\n", i + 1);
            free(input); free(output); free(expected);
            return 1;
        }
        
        int c;
        do {
            c = getchar();
        } while (c == ' ' || c == '\t');
        
        if (c != ',' && c != EOF) {
            ungetc(c, stdin);
        }
    }

    calc_accel(input, output, y);

    printf("\n--- Acceleration Results (m/s^2, integer) ---\n");
    for (int i = 0; i < y; i++) {
        printf("%d\n", output[i]);
    }

    printf("\n--- Correctness Check ---\n");
    c_calc_accel(input, expected, y);
    int correct = verify_out(output, expected, y);
    if (correct) {
        printf("  All %d results match the C reference. PASSED.\n", y);
    } else {
        printf("  Some results did NOT match. FAILED.\n");
    }

    free(input);
    free(output);
    free(expected);

    run_bench();

    printf("\nDone.\n");
    system("pause");
    return 0;
}
