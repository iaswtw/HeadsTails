#include <iostream>
#include <stdlib.h>

void print_progres(int trial_i, long long total_trials)
{
    // todo - print progress
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
    long long num_trials                = 1 << 27;

    printf("Trying %lld times\n\n", num_trials);

    const int hist_N = num_consecutive_heads;
    int hist[hist_N] = { 0, };
    int hist_i = 0;
    int hist_start = 0;
    long long trial_i;
    long long j;
    int num_consecutive_heads_found = 0;

    srand(0);

    for (trial_i = 0; trial_i < num_trials; trial_i++)
    {
        double r = double(rand()) / double(RAND_MAX);

        // <0.5 are heads
        // >= 0.5 are tails
        if (r < 0.5)
            hist[hist_i] = 0;
        else
            hist[hist_i] = 1;

        hist_i = (hist_i + 1) % hist_N;

        print_progres(trial_i, num_trials);

        if (trial_i >= hist_N)
        {
            // assume all heads
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
                //print_progres(trial_i, num_trials);
                printf("\rFound %d consecutive heads at %lld\n", hist_N, trial_i);
                num_consecutive_heads_found++;
            }
        }
    }

    printf("\n\n");
    printf("Num %d consecutive heads found: %d\n", num_consecutive_heads, num_consecutive_heads_found);
}
