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
  protected:
    std::ostream* sout_;

  public:
    StreamWriter();
    virtual ~StreamWriter();
    virtual int write(Value const& root, std::ostream* sout) = 0;
    class Factory {
      public:
        virtual ~Factory();
        virtual StreamWriter* newStreamWriter() const = 0;
    };
};

//--------------------------------------------------------------------------------
std::string writeString(StreamWriter::Factory const& factory, Value const& root);

//--------------------------------------------------------------------------------
class StreamWriterBuilder : public StreamWriter::Factory {
  public:
    Value settings_;
    StreamWriterBuilder();
    ~StreamWriterBuilder() override;

    StreamWriter* newStreamWriter() const override;
    bool validate(Value* invalid) const;
    Value& operator[](const std::string& key);
    static void setDefaults(Value* settings);
};

//--------------------------------------------------------------------------------
std::string valueToQuotedString(const char* value);

//--------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream&, const Value& root);

}  // namespace qblocks
