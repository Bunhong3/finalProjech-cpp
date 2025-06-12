# Set the minimum required version of CMake. FetchContent is included in 3.14+.
cmake_minimum_required(VERSION 3.14)

# Set the project name to match your application.
project(WorkerManagementSystem)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the FetchContent module to manage dependencies at configure time.
include(FetchContent)

# --- Dependency: xlnt ---
# Declare the xlnt library, which is used for reading and writing Excel files.
# Using a specific git tag is recommended over 'master' for reproducible builds.
FetchContent_Declare(
  xlnt
  GIT_REPOSITORY https://github.com/tfussell/xlnt.git
  GIT_TAG v1.5.0
)

# --- Dependency: tabulate ---
# Declare the tabulate library, for creating nicely formatted text tables.
FetchContent_Declare(
  tabulate
  GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
  GIT_TAG master # This library does not have version tags, so master is used.
)

# This command ensures the dependencies are downloaded and prepared.
FetchContent_MakeAvailable(xlnt tabulate)

# Add the main executable target from your source file.
add_executable(WorkerManagementSystem main.cpp)

# Link the required libraries (xlnt and tabulate) to your executable.
target_link_libraries(WorkerManagementSystem PRIVATE xlnt tabulate)

# For Windows: Copy the correct xlnt DLL to the output directory after build.
if(WIN32)
  add_custom_command(
    TARGET WorkerManagementSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/_deps/xlnt-build/source/$<CONFIG>/$<IF:$<CONFIG:Debug>,xlntd.dll,xlnt.dll>"
        $<TARGET_FILE_DIR:WorkerManagementSystem>
    COMMENT "Copying xlnt DLL to output directory..."
  )
endif()

# Optional: Add compiler warnings to catch potential issues.
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
