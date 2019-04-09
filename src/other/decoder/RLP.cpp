#include "RLP.h"

using namespace qblocks;

RLP::RLP(vector_ref<byte const> _d) : m_data(_d) {
    if (actualSize() < _d.m_count || actualSize() > _d.m_count) {
        // reset
        _d.m_data = nullptr;
        _d.m_count = 0;
    }
}

RLP::iterator& RLP::iterator::operator++() {
    if (m_remaining) {
        m_currentItem.retarget(m_currentItem.next().m_data, m_remaining);
        m_currentItem = m_currentItem.cropped(0, sizeAsEncoded(m_currentItem));
        m_remaining -= std::min<size_t>(m_remaining, m_currentItem.m_count);
    }
    else
        m_currentItem.retarget(m_currentItem.next().m_data, 0);
    return *this;
}

RLP::iterator::iterator(RLP const& _parent, bool _begin) {
    if (_begin && _parent.isList()) {
        auto pl = _parent.payload();
        m_currentItem = pl.cropped(0, sizeAsEncoded(pl));
        m_remaining = pl.m_count - m_currentItem.m_count;

    } else {
        m_currentItem = _parent.data().cropped(_parent.data().m_count);
        m_remaining = 0;
    }
}

RLP RLP::operator[](size_t _i) const {
    if (_i < m_lastIndex) {
        m_lastEnd = sizeAsEncoded(payload());
        m_lastItem = payload().cropped(0, m_lastEnd);
        m_lastIndex = 0;
    }

    for (; m_lastIndex < _i && m_lastItem.m_count; ++m_lastIndex) {
        m_lastItem = payload().cropped(m_lastEnd);
        m_lastItem = m_lastItem.cropped(0, sizeAsEncoded(m_lastItem));
        m_lastEnd += m_lastItem.m_count;
    }
    return RLP(m_lastItem);
}

bool RLP::isInt() const {
    if (isNull())
        return false;
    byte n = m_data[0];
    if (n < c_rlpDataImmLenStart) {
        return n != 0;

    } else if (n == c_rlpDataImmLenStart) {
        return true;

    } else if (n <= c_rlpDataIndLenZero) {
        return m_data[1] != 0;

    } else if (n < c_rlpListStart) {
        return m_data[1 + n - c_rlpDataIndLenZero] != 0;

    }
    return false;
}

size_t RLP::length() const {
    if (isNull())
        return 0;

    size_t ret = 0;
    byte const n = m_data[0];
    if (n < c_rlpDataImmLenStart) {
        return 1;

    } else if (n <= c_rlpDataIndLenZero) {
        return n - c_rlpDataImmLenStart;

    } else if (n < c_rlpListStart) {
        unsigned lengthSize = n - c_rlpDataIndLenZero;
        for (unsigned i = 0; i < lengthSize; ++i)
            ret = (ret << 8) | m_data[i + 1];

    } else if (n <= c_rlpListIndLenZero) {
        return n - c_rlpListStart;

    } else {
        unsigned lengthSize = n - c_rlpListIndLenZero;
        for (unsigned i = 0; i < lengthSize; ++i)
            ret = (ret << 8) | m_data[i + 1];
    }

    return ret;
}

RLPStream& RLPStream::appendRaw(vector_ref<byte const> _s, size_t _itemCount) {
    m_out.insert(m_out.end(), _s.begin(), _s.end());
    noteAppended(_itemCount);
    return *this;
}

void RLPStream::noteAppended(size_t _itemCount) {
    if (!_itemCount)
        return;
    while (m_listStack.size()) {
        m_listStack.back().first -= _itemCount;
        if (m_listStack.back().first) {
            break;

        } else {
            auto p = m_listStack.back().second;
            m_listStack.pop_back();
            size_t s = m_out.size() - p;        // list size
            auto brs = bytesRequired(s);
            unsigned encodeSize = s < c_rlpListImmLenCount ? 1 : (1 + brs);
            auto os = m_out.size();
            m_out.resize(os + encodeSize);
            memmove(m_out.data() + p + encodeSize, m_out.data() + p, os - p);
            if (s < c_rlpListImmLenCount)
                m_out[p] = (byte)(c_rlpListStart + s);
            else if (c_rlpListIndLenZero + brs <= 0xff) {
                m_out[p] = (byte)(c_rlpListIndLenZero + brs);
                byte* b = &(m_out[p + brs]);
                for (; s; s >>= 8)
                    *(b--) = (byte)s;
            }
        }
        _itemCount = 1;    // for all following iterations, we've effectively appended a single item only since we completed a list.
    }
}

RLPStream& RLPStream::appendList(size_t _items) {
    if (_items)
        m_listStack.push_back(std::make_pair(_items, m_out.size()));
    else
        appendList(bytes());
    return *this;
}

RLPStream& RLPStream::appendList(vector_ref<byte const> _rlp) {
    if (_rlp.m_count < c_rlpListImmLenCount)
        m_out.push_back((byte)(_rlp.m_count + c_rlpListStart));
    else
        pushCount(_rlp.m_count, c_rlpListIndLenZero);
    appendRaw(_rlp, 1);
    return *this;
}

RLPStream& RLPStream::append(vector_ref<byte const> _s, bool _compact) {
    size_t s = _s.m_count;
    byte const* d = _s.m_data;
    if (_compact) {
        for (size_t i = 0; i < _s.m_count && !*d; ++i, --s, ++d) {
        }
    }

    if (s == 1 && *d < c_rlpDataImmLenStart) {
        m_out.push_back(*d);
    } else {
        if (s < c_rlpDataImmLenCount)
            m_out.push_back((byte)(s + c_rlpDataImmLenStart));
        else
            pushCount(s, c_rlpDataIndLenZero);
        appendRaw(vector_ref<byte const>(d, s), 0);
    }
    noteAppended();
    return *this;
}

RLPStream& RLPStream::append(bigint _i) {
#if 0
    if (!_i)
        m_out.push_back(c_rlpDataImmLenStart);
    else if (_i < c_rlpDataImmLenStart)
        m_out.push_back((byte)_i);
    else {
        unsigned br = bytesRequired(_i);
        if (br < c_rlpDataImmLenCount)
            m_out.push_back((byte)(br + c_rlpDataImmLenStart));
        else {
            auto brbr = bytesRequired(br);
            if (c_rlpDataIndLenZero + brbr > 0xff)
                BOOST_THROW_EXCEPTION(RLPException() << errinfo_comment("Number too large for RLP"));
            m_out.push_back((byte)(c_rlpDataIndLenZero + brbr));
            pushInt(br, brbr);
        }
        pushInt(_i, br);
    }
    noteAppended();
#endif
    return *this;
}

void RLPStream::pushCount(size_t _count, byte _base) {
    auto br = bytesRequired(_count);
    if (int(br) + _base > 0xff)
        BOOST_THROW_EXCEPTION(RLPException() << errinfo_comment("Count too large for RLP"));
    m_out.push_back((byte)(br + _base));    // max 8 bytes.
    pushInt(_count, br);
}

namespace qblocks {
    int fromHex(char _i) {
        if (_i >= '0' && _i <= '9') return _i - '0';
        if (_i >= 'a' && _i <= 'f') return _i - 'a' + 10;
        if (_i >= 'A' && _i <= 'F') return _i - 'A' + 10;
        return -1;
    }

    using byte = uint8_t;
    using bytes = vector<byte>;

    bytes fromHex(const string_q& _sIn) {

        string_q _s = "0x" + substitute(_sIn, "0x", "");
        if (!isHexStr(_s))
            return bytes();
        _s = _s.substr(2);

        bytes ret;
        ret.reserve(_s.size() / 2);
        for (unsigned i = 0 ; i < _s.size(); i += 2) {
            int nib1 = fromHex(_s[i]);
            int nib2 = fromHex(_s[i + 1]);
            ret.push_back((byte)(nib1 * 16 + nib2));
        }

        return ret;
    }
};
