cmake_minimum_required(VERSION 3.0)

project("8080emu" VERSION 0.0.1 LANGUAGES C)

# Find PThreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(src)
