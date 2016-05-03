#!/bin/bash
./player `hostname` 9999 > /tmp/p1 &
./player `hostname` 9999 > /tmp/p2 &
./player `hostname` 9999 > /tmp/p3 &
./master 9999 3 1000 > /tmp/m &
