// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for io.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

#include <unistd.h>
#include <fcntl.h>

static inline int _open(const char* filename, int oflag, ...) { return open(filename, oflag); }
static inline int _read(int fd, void* buf, size_t nbyte) { return read(fd, buf, nbyte); }
static inline int _write(int fd, const void* buf, size_t nbyte) { return write(fd, buf, nbyte); }
static inline int _close(int fd) { return close(fd); }
static inline long _lseek(int fd, long offset, int whence) { return lseek(fd, offset, whence); }
static inline int _access(const char* path, int mode) { return access(path, mode); }

#endif // ORBITER_BUILD_SDLGPUCLIENT
