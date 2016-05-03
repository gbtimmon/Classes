#!/bin/bash
./player `hostname` 9999 | tee /tmp/p1 &
./player `hostname` 9999 | tee /tmp/p2 &
./player `hostname` 9999 | tee /tmp/p3 &
./master 9999 3 10     | tee /tmp/m &
