add_executable(kernel32 kernel32.cpp)
target_link_libraries(kernel32 mspdbx::mspdbx)