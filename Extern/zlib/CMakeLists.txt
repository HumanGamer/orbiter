project(zlib)

Include(FetchContent)

set(SKIP_INSTALL_ALL ON)

FetchContent_Declare(
  zlib
  GIT_REPOSITORY https://github.com/madler/zlib.git
  GIT_TAG v1.2.11
)

# Patch to fix fdopen redefinition conflict with macOS system headers
FetchContent_Populate(zlib)
file(READ "${zlib_SOURCE_DIR}/zutil.h" ZUTIL_CONTENT)
string(REPLACE "#        define fdopen(fd,mode) NULL /* No fdopen() */" "" ZUTIL_CONTENT "${ZUTIL_CONTENT}")
string(REPLACE "#  define fdopen(fd,mode) NULL /* No fdopen() */" "" ZUTIL_CONTENT "${ZUTIL_CONTENT}")
string(REPLACE "#    define fdopen(fd,mode) NULL /* No fdopen() */" "" ZUTIL_CONTENT "${ZUTIL_CONTENT}")
file(WRITE "${zlib_SOURCE_DIR}/zutil.h" "${ZUTIL_CONTENT}")

# Build zlib from the patched source
add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
target_include_directories(zlib INTERFACE ${zlib_SOURCE_DIR})
target_include_directories(zlib INTERFACE ${zlib_BINARY_DIR})

set_target_properties(zlib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${ORBITER_BINARY_ROOT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${ORBITER_BINARY_ROOT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${ORBITER_BINARY_ROOT_DIR}
)
install(TARGETS zlib RUNTIME DESTINATION ${ORBITER_INSTALL_ROOT_DIR})
