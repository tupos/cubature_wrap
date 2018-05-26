set(CUBATURE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cubature")
set(CUBATURE_SRC "${CUBATURE_DIR}/hcubature.c" "${CUBATURE_DIR}/pcubature.c")

include_directories("${CUBATURE_DIR}")

add_library(cubature SHARED ${CUBATURE_SRC})

add_executable(cub_wrap main.cpp)
target_link_libraries(cub_wrap cubature)
