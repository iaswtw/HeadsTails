#include <iostream>
#include <stdlib.h>
#include <time.h>


void search_heads(uint32_t num_consecutive_heads, uint64_t num_trials);

constexpr int NUM_CONSECUTIVE_HEADS        = 27;
constexpr uint64_t NUM_TRIALS              = (1LL << 33);


//-------------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------------
class Progress
{
public:
    uint64_t * trial_number;
    unsigned int current;
    unsigned int max;

    Progress(uint64_t num_trials)
        : current(0),
          max(100)
    {
        double processor_dependent_inc = double(1ULL << 26);
        max = int(double(num_trials) / processor_dependent_inc);

        if (max < 100)
            max = 100;
        if (max > 100)
            max = 1000;
        if (max > 1000)
            max = 10000;

        //printf("max = %u\n\n", max);

        // Allocate and construct lookup table to print progress
        trial_number = new uint64_t[max];
        for (int i = 0; i < max; i++)
        {
            trial_number[i] = (double(i) / max) * double(num_trials);
        }
        current = 0;
    }

    ~Progress()
    {
        delete [] trial_number;
    }

    void print_progress(uint64_t current_trial)
    {
        if (current < max)
        {
            if (trial_number[current] == current_trial)
            {
                double percent = double(current) / double(max) * 100;
                if (max == 100)
                    printf("\r--- progress %u%%", int(percent));
                else if (max == 1000)
                    printf("\r--- progress %.1lf%%", percent);
                else if (max == 10000)
                    printf("\r--- progress %.2lf%%", percent);
                current++;
            }
        }
    }

    /* Clear the progress line.
     */
    void clear_progress()
    {
        printf("\r                         \n");
    }
};

//-------------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
    std::cout.imbue(std::locale(""));

    time_t start_time = time(NULL);

    search_heads(NUM_CONSECUTIVE_HEADS, NUM_TRIALS);

    time_t end_time = time(NULL);

    printf("\nSearch took %llu seconds\n", end_time - start_time);

}

void search_heads(uint32_t num_consecutive_heads, uint64_t num_trials)
{
    Progress progress(num_trials);

    //printf("Trying %llu times\n\n", num_trials);
    std::cout << "Running " << num_trials << " trials" << "\n";
    std::cout << "Looking for " << num_consecutive_heads << " consecutive heads" << "\n\n";

    const unsigned int hist_N = num_consecutive_heads;
    int * hist = new int[hist_N];
    int hist_i = 0;
    int num_consecutive_heads_found = 0;

    // select either a constant seed or one based on current time.
    unsigned long seed = 0;
    //unsigned long seed = time(NULL);

    printf("Using seed %lu\n\n", seed);
    srand(seed);

    for (uint64_t trial_i = 0; trial_i < num_trials; trial_i++)
    {
        int r = rand();

        // classify random number into heads/tails. write to the oldest value in history buffer.
        // <  0.5 are heads
        // >= 0.5 are tails
        hist[hist_i] = (r < (RAND_MAX >> 1)) ? int(0) : int(1);

        // Advance index into history buffer
        hist_i = (hist_i + 1) % hist_N;

        //print_progres(trial_i, NUM_TRIALS);
        progress.print_progress(trial_i);

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
                std::cout << "\r    Found " << hist_N << " consecutive heads at " << trial_i << "\n";
                num_consecutive_heads_found++;
            }
        }
    }

    progress.clear_progress();

    printf("\n");
    printf("Num %u consecutive heads found: %u\n", num_consecutive_heads, num_consecutive_heads_found);

    delete[] hist;
}
