// Copyright 2007-2011 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "json_value.h"
#include "json_writer.h"
#include "basetypes.h"

namespace qblocks {

//--------------------------------------------------------------------------------
static Value const nullStatic;

//--------------------------------------------------------------------------------
static inline double integerToDouble(uint64_t value) {
    return static_cast<double>(int64_t(value / 2)) * 2.0 + static_cast<double>(int64_t(value & 1));
}

//--------------------------------------------------------------------------------
template <typename T>
static inline double integerToDouble(T value) {
    return static_cast<double>(value);
}

//--------------------------------------------------------------------------------
template <typename T, typename U>
static inline bool InRange(double d, T min, U max) {
    return d >= integerToDouble(min) && d <= integerToDouble(max);
}

//--------------------------------------------------------------------------------
/** Duplicates the specified string value.
 * @param value Pointer to the string to duplicate. Must be zero-terminated if
 *              length is "unknown".
 * @param length Length of the value. if equals to unknown, then it will be
 *               computed using strlen(value).
 * @return Pointer on the duplicate instance of string.
 */
static inline char* duplicateStringValue(const char* value, size_t length) {
    // Avoid an integer overflow in the call to malloc below by limiting length
    // to a sane value.
    if (length >= static_cast<size_t>(Value::maxInt))
        length = Value::maxInt - 1;

    auto newString = static_cast<char*>(malloc(length + 1));
    if (newString == nullptr) {
        throwRuntimeError(
            "in Json::Value::duplicateStringValue(): "
            "Failed to allocate string value buffer");
    }
    memcpy(newString, value, length);
    newString[length] = 0;
    return newString;
}

static inline char* duplicateAndPrefixStringValue(const char* value, unsigned int length) {
    // Avoid an integer overflow in the call to malloc below by limiting length to a sane value.
    JSON_ASSERT_MESSAGE(length <= static_cast<unsigned>(Value::maxInt) - sizeof(unsigned) - 1U,
                        "in Json::Value::duplicateAndPrefixStringValue(): length too big for prefixing");
    size_t actualLength = sizeof(length) + length + 1;
    auto newString = static_cast<char*>(malloc(actualLength));
    if (newString == nullptr) {
        throwRuntimeError("in Json::Value::duplicateAndPrefixStringValue(): Failed to allocate string value buffer");
    }
    *reinterpret_cast<unsigned*>(newString) = length;
    memcpy(newString + sizeof(unsigned), value, length);
    newString[actualLength - 1U] = 0;  // to avoid buffer over-run accidents by users later
    return newString;
}

//--------------------------------------------------------------------------------
inline static void decodePrefixedString(bool isPrefixed, char const* prefixed, unsigned* length, char const** value) {
    if (!isPrefixed) {
        *length = static_cast<unsigned>(strlen(prefixed));
        *value = prefixed;
    } else {
        *length = *reinterpret_cast<unsigned const*>(prefixed);
        *value = prefixed + sizeof(unsigned);
    }
}

//--------------------------------------------------------------------------------
static inline void releasePrefixedStringValue(char* value) {
    free(value);
}

//--------------------------------------------------------------------------------
static inline void releaseStringValue(char* value, unsigned) {
    free(value);
}

//--------------------------------------------------------------------------------
ValueIteratorBase::ValueIteratorBase() : current_() {
}

//--------------------------------------------------------------------------------
ValueIteratorBase::ValueIteratorBase(const std::map<CZString, Value>::iterator& current)
    : current_(current), isNull_(false) {
}

//--------------------------------------------------------------------------------
Value& ValueIteratorBase::deref() {
    return current_->second;
}

//--------------------------------------------------------------------------------
const Value& ValueIteratorBase::deref() const {
    return current_->second;
}

//--------------------------------------------------------------------------------
void ValueIteratorBase::increment() {
    ++current_;
}

//--------------------------------------------------------------------------------
void ValueIteratorBase::decrement() {
    --current_;
}

//--------------------------------------------------------------------------------
ValueIteratorBase::difference_type

//--------------------------------------------------------------------------------
ValueIteratorBase::computeDistance(const SelfType& other) const {
    // Iterator for null value are initialized using the default
    // constructor, which initialize current_ to the default
    // std::map::iterator. As begin() and end() are two instance
    // of the default std::map::iterator, they can not be compared.
    // To allow this, we handle this comparison specifically.
    if (isNull_ && other.isNull_) {
        return 0;
    }

    // Usage of std::distance is not portable (does not compile with Sun Studio 12
    // RogueWave STL,
    // which is the one used by default).
    // Using a portable hand-made version for non random iterator instead:
    //   return difference_type( std::distance( current_, other.current_ ) );
    difference_type myDistance = 0;
    for (std::map<CZString, Value>::iterator it = current_; it != other.current_; ++it) {
        ++myDistance;
    }
    return myDistance;
}

//--------------------------------------------------------------------------------
bool ValueIteratorBase::isEqual(const SelfType& other) const {
    if (isNull_) {
        return other.isNull_;
    }
    return current_ == other.current_;
}

//--------------------------------------------------------------------------------
void ValueIteratorBase::copy(const SelfType& other) {
    current_ = other.current_;
    isNull_ = other.isNull_;
}

//--------------------------------------------------------------------------------
Value ValueIteratorBase::key() const {
    const CZString czstring = (*current_).first;
    if (czstring.data()) {
        if (czstring.isStaticString())
            return Value(StaticString(czstring.data()));
        return Value(czstring.data(), czstring.data() + czstring.length());
    }
    return Value(czstring.index());
}

//--------------------------------------------------------------------------------
unsigned int ValueIteratorBase::index() const {
    const CZString czstring = (*current_).first;
    if (!czstring.data())
        return czstring.index();
    return (unsigned int)(-1);
}

//--------------------------------------------------------------------------------
string_q ValueIteratorBase::name() const {
    char const* keey;
    char const* end;
    keey = memberName(&end);
    if (!keey)
        return string_q();
    return string_q(keey, end);
}

//--------------------------------------------------------------------------------
char const* ValueIteratorBase::memberName(char const** end) const {
    const char* cname = (*current_).first.data();
    if (!cname) {
        *end = nullptr;
        return nullptr;
    }
    *end = cname + (*current_).first.length();
    return cname;
}

//--------------------------------------------------------------------------------
ValueConstIterator::ValueConstIterator() = default;

//--------------------------------------------------------------------------------
ValueConstIterator::ValueConstIterator(const std::map<CZString, Value>::iterator& current)
    : ValueIteratorBase(current) {
}

//--------------------------------------------------------------------------------
ValueConstIterator::ValueConstIterator(ValueIterator const& other) : ValueIteratorBase(other) {
}

//--------------------------------------------------------------------------------
ValueConstIterator& ValueConstIterator::operator=(const ValueIteratorBase& other) {
    copy(other);
    return *this;
}

//--------------------------------------------------------------------------------
ValueIterator::ValueIterator() = default;

//--------------------------------------------------------------------------------
ValueIterator::ValueIterator(const std::map<CZString, Value>::iterator& current) : ValueIteratorBase(current) {
}

//--------------------------------------------------------------------------------
ValueIterator::ValueIterator(const ValueConstIterator& other) : ValueIteratorBase(other) {
    throwRuntimeError("ConstIterator to Iterator should never be allowed.");
}

//--------------------------------------------------------------------------------
ValueIterator::ValueIterator(const ValueIterator& other) = default;

//--------------------------------------------------------------------------------
ValueIterator& ValueIterator::operator=(const SelfType& other) {
    copy(other);
    return *this;
}

//--------------------------------------------------------------------------------
void throwRuntimeError(string_q const& msg) {
    cerr << msg << endl;
    abort();
}

//--------------------------------------------------------------------------------
CZString::CZString(unsigned int index) : cstr_(nullptr), index_(index) {
}

//--------------------------------------------------------------------------------
CZString::CZString(char const* str, unsigned length, DuplicationPolicy allocate) : cstr_(str) {
    storage_.policy_ = allocate & 0x3;
    storage_.length_ = length & 0x3FFFFFFF;
}

//--------------------------------------------------------------------------------
CZString::CZString(const CZString& other) {
    cstr_ = (other.storage_.policy_ != noDuplication && other.cstr_ != nullptr
                 ? duplicateStringValue(other.cstr_, other.storage_.length_)
                 : other.cstr_);
    storage_.policy_ =
        static_cast<unsigned>(
            other.cstr_
                ? (static_cast<DuplicationPolicy>(other.storage_.policy_) == noDuplication ? noDuplication : duplicate)
                : static_cast<DuplicationPolicy>(other.storage_.policy_)) &
        3U;
    storage_.length_ = other.storage_.length_;
}

//--------------------------------------------------------------------------------
CZString::CZString(CZString&& other) : cstr_(other.cstr_), index_(other.index_) {
    other.cstr_ = nullptr;
}

//--------------------------------------------------------------------------------
CZString::~CZString() {
    if (cstr_ && storage_.policy_ == duplicate) {
        releaseStringValue(const_cast<char*>(cstr_), storage_.length_ + 1U);
    }
}

//--------------------------------------------------------------------------------
CZString& CZString::operator=(const CZString& other) {
    cstr_ = other.cstr_;
    index_ = other.index_;
    return *this;
}

//--------------------------------------------------------------------------------
CZString& CZString::operator=(CZString&& other) {
    cstr_ = other.cstr_;
    index_ = other.index_;
    other.cstr_ = nullptr;
    return *this;
}

//--------------------------------------------------------------------------------
bool CZString::operator<(const CZString& other) const {
    if (!cstr_)
        return index_ < other.index_;

    unsigned this_len = this->storage_.length_;
    unsigned other_len = other.storage_.length_;
    unsigned min_len = std::min<unsigned>(this_len, other_len);

    JSON_ASSERT(this->cstr_ && other.cstr_);
    int comp = memcmp(this->cstr_, other.cstr_, min_len);
    if (comp < 0)
        return true;
    if (comp > 0)
        return false;
    return (this_len < other_len);
}

//--------------------------------------------------------------------------------
bool CZString::operator==(const CZString& other) const {
    if (!cstr_)
        return index_ == other.index_;

    unsigned this_len = this->storage_.length_;
    unsigned other_len = other.storage_.length_;
    if (this_len != other_len)
        return false;
    JSON_ASSERT(this->cstr_ && other.cstr_);

    int comp = memcmp(this->cstr_, other.cstr_, this_len);
    return comp == 0;
}

//--------------------------------------------------------------------------------
unsigned int CZString::index() const {
    return index_;
}

//--------------------------------------------------------------------------------
const char* CZString::data() const {
    return cstr_;
}

//--------------------------------------------------------------------------------
unsigned CZString::length() const {
    return storage_.length_;
}

//--------------------------------------------------------------------------------
bool CZString::isStaticString() const {
    return storage_.policy_ == noDuplication;
}

//--------------------------------------------------------------------------------
Value::Value(ValueType type) {
    static char const emptyString[] = "";
    initBasic(type);
    switch (type) {
        case nullValType:
            break;
        case intValType:
        case uintValType:
            value_.int_ = 0;
            break;
        case realValType:
            value_.real_ = 0.0;
            break;
        case strValType:
            // allocated_ == false, so this is safe.
            value_.string_ = const_cast<char*>(static_cast<char const*>(emptyString));
            break;
        case arrayValType:
        case objValType:
            value_.map_ = new std::map<CZString, Value>();
            break;
        case boolValType:
            value_.bool_ = false;
            break;
        default:
            assert(false);
    }
}

//--------------------------------------------------------------------------------
Value::Value(int value) {
    initBasic(intValType);
    value_.int_ = value;
}

//--------------------------------------------------------------------------------
Value::Value(unsigned int value) {
    initBasic(uintValType);
    value_.uint_ = value;
}

//--------------------------------------------------------------------------------
Value::Value(int64_t value) {
    initBasic(intValType);
    value_.int_ = value;
}

//--------------------------------------------------------------------------------
Value::Value(uint64_t value) {
    initBasic(uintValType);
    value_.uint_ = value;
}

//--------------------------------------------------------------------------------
Value::Value(double value) {
    initBasic(realValType);
    value_.real_ = value;
}

//--------------------------------------------------------------------------------
Value::Value(const char* value) {
    initBasic(strValType, true);
    JSON_ASSERT_MESSAGE(value != nullptr, "Null Value Passed to Value Constructor");
    value_.string_ = duplicateAndPrefixStringValue(value, static_cast<unsigned>(strlen(value)));
}

//--------------------------------------------------------------------------------
Value::Value(const char* begin, const char* end) {
    initBasic(strValType, true);
    value_.string_ = duplicateAndPrefixStringValue(begin, static_cast<unsigned>(end - begin));
}

//--------------------------------------------------------------------------------
Value::Value(const string_q& value) {
    initBasic(strValType, true);
    value_.string_ = duplicateAndPrefixStringValue(value.data(), static_cast<unsigned>(value.length()));
}

//--------------------------------------------------------------------------------
Value::Value(const StaticString& value) {
    initBasic(strValType);
    value_.string_ = const_cast<char*>(value.c_str());
}

//--------------------------------------------------------------------------------
Value::Value(bool value) {
    initBasic(boolValType);
    value_.bool_ = value;
}

//--------------------------------------------------------------------------------
Value::Value(const Value& other) {
    dupPayload(other);
    start_ = other.start_;
    limit_ = other.limit_;
}

//--------------------------------------------------------------------------------
Value::Value(Value&& other) {
    initBasic(nullValType);
    swap(other);
}

//--------------------------------------------------------------------------------
Value::~Value() {
    releasePayload();
    value_.uint_ = 0;
}

//--------------------------------------------------------------------------------
Value& Value::operator=(const Value& other) {
    Value(other).swap(*this);
    return *this;
}

//--------------------------------------------------------------------------------
Value& Value::operator=(Value&& other) {
    other.swap(*this);
    return *this;
}

//--------------------------------------------------------------------------------
void Value::copyPayload(const Value& other) {
    releasePayload();
    dupPayload(other);
}

//--------------------------------------------------------------------------------
void Value::swap(Value& other) {
    std::swap(bits_, other.bits_);
    std::swap(value_, other.value_);
    std::swap(start_, other.start_);
    std::swap(limit_, other.limit_);
}

//--------------------------------------------------------------------------------
void Value::copy(const Value& other) {
    copyPayload(other);
    start_ = other.start_;
    limit_ = other.limit_;
}

//--------------------------------------------------------------------------------
ValueType Value::type() const {
    return static_cast<ValueType>(bits_.value_type_);
}

//--------------------------------------------------------------------------------
bool Value::getString(char const** begin, char const** end) const {
    if (type() != strValType)
        return false;
    if (value_.string_ == nullptr)
        return false;
    unsigned length;
    decodePrefixedString(this->isAllocated(), this->value_.string_, &length, begin);
    *end = *begin + length;
    return true;
}

//--------------------------------------------------------------------------------
string_q Value::asString() const {
    switch (type()) {
        case nullValType:
            return "";
        case strValType: {
            if (value_.string_ == nullptr)
                return "";
            unsigned this_len;
            char const* this_str;
            decodePrefixedString(this->isAllocated(), this->value_.string_, &this_len, &this_str);
            return string_q(this_str, this_len);
        }
        case boolValType:
            return value_.bool_ ? "true" : "false";
        case intValType:
            return int_2_Str(value_.int_);
        case uintValType:
            return uint_2_Str(value_.uint_);
        case realValType:
            extern string_q double_2_Str(double f, size_t nDecimals = NOPOS);
            return double_2_Str(value_.real_);
        default:
            break;
    }
    JSON_ASSERT_MESSAGE(FAIL, "Type is not convertible to string");
    return "";
}

//--------------------------------------------------------------------------------
int Value::asInt() const {
    switch (type()) {
        case intValType:
            JSON_ASSERT_MESSAGE(isInt(), "int64_t out of Int range");
            return int(value_.int_);
        case uintValType:
            JSON_ASSERT_MESSAGE(isInt(), "LargestUInt out of Int range");
            return int(value_.uint_);
        case realValType:
            JSON_ASSERT_MESSAGE(InRange(value_.real_, minInt, maxInt), "double out of Int range");
            return int(value_.real_);
        case nullValType:
            return 0;
        case boolValType:
            return value_.bool_ ? 1 : 0;
        default:
            break;
    }
    JSON_ASSERT_MESSAGE(FAIL, "Value is not convertible to Int.");
    return 0;
}

//--------------------------------------------------------------------------------
unsigned int Value::asUInt() const {
    switch (type()) {
        case intValType:
            JSON_ASSERT_MESSAGE(isUInt(), "int64_t out of unsinged int range");
            return (unsigned int)(value_.int_);
        case uintValType:
            JSON_ASSERT_MESSAGE(isUInt(), "LargestUInt out of unsigned int range");
            return (unsigned int)(value_.uint_);
        case realValType:
            JSON_ASSERT_MESSAGE(InRange(value_.real_, 0, maxUInt), "double out of unsigned int range");
            return (unsigned int)(value_.real_);
        case nullValType:
            return 0;
        case boolValType:
            return value_.bool_ ? 1 : 0;
        default:
            break;
    }
    JSON_ASSERT_MESSAGE(FAIL, "Value is not convertible to unsigned int.");
    return 0;
}

//--------------------------------------------------------------------------------
int64_t Value::asInt64() const {
    switch (type()) {
        case intValType:
            return int64_t(value_.int_);
        case uintValType:
            JSON_ASSERT_MESSAGE(isInt64(), "LargestUInt out of int64_t range");
            return int64_t(value_.uint_);
        case realValType:
            JSON_ASSERT_MESSAGE(InRange(value_.real_, minInt64, maxInt64), "double out of int64_t range");
            return int64_t(value_.real_);
        case nullValType:
            return 0;
        case boolValType:
            return value_.bool_ ? 1 : 0;
        default:
            break;
    }
    JSON_ASSERT_MESSAGE(FAIL, "Value is not convertible to int64_t.");
    return 0;
}

//--------------------------------------------------------------------------------
uint64_t Value::asUInt64() const {
    switch (type()) {
        case intValType:
            JSON_ASSERT_MESSAGE(isUInt64(), "int64_t out of UInt64 range");
            return uint64_t(value_.int_);
        case uintValType:
            return uint64_t(value_.uint_);
        case realValType:
            JSON_ASSERT_MESSAGE(InRange(value_.real_, 0, maxUInt64), "double out of UInt64 range");
            return uint64_t(value_.real_);
        case nullValType:
            return 0;
        case boolValType:
            return value_.bool_ ? 1 : 0;
        default:
            break;
    }
    JSON_ASSERT_MESSAGE(FAIL, "Value is not convertible to UInt64.");
    return 0;
}

//--------------------------------------------------------------------------------
double Value::asDouble() const {
    switch (type()) {
        case intValType:
            return static_cast<double>(value_.int_);
        case uintValType:
            return integerToDouble(value_.uint_);
        case realValType:
            return value_.real_;
        case nullValType:
            return 0.0;
        case boolValType:
            return value_.bool_ ? 1.0 : 0.0;
        default:
            break;
    }
    JSON_ASSERT_MESSAGE(FAIL, "Value is not convertible to double.");
    return 0;
}

//--------------------------------------------------------------------------------
float Value::asFloat() const {
    switch (type()) {
        case intValType:
            return static_cast<float>(value_.int_);
        case uintValType:
            // This can fail (silently?) if the value is bigger than MAX_FLOAT.
            return static_cast<float>(integerToDouble(value_.uint_));
        case realValType:
            return static_cast<float>(value_.real_);
        case nullValType:
            return 0.0;
        case boolValType:
            return value_.bool_ ? 1.0F : 0.0F;
        default:
            break;
    }
    JSON_ASSERT_MESSAGE(FAIL, "Value is not convertible to float.");
    return 0;
}

//--------------------------------------------------------------------------------
bool Value::asBool() const {
    switch (type()) {
        case boolValType:
            return value_.bool_;
        case nullValType:
            return false;
        case intValType:
            return value_.int_ != 0;
        case uintValType:
            return value_.uint_ != 0;
        case realValType: {
            // According to JavaScript language zero or NaN is regarded as false
            const auto value_classification = std::fpclassify(value_.real_);
            return value_classification != FP_ZERO && value_classification != FP_NAN;
        }
        default:
            break;
    }
    JSON_ASSERT_MESSAGE(FAIL, "Value is not convertible to bool.");
    return false;
}

//--------------------------------------------------------------------------------
bool Value::isConvertibleTo(ValueType other) const {
    switch (other) {
        case nullValType:
            return (isNumeric() && asDouble() == 0.0) || (type() == boolValType && !value_.bool_) ||
                   (type() == strValType && asString().empty()) || (type() == arrayValType && value_.map_->empty()) ||
                   (type() == objValType && value_.map_->empty()) || type() == nullValType;
        case intValType:
            return isInt() || (type() == realValType && InRange(value_.real_, minInt, maxInt)) ||
                   type() == boolValType || type() == nullValType;
        case uintValType:
            return isUInt() || (type() == realValType && InRange(value_.real_, 0, maxUInt)) || type() == boolValType ||
                   type() == nullValType;
        case realValType:
            return isNumeric() || type() == boolValType || type() == nullValType;
        case boolValType:
            return isNumeric() || type() == boolValType || type() == nullValType;
        case strValType:
            return isNumeric() || type() == boolValType || type() == strValType || type() == nullValType;
        case arrayValType:
            return type() == arrayValType || type() == nullValType;
        case objValType:
            return type() == objValType || type() == nullValType;
    }
    assert(false);
    return false;
}

//--------------------------------------------------------------------------------
unsigned int Value::size() const {
    switch (type()) {
        case nullValType:
        case intValType:
        case uintValType:
        case realValType:
        case boolValType:
        case strValType:
            return 0;
        case arrayValType:  // size of the array is highest index + 1
            if (!value_.map_->empty()) {
                std::map<CZString, Value>::const_iterator itLast = value_.map_->end();
                --itLast;
                return (*itLast).first.index() + 1;
            }
            return 0;
        case objValType:
            return (unsigned int)(value_.map_->size());
    }
    assert(false);
    return 0;  // unreachable;
}

//--------------------------------------------------------------------------------
bool Value::empty() const {
    if ((type() == nullValType) || isArray() || isObject())
        return size() == 0U;
    return false;
}

//--------------------------------------------------------------------------------
Value::operator bool() const {
    return !(type() == nullValType);
}

//--------------------------------------------------------------------------------
void Value::clear() {
    JSON_ASSERT_MESSAGE(type() == nullValType || type() == arrayValType || type() == objValType,
                        "in Json::Value::clear(): requires complex value");
    start_ = 0;
    limit_ = 0;
    switch (type()) {
        case arrayValType:
        case objValType:
            value_.map_->clear();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------------------------
void Value::resize(unsigned int newSize) {
    JSON_ASSERT_MESSAGE(type() == nullValType || type() == arrayValType,
                        "in Json::Value::resize(): requires arrayValType");
    if (type() == nullValType)
        *this = Value(arrayValType);
    unsigned int oldSize = size();
    if (newSize == 0)
        clear();
    else if (newSize > oldSize)
        this->operator[](newSize - 1);
    else {
        for (unsigned int index = newSize; index < oldSize; ++index) {
            value_.map_->erase(index);
        }
        JSON_ASSERT(size() == newSize);
    }
}

//--------------------------------------------------------------------------------
Value& Value::operator[](unsigned int index) {
    JSON_ASSERT_MESSAGE(type() == nullValType || type() == arrayValType,
                        "in Json::Value::operator[](unsigned int): requires arrayValType");
    if (type() == nullValType)
        *this = Value(arrayValType);
    CZString key(index);
    auto it = value_.map_->lower_bound(key);
    if (it != value_.map_->end() && (*it).first == key)
        return (*it).second;

    std::map<CZString, Value>::value_type defaultValue(key, nullStatic);
    it = value_.map_->insert(it, defaultValue);
    return (*it).second;
}

//--------------------------------------------------------------------------------
Value& Value::operator[](int index) {
    JSON_ASSERT_MESSAGE(index >= 0, "in Json::Value::operator[](int index): index cannot be negative");
    return (*this)[(unsigned int)(index)];
}

//--------------------------------------------------------------------------------
const Value& Value::operator[](unsigned int index) const {
    JSON_ASSERT_MESSAGE(type() == nullValType || type() == arrayValType,
                        "in Json::Value::operator[](unsigned int)const: requires arrayValType");
    if (type() == nullValType)
        return nullStatic;
    CZString key(index);
    std::map<CZString, Value>::const_iterator it = value_.map_->find(key);
    if (it == value_.map_->end())
        return nullStatic;
    return (*it).second;
}

//--------------------------------------------------------------------------------
const Value& Value::operator[](int index) const {
    JSON_ASSERT_MESSAGE(index >= 0, "in Json::Value::operator[](int index) const: index cannot be negative");
    return (*this)[(unsigned int)(index)];
}

//--------------------------------------------------------------------------------
void Value::initBasic(ValueType type, bool allocated) {
    setType(type);
    setIsAllocated(allocated);
    start_ = 0;
    limit_ = 0;
}

//--------------------------------------------------------------------------------
void Value::dupPayload(const Value& other) {
    setType(other.type());
    setIsAllocated(false);
    switch (type()) {
        case nullValType:
        case intValType:
        case uintValType:
        case realValType:
        case boolValType:
            value_ = other.value_;
            break;
        case strValType:
            if (other.value_.string_ && other.isAllocated()) {
                unsigned len;
                char const* str;
                decodePrefixedString(other.isAllocated(), other.value_.string_, &len, &str);
                value_.string_ = duplicateAndPrefixStringValue(str, len);
                setIsAllocated(true);
            } else {
                value_.string_ = other.value_.string_;
            }
            break;
        case arrayValType:
        case objValType:
            value_.map_ = new std::map<CZString, Value>(*other.value_.map_);
            break;
        default:
            assert(false);
    }
}

//--------------------------------------------------------------------------------
void Value::releasePayload() {
    switch (type()) {
        case nullValType:
        case intValType:
        case uintValType:
        case realValType:
        case boolValType:
            break;
        case strValType:
            if (isAllocated())
                releasePrefixedStringValue(value_.string_);
            break;
        case arrayValType:
        case objValType:
            delete value_.map_;
            break;
        default:
            assert(false);
    }
}

//--------------------------------------------------------------------------------
Value& Value::resolveReference(const char* key) {
    JSON_ASSERT_MESSAGE(type() == nullValType || type() == objValType,
                        "in Json::Value::resolveReference(): requires objValType");
    if (type() == nullValType)
        *this = Value(objValType);

    CZString actualKey(key, static_cast<unsigned>(strlen(key)), CZString::noDuplication);  // NOTE!
    auto it = value_.map_->lower_bound(actualKey);
    if (it != value_.map_->end() && (*it).first == actualKey)
        return (*it).second;

    std::map<CZString, Value>::value_type defaultValue(actualKey, nullStatic);
    it = value_.map_->insert(it, defaultValue);
    Value& value = (*it).second;
    return value;
}

//--------------------------------------------------------------------------------
Value& Value::resolveReference(char const* key, char const* end) {
    JSON_ASSERT_MESSAGE(type() == nullValType || type() == objValType,
                        "in Json::Value::resolveReference(key, end): requires objValType");
    if (type() == nullValType)
        *this = Value(objValType);
    CZString actualKey(key, static_cast<unsigned>(end - key), CZString::duplicateOnCopy);
    auto it = value_.map_->lower_bound(actualKey);
    if (it != value_.map_->end() && (*it).first == actualKey)
        return (*it).second;

    std::map<CZString, Value>::value_type defaultValue(actualKey, nullStatic);
    it = value_.map_->insert(it, defaultValue);
    Value& value = (*it).second;
    return value;
}

//--------------------------------------------------------------------------------
Value const* Value::find(char const* begin, char const* end) const {
    JSON_ASSERT_MESSAGE(type() == nullValType || type() == objValType,
                        "in Json::Value::find(begin, end): requires objValType or nullValType");

    if (type() == nullValType)
        return nullptr;

    CZString actualKey(begin, static_cast<unsigned>(end - begin), CZString::noDuplication);
    std::map<CZString, Value>::const_iterator it = value_.map_->find(actualKey);
    if (it == value_.map_->end())
        return nullptr;

    return &(*it).second;
}

//--------------------------------------------------------------------------------
const Value& Value::operator[](const char* key) const {
    Value const* found = find(key, key + strlen(key));
    if (!found)
        return nullStatic;
    return *found;
}

//--------------------------------------------------------------------------------
Value const& Value::operator[](const string_q& key) const {
    Value const* found = find(key.data(), key.data() + key.length());
    if (!found)
        return nullStatic;
    return *found;
}

//--------------------------------------------------------------------------------
Value& Value::operator[](const char* key) {
    return resolveReference(key, key + strlen(key));
}

//--------------------------------------------------------------------------------
Value& Value::operator[](const string_q& key) {
    return resolveReference(key.data(), key.data() + key.length());
}

//--------------------------------------------------------------------------------
Value& Value::operator[](const StaticString& key) {
    return resolveReference(key.c_str());
}

//--------------------------------------------------------------------------------
bool Value::isMember(string_q const& key) const {
    return find(key.data(), key.data() + key.length()) != nullptr;
}

//--------------------------------------------------------------------------------
CStringArray Value::getMemberNames() const {
    JSON_ASSERT_MESSAGE(type() == nullValType || type() == objValType,
                        "in Json::Value::getMemberNames(), value must be objValType");

    CStringArray members;
    if (type() != nullValType) {
        members.reserve(value_.map_->size());
        std::map<CZString, Value>::const_iterator it = value_.map_->begin();
        std::map<CZString, Value>::const_iterator itEnd = value_.map_->end();
        for (; it != itEnd; ++it) {
            members.push_back(string_q((*it).first.data(), (*it).first.length()));
        }
    }
    return members;
}

//--------------------------------------------------------------------------------
static bool IsIntegral(double d) {
    double integral_part;
    return modf(d, &integral_part) == 0.0;
}

//--------------------------------------------------------------------------------
bool Value::isInt() const {
    switch (type()) {
        case intValType:
            return value_.int_ >= minInt && value_.int_ <= maxInt;
        case uintValType:
            return value_.uint_ <= (unsigned int)(maxInt);
        case realValType:
            return value_.real_ >= minInt && value_.real_ <= maxInt && IsIntegral(value_.real_);
        default:
            break;
    }
    return false;
}

//--------------------------------------------------------------------------------
bool Value::isUInt() const {
    switch (type()) {
        case intValType:
            return value_.int_ >= 0 && uint64_t(value_.int_) <= uint64_t(maxUInt);
        case uintValType:
            return value_.uint_ <= maxUInt;
        case realValType:
            return value_.real_ >= 0 && value_.real_ <= maxUInt && IsIntegral(value_.real_);
        default:
            break;
    }
    return false;
}

//--------------------------------------------------------------------------------
bool Value::isInt64() const {
    switch (type()) {
        case intValType:
            return true;
        case uintValType:
            return value_.uint_ <= uint64_t(maxInt64);
        case realValType:
            // Note that maxInt64 (= 2^63 - 1) is not exactly representable as a
            // double, so double(maxInt64) will be rounded up to 2^63. Therefore we
            // require the value to be strictly less than the limit.
            return value_.real_ >= double(minInt64) && value_.real_ < double(maxInt64) && IsIntegral(value_.real_);
        default:
            break;
    }
    return false;
}

//--------------------------------------------------------------------------------
bool Value::isUInt64() const {
    switch (type()) {
        case intValType:
            return value_.int_ >= 0;
        case uintValType:
            return true;
        case realValType:
            // Note that maxUInt64 (= 2^64 - 1) is not exactly representable as a
            // double, so double(maxUInt64) will be rounded up to 2^64. Therefore we
            // require the value to be strictly less than the limit.
            return value_.real_ >= 0 && value_.real_ < maxUInt64AsDouble && IsIntegral(value_.real_);
        default:
            break;
    }
    return false;
}

//--------------------------------------------------------------------------------
bool Value::isIntegral() const {
    switch (type()) {
        case intValType:
        case uintValType:
            return true;
        case realValType:
            // Note that maxUInt64 (= 2^64 - 1) is not exactly representable as a
            // double, so double(maxUInt64) will be rounded up to 2^64. Therefore we
            // require the value to be strictly less than the limit.
            return (value_.real_ >= double(minInt64)) && (value_.real_ < maxUInt64AsDouble) && IsIntegral(value_.real_);
        default:
            break;
    }
    return false;
}

//--------------------------------------------------------------------------------
bool Value::isDouble() const {
    return type() == intValType || type() == uintValType || type() == realValType;
}

//--------------------------------------------------------------------------------
bool Value::isNumeric() const {
    return isDouble();
}

//--------------------------------------------------------------------------------
bool Value::isString() const {
    return type() == strValType;
}

//--------------------------------------------------------------------------------
bool Value::isArray() const {
    return type() == arrayValType;
}

//--------------------------------------------------------------------------------
bool Value::isObject() const {
    return type() == objValType;
}

//--------------------------------------------------------------------------------
Value::const_iterator Value::begin() const {
    switch (type()) {
        case arrayValType:
        case objValType:
            if (value_.map_)
                return const_iterator(value_.map_->begin());
            break;
        default:
            break;
    }
    return {};
}

//--------------------------------------------------------------------------------
Value::const_iterator Value::end() const {
    switch (type()) {
        case arrayValType:
        case objValType:
            if (value_.map_)
                return const_iterator(value_.map_->end());
            break;
        default:
            break;
    }
    return {};
}

//--------------------------------------------------------------------------------
Value::iterator Value::begin() {
    switch (type()) {
        case arrayValType:
        case objValType:
            if (value_.map_)
                return iterator(value_.map_->begin());
            break;
        default:
            break;
    }
    return iterator();
}

//--------------------------------------------------------------------------------
Value::iterator Value::end() {
    switch (type()) {
        case arrayValType:
        case objValType:
            if (value_.map_)
                return iterator(value_.map_->end());
            break;
        default:
            break;
    }
    return iterator();
}

}  // namespace qblocks
