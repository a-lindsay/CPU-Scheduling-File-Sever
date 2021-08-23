#!/bin/bash

./mserver &

for i in $(seq 1 10); do
	./client 127.0.0.1:3000 file &
done

