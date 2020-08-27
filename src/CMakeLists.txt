set (EMU_SRCS
  arithmetic.c
  branch.c
  cpu.c
  data.c
  decoder.c
  disasm.c
  logic.c
  main.c
  special.c
)

add_executable(${PROJECT_NAME} ${EMU_SRCS})
target_link_libraries(${PROJECT_NAME} Threads::Threads)
