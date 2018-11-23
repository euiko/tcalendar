add_executable(calendar calendar.cpp)
find_package(Threads REQUIRED)

target_link_libraries(calendar cppurses -lncurses -pthread)
target_compile_features(calendar PRIVATE cxx_std_14)