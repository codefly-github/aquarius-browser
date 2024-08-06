#!/bin/bash

# Ensure we're in the project root
cd "$(dirname "$0")"

# Main branch name
MAIN_BRANCH="main"

# Function to add, commit, and push a chunk
commit_and_push_chunk() {
    local CHUNK=$1
    local BRANCH_NAME="chunk-${CHUNK//\//-}"

    # Create a new branch for the chunk
    git checkout -b "$BRANCH_NAME"

    # Add the chunk
    git add "$CHUNK"

    # Commit the chunk
    git commit -m "Add $CHUNK"

    # Push the branch to GitHub
    git push origin "$BRANCH_NAME"

    # Merge the chunk branch into the main branch
    git checkout "$MAIN_BRANCH"
    git pull origin "$MAIN_BRANCH"
    git merge "$BRANCH_NAME"

    # Push the updated main branch to GitHub
    git push origin "$MAIN_BRANCH"

    # Delete the chunk branch locally and remotely
    git branch -d "$BRANCH_NAME"
    git push origin --delete "$BRANCH_NAME"
}

# Populate CHUNKS array with top-level directories and files
CHUNKS=($(find . -maxdepth 1 -mindepth 1 -type d -or -type f | sed 's|^\./||'))

# Loop through each chunk and process it
for CHUNK in "${CHUNKS[@]}"; do
    commit_and_push_chunk "$CHUNK"
done

