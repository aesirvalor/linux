#!/bin/bash

set -e

# Read commit hashes from the file into an array
mapfile -t commit_hashes < bad_commits.txt

# Reverse the array
reversed_hashes=("${commit_hashes[@]}")
for ((i = 0; i < ${#commit_hashes[@]}; i++)); do
    reversed_hashes[$i]=${commit_hashes[${#commit_hashes[@]} - 1 - $i]}
done

# Apply each commit in reverse order
for hash in "${reversed_hashes[@]}"; do
    echo "Reverting commit hash $hash"
    git revert $hash
done
