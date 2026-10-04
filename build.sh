#!/usr/bin/env bash

EXECUTABLE_FILE="Software"
BUILD_PATH="build"
JOBS=8


SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
BUILD_DIR="$SCRIPT_DIR/$BUILD_PATH"


echo "-- Base path: $SCRIPT_DIR"
echo "-- Build path: $BUILD_DIR"
echo "-- Jobs set to: $JOBS"


RUN_EXEC=false
for arg in "$@"; do
    if  [[ "$arg" == "-r" ]]; then
        RUN_EXEC=true
    fi
done


cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" -G Ninja
CONFIGURE_EXIT=$?

cmake --build "$BUILD_DIR" --parallel "$JOBS"
BUILD_EXIT=$?

# -eq is equals and I use it as `error_check == 0` here
if [[ "$RUN_EXEC" == true && $CONFIGURE_EXIT -eq 0 && $BUILD_EXIT -eq 0 ]]; then
    echo "-- Executing: $EXECUTABLE_FILE"
    exec "$BUILD_DIR/$EXECUTABLE_FILE"
fi
