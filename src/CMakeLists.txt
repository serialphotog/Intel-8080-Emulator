set (EMU_SRCS
 sys/arithmetic.c
  sys/branch.c
  sys/cpu.c
  sys/data.c
  sys/decoder.c
  sys/disasm.c
  sys/logic.c
  sys/special.c
  display/display.c
  main.c
)
include_directories(.)

add_executable(${PROJECT_NAME} ${EMU_SRCS})
target_link_libraries(${PROJECT_NAME} Threads::Threads ${GTK3_LIBRARIES})
