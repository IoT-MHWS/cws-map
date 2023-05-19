file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
  ./algo/illumination.cpp
)

add_executable(illumination ${SRCS})
