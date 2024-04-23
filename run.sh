#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to the source directory
cd src || exit

# Compile the C code
if gcc -o ../build/main main.c hash_table.c; then
    echo "Compilation successful!"
else
    echo "Compilation failed."
    exit 1
fi

# Navigate back to the original directory
cd ..

# Run the executable
echo "Running the executable..."
./build/main

# Wait for user input before exiting
read -rp "Press Enter to exit..."
