# profiling_helper

Makes it easier to profile the SPA (since it doesn't have all the other crap that AutoTester comes with).

All it does is:

- take in a source file and query file
- process the source file with SP
- parse queries from the query file
- run all of the queries

That's it, it doesn't check if the queries ran successfully or whatever.

Otherwise, it should be very similar to what's being done in the AutoTester TestWarpper.

## How to use this?

You can probably use something like [uftrace](https://github.com/namhyung/uftrace) to profile the executable.

Build for the "Profile" CMake target, then run `uftrace record profiling_helper source.txt queries.txt`.

The profiling results should be saved in `uftrace.data` in the current directory, so you can use `uftrace tui` (or similar) to examine it.

## Improving accuracy

For accurate results (especially between separate runs, but also including within a single run), you may want to reduce variations in your results e.g. by:
- disabling powersaving features
- disabling dynamic CPU clock scaling features (e.g. Intel Turbo Boost and equivalents)
- avoid having too much crap open in the background
- running each test multiple times to average out any random error
