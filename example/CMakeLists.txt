add_executable(basicUsage src/main.cpp)
add_executable(Usage src/example.cpp)

target_link_libraries(basicUsage HttpServer)
target_link_libraries(Usage HttpServer)