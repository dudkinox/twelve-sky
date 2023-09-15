#!/bin/sh
valgrind --trace-malloc=yes --trace-children=yes --tool=memcheck --leak-check=yes -v --show-reachable=yes --log-file=memtx ./ctsd
