include_directories(/usr/local/include/gecode)
link_directories(/usr/local/lib/gecode)

add_executable(australia australia-colors.cpp)
target_link_libraries(australia gecodekernel gecodeint gecodesupport)

add_executable(send-more-money send-more-money.cpp)
target_link_libraries(send-more-money gecodesearch gecodekernel gecodeint gecodesupport)