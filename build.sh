#!/bin/bash

set -e

CLEAN_BUILD=false
RUN_TESTS=false

for arg in "$@"; do
  case $arg in
  --clean | -c)
    CLEAN_BUILD=true
    ;;
  --test | -t)
    RUN_TESTS=true
    ;;
  esac
done

echo "Building Horizon..."

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

if [ "$CLEAN_BUILD" = true ]; then
  echo "Cleaning build directory..."
  rm -rf build
fi

mkdir -p build && cd build

cmake .. -DCMAKE_BUILD_TYPE=Release -DHORIZON_BUILD_EXAMPLES=ON -DHORIZON_BUILD_TESTS=ON -G "${GENERATOR}"
make -j${JOBS}
echo "Build complete!"

if [[ "$OSTYPE" == "darwin"* ]]; then
  export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$(pwd)/core:$(pwd)/rhi
else
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/core:$(pwd)/rhi
fi

if [ "$RUN_TESTS" = true ]; then
  echo "Running Horizon Unit Tests..."
  if [[ -f "tests/horizon_tests" ]]; then
    ./tests/horizon_tests || true
  elif [[ -f "bin/horizon_tests" ]]; then
    ./bin/horizon_tests || true
  else
    echo "Error: horizon_tests not found!"
    exit 1
  fi
  exit 0
fi

run_example() {
  local exe_path="$1"
  if [[ -f "$exe_path" ]]; then
    echo "Running $exe_path..."
    "$exe_path"
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
  find . -name "hello_triangle" -type f 2>/dev/null || echo "No hello_triangle found"
  exit 1
fi
