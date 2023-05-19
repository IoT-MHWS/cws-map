include(FetchContent)

include(cmake/CPM.cmake)
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.3")

FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG v1.54.0
)
FetchContent_MakeAvailable(gRPC)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)
