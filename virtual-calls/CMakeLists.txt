set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(virtual-calls
  VirtualCalls.cpp
  )
target_link_libraries(virtual-calls
  clangTooling
  clangBasic
  clangASTMatchers
  )
