#!/usr/bin/env zsh
#SBATCH -p instruction
#SBATCH -J task3
#SBATCH -c 8
#SBATCH -t 0-00:05:00
#SBATCH -o task3-%j.out -e task3-%j.err

# Problem 3: OpenMP version, 1 and 8 threads.  Submit with:  sbatch task3.sh

g++ task3.cpp -Wall -O3 -std=c++17 -o task3 -fopenmp
./task3 100 10 1
./task3 100 10 8
