// //////////////////////////////////////////////////////////
// MemoryMapped.cpp
// Copyright (c) 2013 Stephan Brumme. All rights Reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//
#include <string>
#include "./MemoryMapped.h"

MemoryMapped::MemoryMapped()
: _file(0), _filename(), _filesize(0), _hint(Normal), _mappedBytes(0), _mappedView(NULL) { }

MemoryMapped::MemoryMapped(const std::string& filename)
: _file(0), _filename(filename), _filesize(0), _hint(Normal), _mappedBytes(WholeFile), _mappedView(NULL) {
    open(filename, _mappedBytes, _hint);
}

MemoryMapped::MemoryMapped(const std::string& filename, size_t mappedBytes, CacheHint hint)
: _file(0), _filename(filename), _filesize(0), _hint(hint), _mappedBytes(mappedBytes), _mappedView(NULL) {
    open(filename, mappedBytes, hint);
}

MemoryMapped::~MemoryMapped() {
    close();
}

bool MemoryMapped::open(const std::string& filename, size_t mappedBytes, CacheHint hint) {
    if (isValid())
        return false;

    _file       = 0;
    _filesize   = 0;
    _hint       = hint;
    _mappedView = NULL;

    _file = ::open(filename.c_str(), O_RDONLY);  // | O_LARGEFILE);
    if (_file == -1) {
        _file = 0;
        return false;
    }

    struct stat statInfo;
    if (fstat(_file, &statInfo) < 0)
        return false;

    _filesize = statInfo.st_size;
    remap(0, mappedBytes);
    if (!_mappedView)
        return false;

    return true;
}

void MemoryMapped::close() {
    _filesize = 0;
    if (_mappedView) {
        ::munmap(_mappedView, _filesize);
        _mappedView = NULL;
    }

    if (_file) {
        ::close(_file);
        _file = 0;
    }
}

unsigned char MemoryMapped::operator[](size_t offset) const {
    return ((unsigned char*)_mappedView)[offset];
}

unsigned char MemoryMapped::at(size_t offset) const {
    if (!_mappedView)
        throw std::invalid_argument("No view mapped");
    if (offset >= _filesize)
        throw std::out_of_range("View is not large enough");
    return operator[](offset);
}

const unsigned char* MemoryMapped::getData() const {
    return (const unsigned char*)_mappedView;
}

bool MemoryMapped::isValid() const {
    return _mappedView != NULL;
}

uint64_t MemoryMapped::size() const {
    return _filesize;
}

size_t MemoryMapped::mappedSize() const {
    return _mappedBytes;
}

bool MemoryMapped::remap(uint64_t offset, size_t mappedBytes) {
    if (!_file)
        return false;

    if (mappedBytes == WholeFile)
        mappedBytes = _filesize;

    if (_mappedView) {
        ::munmap(_mappedView, _mappedBytes);
        _mappedView = NULL;
    }

    if (offset > _filesize)
        return false;
    if (offset + mappedBytes > _filesize)
        mappedBytes = size_t(_filesize - offset);

    _mappedView = ::mmap(NULL, mappedBytes, PROT_READ, MAP_SHARED, _file, offset);
    if (_mappedView == MAP_FAILED) {
        _mappedBytes = 0;
        _mappedView  = NULL;
        return false;
    }

    _mappedBytes = mappedBytes;

    int linuxHint = 0;
    switch (_hint) {
        case Normal:         linuxHint = MADV_NORMAL;     break;
        case SequentialScan: linuxHint = MADV_SEQUENTIAL; break;
        case RandomAccess:   linuxHint = MADV_RANDOM;     break;
        default: break;
    }

    ::madvise(_mappedView, _mappedBytes, linuxHint);
    return true;
}

int MemoryMapped::getpagesize() {
    return static_cast<int>(sysconf(_SC_PAGESIZE));
}
