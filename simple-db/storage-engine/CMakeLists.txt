cmake_minimum_required(VERSION 3.10)
project(storage_engine)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for clang-tidy + IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# ------------------------------------
# Googletest via FetchContent
# ------------------------------------
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

# ------------------------------------
# clang-tidy configuration
# ------------------------------------
find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if (CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")

    # Make clang-tidy aware of full compile flags and include paths
    set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_EXE}"
        "-p=${CMAKE_BINARY_DIR}"
    )
endif()

# ------------------------------------
# Sanitizers
# ------------------------------------
option(SANITIZE "Enable sanitizers" OFF)

if(SANITIZE)
    set(SANITIZER_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${SANITIZER_FLAGS}")
endif()
