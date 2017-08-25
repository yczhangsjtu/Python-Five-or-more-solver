#!/bin/bash

for i in `seq 1 100`; do
	./fomanl -te | python sortout.py >> data
done
