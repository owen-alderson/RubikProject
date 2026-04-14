cmake_minimum_required(VERSION 3.10)
project(RubiksSolver C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find pthreads for your multithreaded solver
find_package(Threads REQUIRED)

# All source files live in src/
set(SRC_FILES
    src/main.c
    src/game.c
    src/cube.c
    src/solver.c
    src/hints.c
    src/timer.c
)

add_executable(rubiks_solver ${SRC_FILES})

# So the compiler can find your headers like "cube.h", "game.h", etc.
target_include_directories(rubiks_solver PRIVATE src)

# Link pthreads
target_link_libraries(rubiks_solver PRIVATE Threads::Threads)
