#!/usr/bin/env bash
#SBATCH -p instruction
#SBATCH -J hw05
#SBATCH -o hw05.out
#SBATCH -e hw05.err
#SBATCH -t 0-00:30:00
#SBATCH -c 1
#SBATCH --mem=16G
#SBATCH --gpus-per-task=1

cd "$SLURM_SUBMIT_DIR"
module load nvidia/cuda

FLAGS="-Xcompiler -O3 -Xcompiler -Wall -Xptxas -O3 -std=c++17"

echo "===== GPU ====="
nvidia-smi --query-gpu=name,memory.total --format=csv

echo "===== Compile ====="
nvcc task1.cu $FLAGS -o task1 || exit 1
nvcc task2.cu $FLAGS -o task2 || exit 1
nvcc task3.cu vscale.cu $FLAGS -o task3 || exit 1

echo "===== task1 ====="
./task1

echo "===== task2 ====="
./task2

echo "===== task3 sanity (n=1000) ====="
./task3 1000

echo "===== task3 sweep ====="
# Logging into csv to record
echo "n,tpb,ms" > task3_timings.csv
for p in $(seq 10 29); do
    n=$((2 ** p))
    for tpb in 512 16; do
        ms=$(./task3 "$n" "$tpb" | head -n 1)
        echo "$n,$tpb,$ms" >> task3_timings.csv
    done
done
cat task3_timings.csv
