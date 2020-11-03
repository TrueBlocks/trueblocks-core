#pragma once
// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "json_value.h"

namespace qblocks {

//--------------------------------------------------------------------------------
class Value;

//--------------------------------------------------------------------------------
class StreamWriter {
  public:
    StreamWriter(void) { }
    ~StreamWriter() = default;

    int write(ostream& sout, const Value& root);

  private:
    void writeValue(ostream& sout, const Value& value);
    vector<string_q> childValues_;
};

}  // namespace qblocks
