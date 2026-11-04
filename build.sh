#!/usr/bin/env bash

set -Eeuo pipefail

# Source - https://stackoverflow.com/a/246128
# Posted by dogbane, modified by community. See post 'Timeline' for change history
# Retrieved 2026-11-01, License - CC BY-SA 4.0
readonly SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"

readonly BUILD_DIR_NAME="build"
readonly BUILD_DIR="$SCRIPT_DIR/$BUILD_DIR_NAME"

BUILD_COMPILER="clang"
BUILD_JOBS="$(nproc --ignore 2)"
BUILD_SYSTEM="Ninja"
BUILD_TYPE="Release"
OUTPUT_FILE="Software"

CLEAN=0
BUILD=0
RUN=0

Help()
{
    cat <<EOF
Usage: ${0##*/} [OPTIONS]

Options:
    -c, --compiler <name>       C compiler: clang or gcc (default: clang)
    -g, --generator <name>      Build generator: make or Ninja (default: Ninja)
    -t, --build-type <type>     Build type: Debug or Release (default: Release)
    -o, --out <file>            Copy the executable to <file>
    -j, --jobs <N>              Number of parallel build jobs
        --clean                 Remove the build directory
        --build                 Build the project
        --run                   Run the program
    -h, --help                  Show help
EOF
}

Args()
{
    while (($#)); do
        case "$1" in
            -c|--compiler|-g|--generator|-t|--build-type|-o|--out|-j|--jobs)
                option="$1"
                if (($# < 2)) || [[ "$2" == -* ]]; then
                    echo "Error: Option $option requires an argument" >&2
                    exit 1
                fi

                case "$option" in
                    -c|--compiler)
                        BUILD_COMPILER="$2"
                        ;;
                    -g|--generator)
                        BUILD_SYSTEM="$2"
                        ;;
                    -t|--build-type)
                        BUILD_TYPE="$2"
                        ;;
                    -o|--out)
                        OUTPUT_FILE="$2"
                        ;;
                    -j|--jobs)
                        BUILD_JOBS="$2"
                        ;;
                esac
                shift 2
                ;;
            --clean)
                CLEAN=1
                shift
                ;;
            --build)
                BUILD=1
                shift
                ;;
            --run)
                RUN=1
                shift
                ;;
            -h|--help)
                Help
                exit 0
                ;;
            *)
                echo "Error: Unknown option: $1" >&2
                echo
                Help >&2
                exit 1
                ;;
        esac
    done

    case "$BUILD_COMPILER" in
        clang|gcc)
            ;;
        *)
            echo "Error: Compiler must be 'clang' or 'gcc'" >&2;
            exit 1
            ;;
    esac

    case "${BUILD_SYSTEM,,}" in
        ninja)
            BUILD_SYSTEM="Ninja"
            ;;
        make)
            BUILD_SYSTEM="Unix Makefiles"
            ;;
        *)
            echo "Error: Generator must be 'make' or 'ninja'" >&2;
            exit 1
            ;;
    esac

    case "$BUILD_TYPE" in
        Debug|Release)
            ;;
        *)
            echo "Error: Build type must be 'Debug' or 'Release'" >&2;
            exit 1
            ;;
    esac

    if [[ ! "$BUILD_JOBS" =~ ^[1-9][0-9]*$ ]]; then
        echo "Error: Jobs must be a positive integer" >&2
        exit 1
    fi

    if (( !CLEAN && !BUILD && !RUN )); then
        Help
        exit 0
    fi
}

Clean()
{
    echo "-- Cleaning: $BUILD_DIR"
    rm -rf -- "$BUILD_DIR"
}

Configure()
{
    echo "-- Configuring project"
    cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" -G "$BUILD_SYSTEM" \
          -DCMAKE_C_COMPILER="$BUILD_COMPILER" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
          -DOUTPUT_NAME="$OUTPUT_FILE"

    # For cmake build type https://stackoverflow.com/a/20423820/33231317 :>
}

Build()
{
    echo "-- Building project"
    cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" --parallel "$BUILD_JOBS"
}

Executable()
{
    if [[ ! -x "$BUILD_DIR/$OUTPUT_FILE" ]]; then
        echo "Error: Executable not found: $OUTPUT_FILE" >&2
        return 1
    fi

    echo "-- Executing: $BUILD_DIR/$OUTPUT_FILE"
    exec "$BUILD_DIR/$OUTPUT_FILE"
}

main()
{
    Args "$@"

    echo "-- Base path: $SCRIPT_DIR"
    echo "-- Build path: $BUILD_DIR"
    echo "-- Build type: $BUILD_TYPE!"
    echo "-- Jobs: $BUILD_JOBS"

    if (( CLEAN )); then
        Clean
    fi

    if (( BUILD )); then
        Configure
        Build
    fi

    if (( RUN )); then
        Executable
    fi
}

main "$@"

# I'm sorry I have used AI for the cases code
