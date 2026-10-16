#!/usr/bin/env bash
#SBATCH -p instruction
#SBATCH -J HW03
#SBATCH -o HW03-%j.out
#SBATCH -e HW03-%j.err
#SBATCH -c 20
#SBATCH -t 0-00:30:00

# Builds all three tasks and records "x time_ms" pairs for plotting.
cd "$SLURM_SUBMIT_DIR"

g++ task1.cpp matmul.cpp -Wall -O3 -std=c++17 -o task1 -fopenmp
g++ task2.cpp convolution.cpp -Wall -O3 -std=c++17 -o task2 -fopenmp
g++ task3.cpp msort.cpp -Wall -O3 -std=c++17 -o task3 -fopenmp

# The third line of each program's output is the time in ms.
for t in $(seq 1 20); do echo "$t $(./task1 1024 "$t" | tail -1)"; done > task1.txt
for t in $(seq 1 20); do echo "$t $(./task2 1024 "$t" | tail -1)"; done > task2.txt

# Task 3a: sweep the threshold ts = 2^1 .. 2^10 with 8 threads
for p in $(seq 1 10); do
    ts=$((2 ** p))
    echo "$ts $(./task3 1000000 8 "$ts" | tail -1)"
done > task3_ts.txt

# Task 3b: sweep threads using the fastest threshold from 3a
best=$(sort -g -k2 task3_ts.txt | head -1 | cut -d' ' -f1)
echo "best ts = $best"
for t in $(seq 1 20); do echo "$t $(./task3 1000000 "$t" "$best" | tail -1)"; done > task3_t.txt
