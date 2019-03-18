/*
 This file is part of cpp-ethereum.
 cpp-ethereum is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 cpp-ethereum is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
 */
/** @file RLP.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 *
 * RLP (de-)serialisation.
 */

#pragma once

#include <vector>
#include <array>
#include <exception>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <type_traits>
#include <cstring>
#include <string>
#include <array>
#include <cstdint>
#include <algorithm>
#include <random>

#include "etherlib.h"

namespace qblocks
{

    template <class _T>
    class vector_ref {
    public:
        using value_type = _T;
        using element_type = _T;
        using mutable_value_type = typename std::conditional<std::is_const<_T>::value, typename std::remove_const<_T>::type, _T>::type;

        static_assert(std::is_pod<value_type>::value, "vector_ref can only be used with PODs due to its low-level treatment of data.");

        vector_ref()
        : m_data(nullptr), m_count(0) {
        }

        /// Creates a new vector_ref to point to @a _count elements starting at @a _data.
        vector_ref(_T* _data, size_t _count)
        : m_data(_data), m_count(_count) {
        }

        /// Creates a new vector_ref pointing to the data part of a string (given as pointer).
        vector_ref(typename std::conditional<std::is_const<_T>::value, std::string const*, std::string*>::type _data)
        : m_data(reinterpret_cast<_T*>(_data->data())), m_count(_data->size() / sizeof(_T)) {
        }

        /// Creates a new vector_ref pointing to the data part of a vector (given as pointer).
        vector_ref(typename std::conditional<std::is_const<_T>::value, std::vector<typename std::remove_const<_T>::type> const*, std::vector<_T>*>::type _data)
        : m_data(_data->data()), m_count(_data->size()) {
        }

        /// Creates a new vector_ref pointing to the data part of a string (given as reference).
        vector_ref(typename std::conditional<std::is_const<_T>::value, std::string const&, std::string&>::type _data)
        : m_data(reinterpret_cast<_T*>(_data.data())), m_count(_data.size() / sizeof(_T)) {
        }

        explicit operator bool() const {
            return m_data && m_count;
        }

        bool contentsEqual(std::vector<mutable_value_type> const& _c) const {
            if (!m_data || m_count == 0)
                return _c.empty();
            else
                return _c.size() == m_count && !memcmp(_c.data(), m_data, m_count * sizeof(_T));
        }

        std::vector<mutable_value_type> toVector() const {
            return std::vector<mutable_value_type>(m_data, m_data + m_count);
        }

        std::vector<unsigned char> toBytes() const {
            return std::vector<unsigned char>(reinterpret_cast<unsigned char const*>(m_data), reinterpret_cast<unsigned char const*>(m_data) + m_count * sizeof(_T));
        }

        std::string toString() const {
            return std::string((char const*)m_data, ((char const*)m_data) + m_count * sizeof(_T));
        }

        template <class _T2> explicit operator vector_ref<_T2>() const {
            assert(m_count * sizeof(_T) / sizeof(_T2) * sizeof(_T2) / sizeof(_T) == m_count);
            return vector_ref<_T2>(reinterpret_cast<_T2*>(m_data), m_count * sizeof(_T) / sizeof(_T2));
        }

        operator vector_ref<_T const>() const {
            return vector_ref<_T const>(m_data, m_count);
        }

        /// @returns a new vector_ref pointing at the next chunk of @a size() elements.
        vector_ref<_T> next() const {
            if (!m_data)
                return *this;
            else
                return vector_ref<_T>(m_data + m_count, m_count);
        }

        /// @returns a new vector_ref which is a shifted and shortened view of the original data.
        /// If this goes out of bounds in any way, returns an empty vector_ref.
        /// If @a _count is ~size_t(0), extends the view to the end of the data.
        vector_ref<_T> cropped(size_t _begin, size_t _count) const {

            if (m_data &&
                _begin <= m_count &&
                _count <= m_count &&
                _begin + _count <= m_count) {
                return vector_ref<_T>(m_data + _begin, _count == ~size_t(0) ? m_count - _begin : _count);
            } else {
                return vector_ref<_T>();
            }
        }

        /// @returns a new vector_ref which is a shifted view of the original data (not going beyond it).
        vector_ref<_T> cropped(size_t _begin) const {
            if (m_data && _begin <= m_count) {
                return vector_ref<_T>(m_data + _begin, m_count - _begin);
            } else {
                return vector_ref<_T>();
            }
        }

        void retarget(_T* _d, size_t _s) {
            m_data = _d;
            m_count = _s;
        }

        _T const *begin(void) const {
            return m_data;
        }

        _T const *end(void) const {
            return m_data + m_count;
        }

        _T const& operator[](size_t _i) const {
            assert(m_data);
            assert(_i < m_count);
            return m_data[_i];
        }

        bool operator==(vector_ref<_T> const& _cmp) const {
            return m_data == _cmp.m_data && m_count == _cmp.m_count;
        }

    public:
        _T* m_data;
        size_t m_count;
    };

    template<class _T> vector_ref<_T const> ref(_T const& _t)              { return vector_ref<_T const>(&_t, 1); }
    template<class _T> vector_ref<_T>       ref(_T& _t)                    { return vector_ref<_T>(&_t, 1); }
    template<class _T> vector_ref<_T const> ref(std::vector<_T> const& _t) { return vector_ref<_T const>(&_t); }
    template<class _T> vector_ref<_T>       ref(std::vector<_T>& _t)       { return vector_ref<_T>(&_t); }

    using byte = uint8_t;
    using bytes = vector<byte>;
    using bytesRef = vector_ref<byte>;

    // Numeric types.
    using bigint = bigint_t;
    using u64 = uint64_t;
    using u128 = bigint_t;
    using u256 = bigint_t;
    using u160 = bigint_t;
    using u512 = bigint_t;
    static const u256 Invalid256 = (biguint_t)1; //~(u256)0;

    enum class HexPrefix
    {
        DontAdd = 0,
        Add = 1,
    };

    /// Convert a series of bytes to the corresponding string of hex duplets.
    /// @param _w specifies the width of the first of the elements. Defaults to two - enough to represent a byte.
    /// @example toHex("A\x69") == "4169"
    template <class T>
    std::string toHex(T const& _data, int _w = 2, HexPrefix _prefix = HexPrefix::DontAdd) {
        std::ostringstream ret;
        unsigned ii = 0;
        for (auto i: _data)
            ret << std::hex << std::setfill('0') << std::setw(ii++ ? 2 : _w) << (int)(typename std::make_unsigned<decltype(i)>::type)i;
        return (_prefix == HexPrefix::Add) ? "0x" + ret.str() : ret.str();
    }

    /// Converts a (printable) ASCII hex character into the correspnding integer value.
    /// @example fromHex('A') == 10 && fromHex('f') == 15 && fromHex('5') == 5
    int fromHex(char _i);

    /// Converts a (printable) ASCII hex string into the corresponding byte stream.
    /// @example fromHex("41626261") == asBytes("Abba")
    /// If _throw = ThrowType::DontThrow, it replaces bad hex characters with 0's, otherwise it will throw an exception.
    bytes fromHex(std::string const& _s);

    /// Converts byte array to a string containing the same (binary) data. Unless
    /// the byte array happens to contain ASCII data, this won't be printable.
    inline std::string asString(bytes const& _b) {
        return std::string((char const*)_b.data(), (char const*)(_b.data() + _b.size()));
    }

    /// Converts a string to a byte array containing the string's (byte) data.
    inline bytes asBytes(std::string const& _b) {
        return bytes((byte const*)_b.data(), (byte const*)(_b.data() + _b.size()));
    }

    // Big-endian to/from host endian conversion functions.

    /// Converts a templated integer value to the big-endian byte-stream represented on a templated collection.
    /// The size of the collection object will be unchanged. If it is too small, it will not represent the
    /// value properly, if too big then the additional elements will be zeroed out.
    /// @a Out will typically be either std::string or bytes.
    /// @a T will typically by unsigned, u160, u256 or bigint.
    template <class T, class Out>
    inline void toBigEndian(T _val, Out& o_out) {
        //        static_assert(std::is_same<bigint, T>::value || !std::numeric_limits<T>::is_signed, "only unsigned types or bigint supported"); //bigint does not carry sign bit on shift
        //        for (auto i = o_out.size(); i != 0; _val >>= 8, i--)
        //        {
        //            T v = _val & (T)0xff;
        //            o_out[i - 1] = (typename Out::value_type)(uint8_t)v;
        //        }
    }

    /// Converts a big-endian byte-stream represented on a templated collection to a templated integer value.
    /// @a _In will typically be either std::string or bytes.
    /// @a T will typically by unsigned, u160, u256 or bigint.
    template <class T, class _In>
    inline T fromBigEndian(_In const& _bytes) {
        T ret = (T)0;
        //        for (auto i: _bytes)
        //            ret = (T)((ret << 8) | (byte)(typename std::make_unsigned<typename _In::value_type>::type)i);
        return ret;
    }

    /// Convenience functions for toBigEndian
    inline bytes toBigEndian(u256 _val) { bytes ret(32); toBigEndian(_val, ret); return ret; }
    //    inline bytes toBigEndian(u160 _val) { bytes ret(20); toBigEndian(_val, ret); return ret; }

    /// Convenience function for toBigEndian.
    /// @returns a byte array just big enough to represent @a _val.
    template <class T>
    inline bytes toCompactBigEndian(T _val, unsigned _min = 0)
    {
        static_assert(std::is_same<bigint, T>::value || !std::numeric_limits<T>::is_signed, "only unsigned types or bigint supported"); //bigint does not carry sign bit on shift
        int i = 0;
        for (T v = _val; v; ++i, v >>= 8) {}
        bytes ret(std::max<unsigned>(_min, (const unsigned int)i), 0);
        toBigEndian(_val, ret);
        return ret;
    }

    /// Convenience function for conversion of a u256 to hex
    inline std::string toHex(u256 val, HexPrefix prefix = HexPrefix::DontAdd) {
        std::string str = toHex(toBigEndian(val));
        return (prefix == HexPrefix::Add) ? "0x" + str : str;
    }

    inline std::string toHex(uint64_t _n) {
        std::ostringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(sizeof(_n) * 2) << _n;
        return ss.str();
    }

    // Algorithms for string and string-like collections.

    /// Escapes a string into the C-string representation.
    /// @p _all if true will escape all characters, not just the unprintable ones.
    //    string_q escaped(const string_q& _s, bool _all);

    // General datatype convenience functions.

    /// Determine bytes required to encode the given integer value. @returns 0 if @a _i is zero.
    template <class T>
    inline unsigned bytesRequired(T _i)
    {
        static_assert(std::is_same<bigint, T>::value || !std::numeric_limits<T>::is_signed, "only unsigned types or bigint supported"); //bigint does not carry sign bit on shift
        unsigned i = 0;
        for (; _i != 0; ++i, _i >>= 8) {}
        return i;
    }

    extern std::random_device s_fixedHashEngine;

    using FixedHash = biguint_t;

    using h2048 = biguint_t; // FixedHash<256>;
    using h1024 = biguint_t; // FixedHash<128>;
    using h520 = biguint_t; // FixedHash<65>;
    using h512 = biguint_t; // FixedHash<64>;
    using h256 = biguint_t; // FixedHash<32>;
    using h160 = biguint_t; // FixedHash<20>;
    using h128 = biguint_t; // FixedHash<16>;
    using h64 = biguint_t; // FixedHash<8>;
    using h512s = std::vector<h512>;
    using h256s = std::vector<h256>;
    using h160s = std::vector<h160>;

    inline std::string toString(h256s const& _bs) {
        std::ostringstream out;
        out << "[ ";
        for (auto i: _bs)
            out << i << ", ";
        //            out << i.abridged() << ", ";
        out << "]";
        return out.str();
    }

    class RLP;
    using RLPs = std::vector<RLP>;

    template <class _T> struct intTraits { static const unsigned maxSize = sizeof(_T); };
    template <> struct intTraits<u160> { static const unsigned maxSize = 20; };

    static const byte c_rlpMaxLengthBytes = 8;
    static const byte c_rlpDataImmLenStart = 0x80;
    static const byte c_rlpListStart = 0xc0;

    static const byte c_rlpDataImmLenCount = c_rlpListStart - c_rlpDataImmLenStart - c_rlpMaxLengthBytes;
    static const byte c_rlpDataIndLenZero = c_rlpDataImmLenStart + c_rlpDataImmLenCount - 1;
    static const byte c_rlpListImmLenCount = 256 - c_rlpListStart - c_rlpMaxLengthBytes;
    static const byte c_rlpListIndLenZero = c_rlpListStart + c_rlpListImmLenCount - 1;

    class RLP {
    public:
        /// Construct a null node.
        RLP() {}

        /// Construct a node of value given in the bytes.
        explicit RLP(vector_ref<byte const> _d);

        /// Construct a node of value given in the bytes.
        explicit RLP(bytes const& _d): RLP(&_d) {
        }

        /// Construct a node to read RLP data in the bytes given.
        RLP(byte const* _b, unsigned _s): RLP(vector_ref<byte const>(_b, _s)) {
        }

        /// Construct a node to read RLP data in the string.
        explicit RLP(const string_q& _s): RLP(vector_ref<byte const>((byte const*)_s.data(), _s.size())) {
        }

        /// The bare data of the RLP.
        vector_ref<byte const> data() const { return m_data; }

        /// @returns true if the RLP is non-null.
        explicit operator bool() const { return !isNull(); }

        /// No value.
        bool isNull() const { return m_data.m_count == 0; }

        /// String value.
        bool isData() const { return !isNull() && m_data[0] < c_rlpListStart; }

        /// List value.
        bool isList() const { return !isNull() && m_data[0] >= c_rlpListStart; }

        /// Integer value. Must not have a leading zero.
        bool isInt() const;

        /// @returns the number of bytes in the data, or zero if it isn't data.
        size_t size() const { return isData() ? length() : 0; }

        /// Equality operators; does best-effort conversion and checks for equality.
        bool operator==(char const* _s) const { return isData() && toString() == _s; }
        bool operator!=(char const* _s) const { return isData() && toString() != _s; }
        bool operator==(std::string const& _s) const { return isData() && toString() == _s; }
        bool operator!=(std::string const& _s) const { return isData() && toString() != _s; }
//        template <unsigned _N> bool operator==(FixedHash<_N> const& _h) const { return isData() && toHash<_N>() == _h; }
//        template <unsigned _N> bool operator!=(FixedHash<_N> const& _s) const { return isData() && toHash<_N>() != _s; }
        bool operator==(unsigned const& _i) const { return isInt() && toInt<unsigned>() == _i; }
        bool operator!=(unsigned const& _i) const { return isInt() && toInt<unsigned>() != _i; }
        bool operator==(u256 const& _i) const { return isInt() && toInt<u256>() == _i; }
        bool operator!=(u256 const& _i) const { return isInt() && toInt<u256>() != _i; }
//        bool operator==(bigint const& _i) const { return isInt() && toInt<bigint>() == _i; }
//        bool operator!=(bigint const& _i) const { return isInt() && toInt<bigint>() != _i; }

        /// Subscript operator.
        /// @returns the list item @a _i if isList() and @a _i < listItems(), or RLP() otherwise.
        /// @note if used to access items in ascending order, this is efficient.
        RLP operator[](size_t _i) const;

        /// @brief Iterator class for iterating through items of RLP list.
        class iterator {
            friend class RLP;

        public:
            iterator& operator++();
            iterator operator++(int) { auto ret = *this; operator++(); return ret; }
            RLP operator*() const { return RLP(m_currentItem); }
            bool operator==(iterator const& _cmp) const { return m_currentItem == _cmp.m_currentItem; }
            bool operator!=(iterator const& _cmp) const { return !operator==(_cmp); }

        private:
            iterator() {}
            iterator(RLP const& _parent, bool _begin);
            size_t m_remaining = 0;
            vector_ref<byte const> m_currentItem;
        };

        /// @brief Iterator into beginning of sub-item list (valid only if we are a list).
        iterator begin() const { return iterator(*this, true); }

        /// @brief Iterator into end of sub-item list (valid only if we are a list).
        iterator end() const { return iterator(*this, false); }

        /// Best-effort conversion operators.
        explicit operator std::string() const { return toString(); }
        explicit operator bytes() const { return toBytes(); }
        explicit operator RLPs() const {
            RLPs ret;
            if (!isList())
                return ret;
            for (auto i: *this)
                ret.push_back(i);
            return ret;
        }
        explicit operator uint8_t() const { return toInt<uint8_t>(); }
        explicit operator uint16_t() const { return toInt<uint16_t>(); }
        explicit operator uint32_t() const { return toInt<uint32_t>(); }
        explicit operator uint64_t() const { return toInt<uint64_t>(); }
        explicit operator u160() const { return toInt<u160>(); }
//        explicit operator u256() const { return toInt<u256>(); }
//        explicit operator bigint() const { return toInt<bigint>(); }
//        template <unsigned _N> explicit operator FixedHash<_N>() const { return toHash<FixedHash<_N>>(); }
        template <class T, class U> explicit operator std::pair<T, U>() const { return toPair<T, U>(); }
//        template <class T> explicit operator std::vector<T>() const { return toVector<T>(); }
        template <class T, size_t N> explicit operator std::array<T, N>() const { return toArray<T, N>(); }

        /// Converts to bytearray. @returns the empty byte array if not a string.
        bytes toBytes() const { if (!isData()) return bytes(); return bytes(payload().m_data, payload().m_data + length()); }
        /// Converts to string. @returns the empty string if not a string.
        std::string toString() const { if (!isData()) return std::string(); return payload().cropped(0, length()).toString(); }

//        template <class T>
//        std::vector<T> toVector() const {
//            std::vector<T> ret;
//            if (isList()) {
//                ret.reserve(nItems());
//                for (auto i: *this)
//                    ret.push_back((T)i);
//            }
//            return ret;
//        }

#define BOOST_THROW_EXCEPTION(a) { cerr << "error"; exit(0); }
        template <class T, class U>
        std::pair<T, U> toPair() const {
            if (nItems() != 2)
                BOOST_THROW_EXCEPTION(BadCast());
            std::pair<T, U> ret;
            ret.first = (T)(*this)[0];
            ret.second = (U)(*this)[1];
            return ret;
        }

        template <class T, size_t N>
        std::array<T, N> toArray() const {
            if (nItems() != N)
                BOOST_THROW_EXCEPTION(BadCast());
            std::array<T, N> ret;
            for (size_t i = 0; i < N; ++i)
                ret[i] = (T)operator[](i);
            return ret;
        }

        /// Converts to int of type given; if isString(), decodes as big-endian bytestream. @returns 0 if not an int or string.
        template <class _T>
        _T toInt(void) const {
            if ((!isInt()) || isList() || isNull()) {
                return 0;
            }
            auto p = payload();
            if (p.m_count > intTraits<_T>::maxSize) {
                return 0;
            }
            return fromBigEndian<_T>(p);
        }

        template <class _N>
        _N toHash(void) const {
            auto p = payload();
            auto l = p.m_count;
            if (!isData() || (l > _N::size) || (l < _N::size)) {
                return _N();
            }
            _N ret;
            size_t s = std::min<size_t>(_N::size, l);
            memcpy(ret.m_data + _N::size - s, p.m_data, s);
            return ret;
        }

        /// @returns the data payload. Valid for all types.
        vector_ref<byte const> payload() const { auto l = length(); if (l > m_data.m_count) BOOST_THROW_EXCEPTION(BadRLP()); return m_data.cropped(payloadOffset(), l); }

        /// @returns the theoretical size of this item as encoded in the data.
        /// @note Under normal circumstances, is equivalent to m_data.m_count - use that unless you know it won't work.
        size_t actualSize() const {
            if (isNull())
                return 0;
            if (isSingleByte())
                return 1;
            if (isData() || isList())
                return payloadOffset() + length();
            return 0;
        }


    private:
        /// Disable construction from rvalue
        explicit RLP(bytes const&&) {
        }

        /// Single-byte data payload.
        bool isSingleByte() const {
            return (!isNull() && m_data[0] < c_rlpDataImmLenStart);
        }

        /// @returns the amount of bytes used to encode the length of the data. Valid for all types.
        unsigned lengthSize() const {
            if (isData() && m_data[0] > c_rlpDataIndLenZero)
                return m_data[0] - c_rlpDataIndLenZero;
            if (isList() && m_data[0] > c_rlpListIndLenZero)
                return m_data[0] - c_rlpListIndLenZero;
            return 0;
        }

        /// @returns the size in bytes of the payload, as given by the RLP as opposed to as inferred from m_data.
        size_t length() const;

        /// @returns the number of bytes into the data that the payload starts.
        size_t payloadOffset() const {
            return isSingleByte() ?
            0 :
            (1 + lengthSize());
        }

        /// @returns the number of data items.
        size_t nItems() const {
            if (isList()) {
                vector_ref<byte const> d = payload();
                size_t i = 0;
                for (; d.m_count; ++i)
                    d = d.cropped(sizeAsEncoded(d));
                return i;
            }
            return 0;
        }

        /// @returns the size encoded into the RLP in @a _data and throws if _data is too short.
        static size_t sizeAsEncoded(vector_ref<byte const> _data) { return RLP(_data).actualSize(); }

        /// Our byte data.
        vector_ref<byte const> m_data;

        /// The list-indexing cache.
        mutable size_t m_lastIndex = (size_t)-1;
        mutable size_t m_lastEnd = 0;
        mutable vector_ref<byte const> m_lastItem;
    };

    /**
     * @brief Class for writing to an RLP bytestream.
     */
    class RLPStream
    {
    public:
        /// Initializes empty RLPStream.
        RLPStream() {}

        /// Initializes the RLPStream as a list of @a _listItems items.
        explicit RLPStream(size_t _listItems) { appendList(_listItems); }

        ~RLPStream() {}

        /// Append given datum to the byte stream.
        RLPStream& append(unsigned _s) { return append(bigint(_s)); }
//        RLPStream& append(u160 _s) { return append(bigint(_s)); }
//        RLPStream& append(u256 _s) { return append(bigint(_s)); }
        RLPStream& append(bigint _s);
        RLPStream& append(vector_ref<byte const> _s, bool _compact = false);
        RLPStream& append(bytes const& _s) { return append(vector_ref<byte const>(&_s)); }
        RLPStream& append(std::string const& _s) { return append(vector_ref<byte const>(_s)); }
        RLPStream& append(char const* _s) { return append(std::string(_s)); }
//        template <unsigned N> RLPStream& append(FixedHash<N> _s, bool _compact = false, bool _allOrNothing = false) { return _allOrNothing && !_s ? append(vector_ref<byte const>()) : append(_s.ref(), _compact); }

        /// Appends an arbitrary RLP fragment - this *must* be a single item unless @a _itemCount is given.
        RLPStream& append(RLP const& _rlp, size_t _itemCount = 1) { return appendRaw(_rlp.data(), _itemCount); }

        /// Appends a sequence of data to the stream as a list.
        template <class _T> RLPStream& append(std::vector<_T> const& _s) { return appendVector(_s); }
        template <class _T> RLPStream& appendVector(std::vector<_T> const& _s) { appendList(_s.size()); for (auto const& i: _s) append(i); return *this; }

        /// Appends a list.
        RLPStream& appendList(size_t _items);
        RLPStream& appendList(vector_ref<byte const> _rlp);
        RLPStream& appendList(bytes const& _rlp) { return appendList(&_rlp); }

        /// Appends raw (pre-serialised) RLP data. Use with caution.
        RLPStream& appendRaw(vector_ref<byte const> _rlp, size_t _itemCount = 1);

        /// Shift operators for appending data items.
        template <class T> RLPStream& operator<<(T _data) { return append(_data); }

        /// Clear the output stream so far.
        void clear() { m_out.clear(); m_listStack.clear(); }

        /// Read the byte stream.
        bytes const& out() const { if(!m_listStack.empty()) BOOST_THROW_EXCEPTION(RLPException() << errinfo_comment("listStack is not empty")); return m_out; }

    private:
        void noteAppended(size_t _itemCount = 1);

        /// Push the node-type byte (using @a _base) along with the item count @a _count.
        /// @arg _count is number of characters for strings, data-bytes for ints, or items for lists.
        void pushCount(size_t _count, byte _offset);

        /// Push an integer as a raw big-endian byte-stream.
        template <class _T> void pushInt(_T _i, size_t _br)
        {
            m_out.resize(m_out.size() + _br);
            byte* b = &m_out.back();
            for (; _i; _i >>= 8)
                *(b--) = (byte)_i;
        }

        /// Our output byte stream.
        bytes m_out;

        std::vector<std::pair<size_t, size_t>> m_listStack;
    };

}
