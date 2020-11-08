#pragma once
// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "basetypes.h"
#include "basenode.h"

namespace qblocks {

//--------------------------------------------------------------------------------
enum TokenType {
    tokEOS = 0,
    tokObjBegin,
    tokObjEnd,
    tokArrayBegin,
    tokArrayEnd,
    tokString,
    tokNumber,
    tokTrue,
    tokFalse,
    tokNull,
    tokNaN,
    tokPosInf,
    tokNegInf,
    tokArraySep,
    tokMemberSep,
    tokError
};

//--------------------------------------------------------------------------------
class Token {
  public:
    TokenType type_;
    const char* start_;
    const char* end_;
};

}  // namespace qblocks
