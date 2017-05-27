#pragma once
// //////////////////////////////////////////////////////////
// Copyright (c) 2013 Stephan Brumme. All rights Reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <string>
#include <stdexcept>
#include <cstdio>

class MemoryMapped {
public:
    enum CacheHint {
        Normal,          ///< good overall performance
        SequentialScan,  ///< read file only once with few seeks
        RandomAccess     ///< jump around
    };

    enum MapRange {
        WholeFile = 0    ///< everything ... be careful when file is larger than memory
    };

    MemoryMapped(void);
    explicit MemoryMapped(const std::string& filename);
    MemoryMapped(const std::string& filename, size_t mappedBytes, CacheHint hint);

    ~MemoryMapped(void);

    bool open(const std::string& filename, size_t mappedBytes = WholeFile, CacheHint hint = Normal);
    void close(void);

    unsigned char operator[](size_t offset) const;
    unsigned char at(size_t offset) const;

    const unsigned char* getData(void) const;

    bool isValid(void) const;

    uint64_t size(void) const;
    size_t   mappedSize(void) const;

    bool remap(uint64_t offset, size_t mappedBytes);

private:
    MemoryMapped(const MemoryMapped&);
    MemoryMapped& operator=(const MemoryMapped&);

    static int getpagesize(void);

    int         _file;
    std::string _filename;
    uint64_t    _filesize;
    CacheHint   _hint;
    size_t      _mappedBytes;
    void*       _mappedView;
};
