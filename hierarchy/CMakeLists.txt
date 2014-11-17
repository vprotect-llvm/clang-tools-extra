set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(hierarchy
  Hierarchy.cpp
  )
target_link_libraries(hierarchy
  clangTooling
  clangBasic
  clangASTMatchers
  )
