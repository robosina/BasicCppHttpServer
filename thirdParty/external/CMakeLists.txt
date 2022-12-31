# spdlog get and populate

include(FetchContent)

message(STATUS "Fetching spdlog")
#
# SPDLOG dependency
#
FetchContent_Declare(
  spdlog
  VERBOSE
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG ad0e89cbfb4d0c1ce4d097e134eb7be67baebb36
)
FetchContent_MakeAvailable(spdlog)

message(STATUS "Fetching gtest")
#
# GTEST dependency
#
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
