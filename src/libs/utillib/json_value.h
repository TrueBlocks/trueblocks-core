#pragma once
// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include <sstream>
#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <cmath>
#include <set>
#include <assert.h>

#include "basetypes.h"
#include "exportcontext.h"
#include "json_token.h"

namespace qblocks {

//--------------------------------------------------------------------------------
void throwRuntimeError(std::string const& msg);

//--------------------------------------------------------------------------------
enum ValueType {
    nullValType = 0,
    intValType,
    uintValType,
    realValType,
    strValType,
    boolValType,
    arrayValType,
    objValType
};

//--------------------------------------------------------------------------------
/** \brief Lightweight wrapper to tag static string.
 *
 * Value constructor and objValType member assignment takes advantage of the
 * StaticString and avoid the cost of string duplication when storing the
 * string or the member name.
 *
 * Example of usage:
 * \code
 * Json::Value aValue( StaticString("some text") );
 * Json::Value object;
 * static const StaticString code("code");
 * object[code] = 1234;
 * \endcode
 */
class StaticString {
  public:
    explicit StaticString(const char* czstring) : c_str_(czstring) {
    }

    operator const char*() const {
        return c_str_;
    }

    const char* c_str() const {
        return c_str_;
    }

  private:
    const char* c_str_;
};

//--------------------------------------------------------------------------------
class CZString {
  public:
    enum DuplicationPolicy { noDuplication = 0, duplicate, duplicateOnCopy };
    CZString(unsigned int index);
    CZString(char const* str, unsigned length, DuplicationPolicy allocate);
    CZString(CZString const& other);
    CZString(CZString&& other);
    ~CZString();
    CZString& operator=(const CZString& other);
    CZString& operator=(CZString&& other);
    bool operator<(CZString const& other) const;
    bool operator==(CZString const& other) const;
    unsigned int index() const;
    char const* data() const;
    unsigned length() const;
    bool isStaticString() const;

  private:
    struct StringStorage {
        unsigned policy_ : 2;
        unsigned length_ : 30;  // 1GB max
    };
    char const* cstr_;  // actually, a prefixed string, unless policy is noDup
    union {
        unsigned int index_;
        StringStorage storage_;
    };
};

//--------------------------------------------------------------------------------
class ValueIterator;

//--------------------------------------------------------------------------------
class ValueConstIterator;

//--------------------------------------------------------------------------------
class Value {
    friend class ValueIteratorBase;

  public:
    using iterator = ValueIterator;
    using const_iterator = ValueConstIterator;

    /// Minimum signed integer value that can be stored in a Json::Value.
    static constexpr int64_t minLargestInt = int64_t(~(uint64_t(-1) / 2));
    /// Maximum signed integer value that can be stored in a Json::Value.
    static constexpr int64_t maxLargestInt = int64_t(uint64_t(-1) / 2);
    /// Maximum unsigned integer value that can be stored in a Json::Value.
    static constexpr uint64_t maxLargestUInt = uint64_t(-1);

    /// Minimum signed int value that can be stored in a Json::Value.
    static constexpr int minInt = int(~((unsigned int)(-1) / 2));
    /// Maximum signed int value that can be stored in a Json::Value.
    static constexpr int maxInt = int((unsigned int)(-1) / 2);
    /// Maximum unsigned int value that can be stored in a Json::Value.
    static constexpr unsigned int maxUInt = (unsigned int)(-1);

    /// Minimum signed 64 bits int value that can be stored in a Json::Value.
    static constexpr int64_t minInt64 = int64_t(~(uint64_t(-1) / 2));
    /// Maximum signed 64 bits int value that can be stored in a Json::Value.
    static constexpr int64_t maxInt64 = int64_t(uint64_t(-1) / 2);
    /// Maximum unsigned 64 bits int value that can be stored in a Json::Value.
    static constexpr uint64_t maxUInt64 = uint64_t(-1);

    // The constant is hard-coded because some compiler have trouble
    // converting Value::maxUInt64 to a double correctly (AIX/xlC).
    // Assumes that uint64_t is a 64 bits integer.
    static constexpr double maxUInt64AsDouble = 18446744073709551615.0;

  public:
    Value(ValueType type = nullValType);
    Value(int value);
    Value(unsigned int value);
    Value(int64_t value);
    Value(uint64_t value);
    Value(double value);
    Value(const char* value);
    Value(const char* begin, const char* end);
    Value(const StaticString& value);
    Value(const std::string& value);
    Value(bool value);
    Value(std::nullptr_t ptr) = delete;
    Value(const Value& other);
    Value(Value&& other);
    ~Value();

    Value& operator=(const Value& other);
    Value& operator=(Value&& other);

    /// Swap everything.
    void swap(Value& other);
    /// Swap values but leave comments and source offsets in place.

    /// copy everything.
    void copy(const Value& other);
    /// copy values but leave comments and source offsets in place.
    void copyPayload(const Value& other);

    ValueType type() const;

    std::string asString() const;  ///< Embedded zeroes are possible.
    /** Get raw char* of string-value.
     *  \return false if !string. (Seg-fault if str or end are NULL.)
     */
    bool getString(char const** begin, char const** end) const;

    int asInt() const;
    unsigned int asUInt() const;
    int64_t asInt64() const;
    uint64_t asUInt64() const;
    float asFloat() const;
    double asDouble() const;
    bool asBool() const;

    bool isInt() const;
    bool isInt64() const;
    bool isUInt() const;
    bool isUInt64() const;
    bool isIntegral() const;
    bool isDouble() const;
    bool isNumeric() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;

    /// The `as<T>` and `is<T>` member function templates and specializations.
    template <typename T>
    T as() const = delete;

    template <typename T>
    bool is() const = delete;

    bool isConvertibleTo(ValueType other) const;

    unsigned int size() const;
    bool empty() const;

    explicit operator bool() const;

    void clear();

    void resize(unsigned int newSize);

    const Value& operator[](unsigned int index) const;
    const Value& operator[](int index) const;
    const Value& operator[](const char* key) const;
    const Value& operator[](const std::string& key) const;

    Value& operator[](unsigned int index);
    Value& operator[](int index);
    Value& operator[](const char* key);
    Value& operator[](const std::string& key);
    Value& operator[](const StaticString& key);

    Value const* find(char const* begin, char const* end) const;
    bool isMember(const std::string& key) const;

    CStringArray getMemberNames() const;

    const_iterator begin() const;
    const_iterator end() const;

    iterator begin();
    iterator end();

  private:
    void setType(ValueType v) {
        bits_.value_type_ = static_cast<unsigned char>(v);
    }

    bool isAllocated() const {
        return bits_.allocated_;
    }

    void setIsAllocated(bool v) {
        bits_.allocated_ = v;
    }

    void initBasic(ValueType type, bool allocated = false);
    void dupPayload(const Value& other);
    void releasePayload();

    Value& resolveReference(const char* key);
    Value& resolveReference(const char* key, const char* end);

    union ValueHolder {
        int64_t int_;
        uint64_t uint_;
        double real_;
        bool bool_;
        char* string_;  // if allocated_, ptr to { unsigned, char[] }.
        std::map<CZString, Value>* map_;
    } value_;

    struct {
        // Really a ValueType, but types should agree for bitfield packing.
        unsigned int value_type_ : 8;
        // Unless allocated_, string_ must be null-terminated.
        unsigned int allocated_ : 1;
    } bits_;

    // [start, limit) byte offsets in the source JSON text from which this Value
    // was extracted.
    ptrdiff_t start_;
    ptrdiff_t limit_;

    friend class JsonReader;
};

//--------------------------------------------------------------------------------
template <>
inline bool Value::as<bool>() const {
    return asBool();
}

template <>
inline bool Value::is<bool>() const {
    return type() == boolValType;
}

//--------------------------------------------------------------------------------
template <>
inline int Value::as<int>() const {
    return asInt();
}

template <>
inline bool Value::is<int>() const {
    return isInt();
}

//--------------------------------------------------------------------------------
template <>
inline unsigned int Value::as<unsigned int>() const {
    return asUInt();
}

template <>
inline bool Value::is<unsigned int>() const {
    return isUInt();
}

//--------------------------------------------------------------------------------
template <>
inline int64_t Value::as<int64_t>() const {
    return asInt64();
}

template <>
inline bool Value::is<int64_t>() const {
    return isInt64();
}

//--------------------------------------------------------------------------------
template <>
inline uint64_t Value::as<uint64_t>() const {
    return asUInt64();
}

template <>
inline bool Value::is<uint64_t>() const {
    return isUInt64();
}

//--------------------------------------------------------------------------------
template <>
inline double Value::as<double>() const {
    return asDouble();
}

template <>
inline bool Value::is<double>() const {
    return isDouble();
}

//--------------------------------------------------------------------------------
template <>
inline std::string Value::as<std::string>() const {
    return asString();
}

template <>
inline bool Value::is<std::string>() const {
    return isString();
}

//--------------------------------------------------------------------------------
template <>
inline float Value::as<float>() const {
    return asFloat();
}

//--------------------------------------------------------------------------------
class ValueIteratorBase {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using size_t = unsigned int;
    using difference_type = int;
    using SelfType = ValueIteratorBase;

    bool operator==(const SelfType& other) const {
        return isEqual(other);
    }

    bool operator!=(const SelfType& other) const {
        return !isEqual(other);
    }

    difference_type operator-(const SelfType& other) const {
        return other.computeDistance(*this);
    }

    /// Return either the index or the member name of the referenced value as a
    /// Value.
    Value key() const;

    /// Return the index of the referenced Value, or -1 if it is not an arrayValType.
    unsigned int index() const;

    /// Return the member name of the referenced Value, or "" if it is not an objValType.
    /// \note Avoid `c_str()` on result, as embedded zeroes are possible.
    std::string name() const;

    char const* memberName(char const** end) const;

  protected:
    const Value& deref() const;
    Value& deref();

    void increment();

    void decrement();

    difference_type computeDistance(const SelfType& other) const;

    bool isEqual(const SelfType& other) const;

    void copy(const SelfType& other);

  private:
    std::map<CZString, Value>::iterator current_;
    bool isNull_{true};

  public:
    ValueIteratorBase();
    explicit ValueIteratorBase(const std::map<CZString, Value>::iterator& current);
};

//--------------------------------------------------------------------------------
class ValueConstIterator : public ValueIteratorBase {
    friend class Value;

  public:
    using reference = const Value&;
    using pointer = const Value*;
    using SelfType = ValueConstIterator;

    ValueConstIterator();
    ValueConstIterator(ValueIterator const& other);

  private:
    /*! \internal Use by Value to create an iterator.
     */
    explicit ValueConstIterator(const std::map<CZString, Value>::iterator& current);

  public:
    SelfType& operator=(const ValueIteratorBase& other);

    SelfType operator++(int) {
        SelfType temp(*this);
        ++*this;
        return temp;
    }

    SelfType operator--(int) {
        SelfType temp(*this);
        --*this;
        return temp;
    }

    SelfType& operator--() {
        decrement();
        return *this;
    }

    SelfType& operator++() {
        increment();
        return *this;
    }

    reference operator*() const {
        return deref();
    }

    pointer operator->() const {
        return &deref();
    }
};

//--------------------------------------------------------------------------------
class ValueIterator : public ValueIteratorBase {
    friend class Value;

  public:
    using size_t = unsigned int;
    using difference_type = int;
    using reference = Value&;
    using pointer = Value*;
    using SelfType = ValueIterator;

    ValueIterator();
    explicit ValueIterator(const ValueConstIterator& other);
    ValueIterator(const ValueIterator& other);

  private:
    explicit ValueIterator(const std::map<CZString, Value>::iterator& current);

  public:
    SelfType& operator=(const SelfType& other);

    SelfType operator++(int) {
        SelfType temp(*this);
        ++*this;
        return temp;
    }

    SelfType operator--(int) {
        SelfType temp(*this);
        --*this;
        return temp;
    }

    SelfType& operator--() {
        decrement();
        return *this;
    }

    SelfType& operator++() {
        increment();
        return *this;
    }

    /*! The return value of non-const iterators can be
     *  changed, so the these functions are not const
     *  because the returned references/pointers can be used
     *  to change state of the base class.
     */
    reference operator*() {
        return deref();
    }
    pointer operator->() {
        return &deref();
    }
};

//--------------------------------------------------------------------------------
extern bool decodeString_4(Token& token, Value& decoded);
extern bool decodeNumber_4(Token& token, Value& decoded);
extern bool decodeToken_4(Token& token, Value& decoded);

}  // namespace qblocks

//--------------------------------------------------------------------------------
#define JSON_ASSERT_MESSAGE(condition, message)                                                                        \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            std::ostringstream oss;                                                                                    \
            oss << message;                                                                                            \
            assert(false && oss.str().c_str());                                                                        \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0);

//--------------------------------------------------------------------------------
#define FAIL 1

//--------------------------------------------------------------------------------
#define JSON_ASSERT(condition) assert(condition)
