# Fetch stb_image from GitHub (single-header image loading library)
include(FetchContent)

FetchContent_Declare(stb
  URL https://github.com/nothings/stb/archive/refs/heads/master.tar.gz
  URL_HASH SHA256=8d43e0e427c9f1a59f3e9dfe71ec0dccc3f0faeb58e259cb6a1a9b0e81e06e0a
)

FetchContent_MakeAvailable(stb)

add_library(stb INTERFACE)
add_library(stb::image INTERFACE)
target_include_directories(stb::image INTERFACE ${stb_SOURCE_DIR})
