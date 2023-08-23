#pragma once
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
#include <stdexcept>

class CMemMapFile {
  public:
    enum CacheHint {
        Normal,          ///< good overall performance
        SequentialScan,  ///< read file only once with few seeks
        RandomAccess     ///< jump around
    };

    enum MapRange {
        WholeFile = 0  ///< everything ... be careful when file is larger than memory
    };

    CMemMapFile(void);
    explicit CMemMapFile(const std::string& filename);
    CMemMapFile(const std::string& filename, size_t mappedBytes, CacheHint hint);

    ~CMemMapFile(void);

    bool open(const std::string& filename, size_t mappedBytes = WholeFile, CacheHint hint = Normal);
    void close(void);

    unsigned char operator[](size_t offset) const;
    unsigned char at(size_t offset) const;

    const unsigned char* getData(void) const;

    bool isValid(void) const;
    bool is_open(void) const {
        return _file != 0;
    }

    uint64_t size(void) const;
    size_t mappedSize(void) const;

    bool remap(uint64_t offset, size_t mappedBytes);

  private:
    CMemMapFile(const CMemMapFile&);
    CMemMapFile& operator=(const CMemMapFile&);

    static int getpagesize(void);

    int _file;
    std::string _filename;
    uint64_t _filesize;
    CacheHint _hint;
    size_t _mappedBytes;
    void* _mappedView;
};
