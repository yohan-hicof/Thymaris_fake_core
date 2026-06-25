cmake_minimum_required(VERSION 3.20)

set(projectName "Thymaris_fake_core")

project(${projectName})

set(CMAKE_CXX_STANDARD 20)


# Set compiler options
add_compile_options(-Wall)
add_compile_options(-fexceptions)

# Add include directories
include_directories(
        /usr/local/include
)

# Add source files
set(SOURCES
        base64.cpp
        database_functions.cpp
        main.cpp
)

set(HEADER_FILES
        base64.h
        database_functions.h
        main.h
        structures.h
)

# Add link directories
link_directories(
        /usr/local/lib
)

# Create executable targets
add_executable(${projectName} ${SOURCES})

if(APP_VERSION)
    target_compile_definitions(${projectName} PRIVATE APP_VERSION="${APP_VERSION}")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Link libraries
target_link_libraries(${projectName}
        pthread        
        pqxx
        pq
        Threads::Threads
)
