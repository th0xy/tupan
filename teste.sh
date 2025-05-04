#!/bin/bash

for N in {1..50}
do
	nc 127.0.0.1 &
done;
