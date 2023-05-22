#!/bin/bash

set -e

# Set defaults
CONFIG="/home/di/config/config.sh"
SEED="-1"
THREADS="4"
N_PREDICT="1"
MODEL="/home/di/alpacca.cpp"
TOP_K="40"
TOP_P="0.9"
TEMP="0.8"
REPEAT_LAST_N="64"
REPEAT_PENALTY="1.3"
PORT="65530"
MODEL="7B/ggml-model-q4_0.bin"

# Source configuration
[ -e $CONFIG ] && echo "Sourcing configuration at $CONFIG" && source $CONFIG

/home/di/alpaca.cpp/main \
    --interactive \
    --seed $SEED \
    --threads $THREADS \
    --n_predict $N_PREDICT \
    --model /home/di/dalai/alpaca/models/$MODEL \
    --top_k $TOP_K \
    --top_p $TOP_P \
    --temp $TEMP \
    --repeat_last_n $REPEAT_LAST_N \
    --repeat_penalty $REPEAT_PENALTY \
    -o $PORT &

echo "Starting http server..."
python /home/di/alpaca.cpp/alpaca-serve.py