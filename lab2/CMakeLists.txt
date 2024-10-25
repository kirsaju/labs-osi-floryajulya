
add_executable(lab2 main.cpp include/medianFilter.h src/medianFilter.cpp)
FILE(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/image.pgm"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)
target_include_directories(lab2 PRIVATE include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
