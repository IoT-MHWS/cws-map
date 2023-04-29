set(PROJECT_NAME app)

project(${PROJECT_NAME}
  DESCRIPTION "Local app to test functionality"
  LANGUAGES C CXX
)

add_executable(${PROJECT_NAME}
  ./main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  cws-map
)
