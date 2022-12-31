add_executable(Test
  main.cpp
  ${SOURCES}
)

target_link_libraries(
  Test
  PRIVATE
  GTest::gtest_main
)