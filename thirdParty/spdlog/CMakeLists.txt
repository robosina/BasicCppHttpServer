# spdlog get and populate

include(FetchContent)

message(STATUS "spdlog: Fetching spdlog")
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
