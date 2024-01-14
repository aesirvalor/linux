#!/bin/bash

set -e

# Read commit hashes from the file into an array
mapfile -t commit_hashes < commit_hashes.txt

# Reverse the array
reversed_hashes=("${commit_hashes[@]}")
for ((i = 0; i < ${#commit_hashes[@]}; i++)); do
    reversed_hashes[$i]=${commit_hashes[${#commit_hashes[@]} - 1 - $i]}
done

# Apply each commit in reverse order
for hash in "${reversed_hashes[@]}"; do
    if grep -q "$hash" processed_commits.txt; then
        echo "Commit $commit_hash has been processed."
    else
        echo "Commit $commit_hash has not been processed yet."
        echo "$hash" >> processed_commits.txt
        output=$(git cherry-pick --allow-empty  --strategy-option=theirs $hash)
        if echo "$output" | grep -q "After resolving the conflicts"; then
            
            exit 1 
        fi
        
    fi
    #git cherry-pick --strategy-option=theirs $hash
    #echo "$hash" >> processed_commits.txt
done
