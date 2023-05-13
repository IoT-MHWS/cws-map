
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./illumination.cpp
)

add_executable(illumination ${SRCS})
