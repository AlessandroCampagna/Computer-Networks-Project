#!/bin/bash

# Define the directory to run the 'tree' command on
DIRECTORY="ASDIR"

# Function to execute 'tree' command and clear the screen
run_tree() {
  clear
  tree "$DIRECTORY"
}

# Trap Ctrl+C to exit the script gracefully
trap 'echo -e "\nScript terminated by user. Exiting..."; exit 0' INT

# Run the 'tree' command and clear the screen every 1 second
while true; do
  run_tree
  sleep 1
done
