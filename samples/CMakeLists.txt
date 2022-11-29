include_directories(/usr/local/include/gecode)
link_directories(/usr/local/lib/gecode)
add_executable(australia australia-colors.cpp)
target_link_libraries(australia gecodekernel gecodeint gecodesupport)