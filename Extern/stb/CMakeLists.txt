# Use local stb_image (single-header image loading library)
add_library(stb INTERFACE)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb_clone)

