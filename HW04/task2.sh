#!/usr/bin/env zsh
#SBATCH -p instruction
#SBATCH -J task2
#SBATCH -c 8
#SBATCH -t 0-00:05:00
#SBATCH -o task2-%j.out -e task2-%j.err

# Problem 2: serial C++ port.  Submit with:  sbatch task2.sh

g++ task2.cpp -Wall -O3 -std=c++17 -o task2
./task2 100 10
