#!/bin/bash

/home/di/alpaca.cpp/main --interactive --seed -1 --threads 4 --n_predict 1 --model \
    ~/dalai/alpaca/models/7B/ggml-model-q4_0.bin --top_k 40 --top_p 0.9 --temp 0.8 \
    --repeat_last_n 64 --repeat_penalty 1.3 -o 65530 &

python /home/di/alpaca.cpp/alpaca-serve.py