#!/usr/bin/env zsh
#SBATCH -p instruction
#SBATCH -J task4
#SBATCH -c 8
#SBATCH -t 0-00:30:00
#SBATCH -o task4-%j.out -e task4-%j.err

# Problem 4: time task3 for every policy and 1..8 threads.
# Submit with:  sbatch task4.sh   (needs task3.cpp and plot_task4.py in this folder)

g++ task3.cpp -Wall -O3 -std=c++17 -o task3 -fopenmp

N=500
TEND=100

echo "# host=$(hostname) cores=$(nproc) N=$N tEnd=$TEND" > task4.csv
echo "schedule,threads,ms" >> task4.csv

for s in static dynamic guided; do
    for p in 1 2 3 4 5 6 7 8; do
        ms=$(OMP_SCHEDULE=$s ./task3 $N $TEND $p | sed 's/[^0-9.]//g')
        echo "$s,$p,$ms" >> task4.csv
        echo "$s threads=$p -> $ms ms"
    done
done

# makes task4.pdf (+ the numbers the report pulls in); harmless if matplotlib is missing
python3 plot_task4.py || echo "plotting failed - run: python3 plot_task4.py"
