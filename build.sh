#!/bin/bash

set -e

echo "Building Quantum..."

# macOS
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Detected macOS"
    if ! brew list molten-vk &>/dev/null; then
        echo "Warning: MoltenVK not installed. Install with: brew install molten-vk"
    fi
    JOBS=$(sysctl -n hw.ncpu)
    GENERATOR="Unix Makefiles"
else
    # Linux
    JOBS=$(nproc)
    GENERATOR="Unix Makefiles"
fi

# rm -rf build

mkdir -p build && cd build

cmake .. -DCMAKE_BUILD_TYPE=Release -DQUANTUM_BUILD_EXAMPLES=ON -G "${GENERATOR}"
make -j${JOBS}
echo "Build complete!"

# Function to run with correct LD_LIBRARY_PATH/DYLD_LIBRARY_PATH
run_example() {
    local exe_path="$1"
    if [[ -f "$exe_path" ]]; then
        echo "Running $exe_path..."
        if [[ "$OSTYPE" == "darwin"* ]]; then
            # macOS uses DYLD_LIBRARY_PATH
            export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$(pwd)/core:$(pwd)/rhi
            "$exe_path"
        else
            # Linux
            export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/core:$(pwd)/rhi
            "$exe_path"
        fi
        return 0
    fi
    return 1
}

echo "Search hello_triangle..."
if run_example "examples/hello_triangle/hello_triangle"; then
    :
elif run_example "bin/hello_triangle"; then
    :
else
    echo "Error: hello_triangle not found!"
    echo "Searching for executable..."
    find . -name "hello_triangle" -type f 2>/dev/null || echo "No hello_triangle found"
    exit 1
fi
