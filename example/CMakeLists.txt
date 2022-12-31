add_executable(App src/example.cpp)
target_link_libraries(App
  HttpServer
  backward
  )

# copy shell script to build directory
add_custom_command(TARGET App POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${PROJECT_SOURCE_DIR}/scripts/benchmark.sh
  ${CMAKE_BINARY_DIR}/bin/benchmark.sh)

# copy html files to build directory
file(COPY ${PROJECT_SOURCE_DIR}/pages/index.html DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${PROJECT_SOURCE_DIR}/pages/favicon.ico DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})