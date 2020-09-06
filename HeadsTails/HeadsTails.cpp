#include <iostream>
#include <stdlib.h>
#include <time.h>

void print_progres(long long trial_i, long long total_trials)
{
    const int progress_increment = total_trials / 100;
    if ((trial_i % progress_increment) == 0)
    {
        int percent = (int)round(100.0 * ((double)trial_i / (double)total_trials));

        printf("\r--- progress %d%%", percent);
    }
}

int main()
{
    const int num_consecutive_heads = 27;
    long long num_trials            = 1LL << 28;

    printf("Trying %lld times\n\n", num_trials);

    const int hist_N = num_consecutive_heads;
    int hist[hist_N] = { 0, };
    int hist_i = 0;
    int num_consecutive_heads_found = 0;

    // select either a constant seed or one based on current time.
    unsigned long seed = 0;
    //unsigned long seed = time(NULL);

    printf("Using seed %lu\n\n", seed);
    srand(seed);

    for (long long trial_i = 0; trial_i < num_trials; trial_i++)
    {
        double r = double(rand()) / double(RAND_MAX);

        // classify random number into heads/tails. write to the oldest value in history buffer.
        // <  0.5 are heads
        // >= 0.5 are tails
        hist[hist_i] = (r < 0.5) ? 0 : 1;

        // Advance index into history buffer
        hist_i = (hist_i + 1) % hist_N;

        print_progres(trial_i, num_trials);

        if (trial_i >= hist_N)
        {
            // check if the entire history buffer has heads.
            // initially, assume all heads.
            bool all_heads = true;
            for (int k = 0; k < hist_N; k++)
            {
                if (hist[k] == 1)
                {
                    all_heads = false;
                    break;
                }
            }

            if (all_heads)
            {
                printf("\rFound %d consecutive heads at %lld\n", hist_N, trial_i);
                num_consecutive_heads_found++;
            }
        }
    }

    printf("\n\n");
    printf("Num %d consecutive heads found: %d\n", num_consecutive_heads, num_consecutive_heads_found);
}
