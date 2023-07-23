////////////////////////////////////////////////////////////
// Copyright (c) 2013 Stephan Brumme. All rights Reserved.
// see http://create.stephan-brumme.com/disclaimer.html
// Page accessed: October 21, 2017 - 11:13:20 PM EST
// Author Stephan Brumme Rudolf-Breitscheid-Str. 226 14482 Potsdam, Germany
// License
// Unless otherwise noted in a file's first 5 lines, all source code and its sub-pages is licensed similar
// to the zlib license: This software is provided 'as-is', without any express or implied warranty. In no
// event will the author be held liable for any damages arising from the use of this software. Permission
// is granted to anyone to use this software for any purpose, including commercial applications, and to
// alter it and redistribute it freely, subject to the following restrictions:
// o - The origin of this software must not be misrepresented; you must not claim that you wrote the
//     original software.
// o - If you use this software in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
// o - Altered source versions must be plainly marked as such, and must not be misrepresented as being
//     the original software.
// Notice: This code has been modified to conform to TrueBlocks formatting preferences as well as other minor changes
//
#include <string>
#include "utillib.h"

//----------------------------------------------------------------------
CMemMapFile::CMemMapFile() : _file(0), _filename(), _filesize(0), _hint(Normal), _mappedBytes(0), _mappedView(NULL) {
}

//----------------------------------------------------------------------
CMemMapFile::CMemMapFile(const string_q& filename)
    : _file(0), _filename(filename), _filesize(0), _hint(Normal), _mappedBytes(WholeFile), _mappedView(NULL) {
    open(filename, _mappedBytes, _hint);
}

//----------------------------------------------------------------------
CMemMapFile::CMemMapFile(const string_q& filename, size_t mappedBytes, CacheHint hint)
    : _file(0), _filename(filename), _filesize(0), _hint(hint), _mappedBytes(mappedBytes), _mappedView(NULL) {
    open(filename, mappedBytes, hint);
}

//----------------------------------------------------------------------
CMemMapFile::~CMemMapFile() {
    close();
}

//----------------------------------------------------------------------
bool CMemMapFile::open(const string_q& filename, size_t mappedBytes, CacheHint hint) {
    if (isValid())
        return false;

    _file = 0;
    _filesize = 0;
    _hint = hint;
    _mappedView = NULL;

    _file = ::open(filename.c_str(), O_RDONLY);  // | O_LARGEFILE);
    if (_file == -1) {
        _file = 0;
        return false;
    }

    struct stat statInfo;
    if (fstat(_file, &statInfo) < 0)
        return false;

    _filesize = (uint64_t)statInfo.st_size;
    remap(0, mappedBytes);
    if (!_mappedView)
        return false;

    return true;
}

//----------------------------------------------------------------------
void CMemMapFile::close() {
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

//----------------------------------------------------------------------
unsigned char CMemMapFile::operator[](size_t offset) const {
    return ((unsigned char*)_mappedView)[offset];
}

//----------------------------------------------------------------------
unsigned char CMemMapFile::at(size_t offset) const {
    if (!_mappedView)
        throw std::invalid_argument("No view mapped");
    if (offset >= _filesize)
        throw std::out_of_range("View is not large enough");
    return operator[](offset);
}

//----------------------------------------------------------------------
const unsigned char* CMemMapFile::getData() const {
    return (const unsigned char*)_mappedView;
}

//----------------------------------------------------------------------
bool CMemMapFile::isValid() const {
    return _mappedView != NULL;
}

//----------------------------------------------------------------------
uint64_t CMemMapFile::size() const {
    return _filesize;
}

//----------------------------------------------------------------------
size_t CMemMapFile::mappedSize() const {
    return _mappedBytes;
}

//----------------------------------------------------------------------
bool CMemMapFile::remap(uint64_t offset, size_t mappedBytes) {
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

    _mappedView = ::mmap(NULL, mappedBytes, PROT_READ, MAP_SHARED, _file, (int64_t)offset);
    if (_mappedView == MAP_FAILED) {
        _mappedBytes = 0;
        _mappedView = NULL;
        return false;
    }

    _mappedBytes = mappedBytes;

    int linuxHint = 0;
    switch (_hint) {
        case Normal:
            linuxHint = MADV_NORMAL;
            break;
        case SequentialScan:
            linuxHint = MADV_SEQUENTIAL;
            break;
        case RandomAccess:
            linuxHint = MADV_RANDOM;
            break;
        default:
            break;
    }

    ::madvise(_mappedView, _mappedBytes, linuxHint);
    return true;
}

//----------------------------------------------------------------------
int CMemMapFile::getpagesize() {
    return static_cast<int>(sysconf(_SC_PAGESIZE));
}
