find_package(Catch2)
add_executable(hellotest hellotest.cpp)
target_link_libraries(hellotest PRIVATE Catch2::Catch2WithMain)