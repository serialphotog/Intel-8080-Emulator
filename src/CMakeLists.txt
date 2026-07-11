set (EMU_SRCS
  arithmetic.c
  branch.c
  cpu.c
  data.c
  decoder.c
  disasm.c
  logic.c
  main.c
	platform.c
  special.c
)

add_executable(${PROJECT_NAME} ${EMU_SRCS})
find_package(SDL2 CONFIG QUIET)
if (NOT SDL2_FOUND)
  include(FetchContent)
  message(STATUS "SDL2 was not found; downloading SDL 2.30.11")
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_TEST OFF CACHE BOOL "" FORCE)
  set(SDL_TESTS OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    SDL2
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.11/SDL2-2.30.11.tar.gz
    URL_HASH SHA256=8b8d4aef2038533da814965220f88f77d60dfa0f32685f80ead65e501337da7f
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
  FetchContent_MakeAvailable(SDL2)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 Threads::Threads)
if (TARGET SDL2::SDL2main)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()
