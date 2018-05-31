set(CUBATURE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cubature")
set(CUBATURE_SRC "${CUBATURE_DIR}/hcubature.c" "${CUBATURE_DIR}/pcubature.c")
set(CUBATURE_HEADERS "${CUBATURE_DIR}/cubature.h")

include_directories("${CUBATURE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_library(cubature SHARED ${CUBATURE_SRC})

add_executable(cubature_wrap main.cpp)
target_link_libraries(cubature_wrap cubature)

install(TARGETS cubature DESTINATION lib)
install(FILES "${CUBATURE_HEADERS}" cubature_wrap.hpp DESTINATION include)
