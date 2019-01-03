#pragma once
/**
 * @file cpptoml.h
 * @author Chase Geigle
 * @date May 2013
 */

#include "utillib.h"
#include <unordered_map>
#include "toml_support.h"

namespace cpptoml {

    class CTomlBase;
    using string_to_base_map = unordered_map<string, shared_ptr<CTomlBase>>;

    template <class T, class... Ts>
    struct is_one_of;

    template <class T, class V>
    struct is_one_of<T, V> : is_same<T, V> {
    };

    template <class T, class V, class... Ts>
    struct is_one_of<T, V, Ts...> {
        const static bool value = is_same<T, V>::value || is_one_of<T, Ts...>::value;
    };

    template <class T>
    class CTomlValue;

    class CTomlArray;
    class CTomlTable;
    class CTomlTableArray;

    template <class T>
    struct valid_val_t
        : is_one_of<T, string, int64_t, double, bool, loc_date_t, loc_time_t, loc_datetime_t, off_datetime_t> {
    };

    template <class T, class Enable = void>
    struct value_traits;

    template <class T>
    struct valid_value_or_string_convertible {
        const static bool value = valid_val_t<typename decay<T>::type>::value || is_convertible<T, string>::value;
    };

    template <class T>
    struct value_traits<T, typename enable_if<valid_value_or_string_convertible<T>::value>::type> {
        using value_type = typename conditional<
        valid_val_t<typename decay<T>::type>::value,
        typename decay<T>::type, string>::type;

        using type = CTomlValue<value_type>;

        static value_type construct(T&& val)
        {
            return value_type(val);
        }
    };

    template <class T>
    struct value_traits<T,
        typename enable_if<!valid_value_or_string_convertible<T>::value && is_floating_point<typename decay<T>::type>::value>::type> {
        using value_type = typename decay<T>::type;
        using type = CTomlValue<double>;
        static value_type construct(T&& val) {
            return value_type(val);
        }
    };

    template <class T>
    struct value_traits<T,
        typename enable_if<!valid_value_or_string_convertible<T>::value
                            && !is_floating_point<typename decay<T>::type>::value
                            && is_signed<typename decay<T>::type>::value>::type> {
        using value_type = int64_t;
        using type = CTomlValue<int64_t>;
        static value_type construct(T&& val) {
            if (val < (numeric_limits<int64_t>::min)())
                throw underflow_error{"constructed value cannot be represented by a 64-bit signed integer"};
            if (val > (numeric_limits<int64_t>::max)())
                throw overflow_error{"constructed value cannot be represented by a 64-bit signed integer"};
            return static_cast<int64_t>(val);
        }
    };

    template <class T>
    struct value_traits<T,
        typename enable_if<!valid_value_or_string_convertible<T>::value
                            && is_unsigned<typename decay<T>::type>::value>::type> {
        using value_type = int64_t;
        using type = CTomlValue<int64_t>;
        static value_type construct(T&& val) {
            if (val > static_cast<uint64_t>((numeric_limits<int64_t>::max)()))
                throw overflow_error{"constructed value cannot be represented by a 64-bit signed integer"};
            return static_cast<int64_t>(val);
        }
    };

    template <class T>
    struct array_of_trait {
        using return_type = option<vector<T>>;
    };

    template <>
    struct array_of_trait<CTomlArray> {
        using return_type = option<vector<shared_ptr<CTomlArray>>>;
    };

    template <class T>
    inline shared_ptr<typename value_traits<T>::type> make_value(T&& val);
    inline shared_ptr<CTomlArray> make_array();

    namespace detail {
        template <class T>
        inline shared_ptr<T> make_element();
    }

    inline shared_ptr<CTomlTable> makeTable();
    inline shared_ptr<CTomlTableArray> makeTableArray(bool is_inline = false);

    //-----------------------------------------------------------------
    class CTomlBase : public enable_shared_from_this<CTomlBase> {
    public:
        virtual ~CTomlBase() = default;
        virtual shared_ptr<CTomlBase> clone() const = 0;
        virtual bool isValue() const { return false; }
        virtual bool isTable() const { return false; }
        virtual bool isArray() const { return false; }
        virtual bool isTableArray() const { return false; }
        shared_ptr<CTomlTable> as_table() {
            if (isTable())
                return static_pointer_cast<CTomlTable>(shared_from_this());
            return nullptr;
        }
        shared_ptr<CTomlArray> as_array() {
            if (isArray())
                return static_pointer_cast<CTomlArray>(shared_from_this());
            return nullptr;
        }
        shared_ptr<CTomlTableArray> asTableArray() {
            if (isTableArray())
                return static_pointer_cast<CTomlTableArray>(shared_from_this());
            return nullptr;
        }

        template <class T>
        shared_ptr<CTomlValue<T>> as();

        template <class T>
        shared_ptr<const CTomlValue<T>> as() const;

        template <class Visitor, class... Args>
        void accept(Visitor&& visitor, Args&&... args) const;

    protected:
        CTomlBase() { }
    };

    //-----------------------------------------------------------------
    template <class T>
    class CTomlValue : public CTomlBase {
        struct make_shared_enabler { };
        template <class U>
        friend shared_ptr<typename value_traits<U>::type>
        cpptoml::make_value(U&& val);
    public:
        static_assert(valid_val_t<T>::value, "invalid value type");
        shared_ptr<CTomlBase> clone() const override;
        CTomlValue(const make_shared_enabler&, const T& val) : CTomlValue(val) { }
        bool isValue() const override { return true; }
        T& get() { return data_; }
        const T& get() const { return data_; }
    private:
        T data_;
        CTomlValue(const T& val) : data_(val) { }
        CTomlValue(const CTomlValue& val) = delete;
        CTomlValue& operator=(const CTomlValue& val) = delete;
    };

    template <class T>
    shared_ptr<typename value_traits<T>::type> make_value(T&& val)
    {
        using value_type = typename value_traits<T>::type;
        using enabler = typename value_type::make_shared_enabler;
        return make_shared<value_type>(
                                            enabler{}, value_traits<T>::construct(forward<T>(val)));
    }

    template <class T>
    inline shared_ptr<CTomlValue<T>> CTomlBase::as() {
        return dynamic_pointer_cast<CTomlValue<T>>(shared_from_this());
    }

    // special case value<double> to allow getting an integer parameter as a
    // double value
    template <>
    inline shared_ptr<CTomlValue<double>> CTomlBase::as()
    {
        if (auto v = dynamic_pointer_cast<CTomlValue<double>>(shared_from_this()))
            return v;
        if (auto v = dynamic_pointer_cast<CTomlValue<int64_t>>(shared_from_this()))
            return make_value<double>(static_cast<double>(v->get()));

        return nullptr;
    }

    template <class T>
    inline shared_ptr<const CTomlValue<T>> CTomlBase::as() const {
        return dynamic_pointer_cast<const CTomlValue<T>>(shared_from_this());
    }

    template <>
    inline shared_ptr<const CTomlValue<double>> CTomlBase::as() const {
        if (auto v = dynamic_pointer_cast<const CTomlValue<double>>(shared_from_this()))
            return v;
        if (auto v = as<int64_t>()) {
            // the below has to be a non-const value<double> due to a bug in
            // libc++: https://llvm.org/bugs/show_bug.cgi?id=18843
            return make_value<double>(static_cast<double>(v->get()));
        }
        return nullptr;
    }

    //-----------------------------------------------------------------
    class CTomlArray : public CTomlBase {
    public:
        friend shared_ptr<CTomlArray> make_array();
        shared_ptr<CTomlBase> clone() const override;
        virtual bool isArray() const override {
            return true;
        }
        using size_type = size_t;
        using iterator = vector<shared_ptr<CTomlBase>>::iterator;
        using const_iterator = vector<shared_ptr<CTomlBase>>::const_iterator;
        iterator begin() { return values_.begin(); }
        const_iterator begin() const { return values_.begin(); }
        iterator end() { return values_.end(); }
        const_iterator end() const { return values_.end(); }
        vector<shared_ptr<CTomlBase>>& get() { return values_; }
        const vector<shared_ptr<CTomlBase>>& get() const { return values_; }
        shared_ptr<CTomlBase> at(size_t idx) const { return values_.at(idx); }
        template <class T>
        vector<shared_ptr<CTomlValue<T>>> array_of() const {
            vector<shared_ptr<CTomlValue<T>>> result(values_.size());
            transform(values_.begin(), values_.end(), result.begin(),
                           [&](shared_ptr<CTomlBase> v) { return v->as<T>(); });
            return result;
        }
        template <class T>
        inline typename array_of_trait<T>::return_type get_array_of() const {
            vector<T> result;
            result.reserve(values_.size());
            for (const auto& val : values_) {
                if (auto v = val->as<T>())
                    result.push_back(v->get());
                else
                    return {};
            }
            return {move(result)};
        }

        vector<shared_ptr<CTomlArray>> nested_array() const {
            vector<shared_ptr<CTomlArray>> result(values_.size());
            transform(values_.begin(), values_.end(), result.begin(),
                           [&](shared_ptr<CTomlBase> v) -> shared_ptr<CTomlArray> {
                               if (v->isArray())
                                   return static_pointer_cast<CTomlArray>(v);
                               return shared_ptr<CTomlArray>{};
                           });
            return result;
        }

        template <class T>
        void push_back(const shared_ptr<CTomlValue<T>>& val) {
            if (values_.empty() || values_[0]->as<T>())
                values_.push_back(val);
            throw runtime_error{"Arrays must be homogenous."};
        }

        void push_back(const shared_ptr<CTomlArray>& val) {
            if (values_.empty() || values_[0]->isArray())
                values_.push_back(val);
            throw runtime_error{"Arrays must be homogenous."};
        }

        template <class T>
        void push_back(T&& val, typename value_traits<T>::type* = 0) {
            push_back(make_value(forward<T>(val)));
        }

        template <class T>
        iterator insert(iterator position, const shared_ptr<CTomlValue<T>>& value) {
            if (values_.empty() || values_[0]->as<T>())
                return values_.insert(position, value);
            throw runtime_error{"Arrays must be homogenous."};
        }

        iterator insert(iterator position, const shared_ptr<CTomlArray>& value) {
            if (values_.empty() || values_[0]->isArray())
                return values_.insert(position, value);
            throw runtime_error{"Arrays must be homogenous."};
        }

        template <class T>
        iterator insert(iterator position, T&& val, typename value_traits<T>::type* = 0) {
            return insert(position, make_value(forward<T>(val)));
        }

        iterator erase(iterator position) {
            return values_.erase(position);
        }

        void clear() {
            values_.clear();
        }

        void reserve(size_type n) {
            values_.reserve(n);
        }

    private:
        CTomlArray() = default;
        template <class InputIterator>
        CTomlArray(InputIterator begin, InputIterator end) : values_{begin, end} { }
        CTomlArray(const CTomlArray& obj) = delete;
        CTomlArray& operator=(const CTomlArray& obj) = delete;
        vector<shared_ptr<CTomlBase>> values_;
    };

    inline shared_ptr<CTomlArray> make_array() {
        struct make_shared_enabler : public CTomlArray {
            make_shared_enabler() { }
        };
        return make_shared<make_shared_enabler>();
    }

    namespace detail {
        template <>
        inline shared_ptr<CTomlArray> make_element<CTomlArray>() { return make_array(); }
    } // namespace detail

    template <>
    inline typename array_of_trait<CTomlArray>::return_type
    CTomlArray::get_array_of<CTomlArray>() const {
        vector<shared_ptr<CTomlArray>> result;
        result.reserve(values_.size());
        for (const auto& val : values_) {
            if (auto v = val->as_array())
                result.push_back(v);
            else
                return {};
        }
        return {move(result)};
    }

    using strval_t = CTomlValue<string>;
    using strvalptr_t = shared_ptr<strval_t>;

    //-----------------------------------------------------------------
    class CTomlTableArray : public CTomlBase {
        friend class CTomlTable;
        friend shared_ptr<CTomlTableArray> makeTableArray(bool);
    public:
        shared_ptr<CTomlBase> clone() const override;
        using size_type = size_t;
        using iterator = vector<shared_ptr<CTomlTable>>::iterator;
        using const_iterator = vector<shared_ptr<CTomlTable>>::const_iterator;
        iterator begin() { return array_.begin(); }
        const_iterator begin() const { return array_.begin(); }
        iterator end() { return array_.end(); }
        const_iterator end() const { return array_.end(); }
        virtual bool isTableArray() const override { return true; }
        vector<shared_ptr<CTomlTable>>& get() { return array_; }
        const vector<shared_ptr<CTomlTable>>& get() const { return array_; }
        void push_back(const shared_ptr<CTomlTable>& val) { array_.push_back(val); }
        iterator insert(iterator position, const shared_ptr<CTomlTable>& value) { return array_.insert(position, value); }
        iterator erase(iterator position) { return array_.erase(position); }
        void clear() { array_.clear(); }
        void reserve(size_type n) { array_.reserve(n); }
        bool is_inline() const { return is_inline_; }
    private:
        CTomlTableArray(bool is_inline = false) : is_inline_(is_inline) { }
        CTomlTableArray(const CTomlTableArray& obj) = delete;
        CTomlTableArray& operator=(const CTomlTableArray& rhs) = delete;
        vector<shared_ptr<CTomlTable>> array_;
        const bool is_inline_ = false;
    };

    inline shared_ptr<CTomlTableArray> makeTableArray(bool is_inline) {
        struct make_shared_enabler : public CTomlTableArray {
            make_shared_enabler(bool mse_is_inline) : CTomlTableArray(mse_is_inline) { }
        };
        return make_shared<make_shared_enabler>(is_inline);
    }

    namespace detail {
        template <>
        inline shared_ptr<CTomlTableArray> make_element<CTomlTableArray>() {
            return makeTableArray(true);
        }
    } // namespace detail

    // The below are overloads for fetching specific value types out of a value
    // where special casting behavior (like bounds checking) is desired

    template <class T>
    typename enable_if<
        !is_floating_point<T>::value && is_signed<T>::value,
        option<T>>::type get_impl(const shared_ptr<CTomlBase>& elem) {
        if (auto v = elem->as<int64_t>()) {
            if (v->get() < (numeric_limits<T>::min)())
                throw underflow_error{ "T cannot represent the value requested in get"};
            if (v->get() > (numeric_limits<T>::max)())
                throw overflow_error{ "T cannot represent the value requested in get"};
            return {static_cast<T>(v->get())};
        } else {
            return {};
        }
    }

    template <class T>
    typename enable_if<
        !is_same<T, bool>::value && is_unsigned<T>::value,
        option<T>>::type get_impl(const shared_ptr<CTomlBase>& elem) {
        if (auto v = elem->as<int64_t>()) {
            if (v->get() < 0)
                throw underflow_error{"T cannot store negative value in get"};
            if (static_cast<uint64_t>(v->get()) > (numeric_limits<T>::max)())
                throw overflow_error{ "T cannot represent the value requested in get"};
            return {static_cast<T>(v->get())};
        } else {
            return {};
        }
    }

    template <class T>
    typename enable_if<
        !is_integral<T>::value || is_same<T, bool>::value,
        option<T>>::type get_impl(const shared_ptr<CTomlBase>& elem) {
        if (auto v = elem->as<T>()) {
            return {v->get()};
        } else {
            return {};
        }
    }

    //-----------------------------------------------------------------
    class CTomlTable : public CTomlBase {
    public:
        friend class CTomlTableArray;
        friend shared_ptr<CTomlTable> makeTable();
        shared_ptr<CTomlBase> clone() const override;
        using iterator = string_to_base_map::iterator;
        using const_iterator = string_to_base_map::const_iterator;
        iterator begin() { return map_.begin(); }
        const_iterator begin() const { return map_.begin(); }
        iterator end() { return map_.end(); }
        const_iterator end() const { return map_.end(); }
        bool isTable() const override { return true; }
        bool empty() const { return map_.empty(); }
        bool contains(const string& key) const { return map_.find(key) != map_.end(); }
        bool contains_qualified(const string& key) const { return resolve_qualified(key); }
        shared_ptr<CTomlBase> get(const string& key) const { return map_.at(key); }
        shared_ptr<CTomlBase> get_qualified(const string& key) const {
            shared_ptr<CTomlBase> p;
            resolve_qualified(key, &p);
            return p;
        }
        shared_ptr<CTomlTable> get_table(const string& key) const {
            if (contains(key) && get(key)->isTable())
                return static_pointer_cast<CTomlTable>(get(key));
            return nullptr;
        }
        shared_ptr<CTomlTable> get_table_qualified(const string& key) const {
            if (contains_qualified(key) && get_qualified(key)->isTable())
                return static_pointer_cast<CTomlTable>(get_qualified(key));
            return nullptr;
        }
        shared_ptr<CTomlArray> get_array(const string& key) const {
            if (!contains(key))
                return nullptr;
            return get(key)->as_array();
        }
        shared_ptr<CTomlArray> getArrayQualified(const string& key) const {
            if (!contains_qualified(key))
                return nullptr;
            return get_qualified(key)->as_array();
        }
        template <class T>
        option<T> get_as(const string& key) const {
            try {
                return get_impl<T>(get(key));
            } catch (const out_of_range&) {
                return {};
            }
        }
        template <class T>
        option<T> get_qualified_as(const string& key) const {
            try {
                return get_impl<T>(get_qualified(key));
            } catch (const out_of_range&) {
                return {};
            }
        }
        template <class T>
        inline typename array_of_trait<T>::return_type get_array_of(const string& key) const {
            if (auto v = get_array(key)) {
                vector<T> result;
                result.reserve(v->get().size());
                for (const auto& b : v->get()) {
                    if (auto val = b->as<T>())
                        result.push_back(val->get());
                    else
                        return {};
                }
                return {move(result)};
            }
            return {};
        }

        template <class T>
        inline typename array_of_trait<T>::return_type getQualifiedArrayOf(const string& key) const {
            if (auto v = getArrayQualified(key)) {
                vector<T> result;
                result.reserve(v->get().size());
                for (const auto& b : v->get()) {
                    if (auto val = b->as<T>())
                        result.push_back(val->get());
                    else
                        return {};
                }
                return {move(result)};
            }
            return {};
        }

        void insert(const string& key, const shared_ptr<CTomlBase>& value) {
            map_[key] = value;
        }

        template <class T>
        void insert(const string& key, T&& val, typename value_traits<T>::type* = 0) {
            insert(key, make_value(forward<T>(val)));
        }

        void erase(const string& key) {
            map_.erase(key);
        }

    private:
        CTomlTable() { }
        CTomlTable(const CTomlTable& obj) = delete;
        CTomlTable& operator=(const CTomlTable& rhs) = delete;
        CStringArray split(const string& value, char separator) const {
            CStringArray result;
            string::size_type p = 0;
            string::size_type q;
            while ((q = value.find(separator, p)) != string::npos) {
                result.emplace_back(value, p, q - p);
                p = q + 1;
            }
            result.emplace_back(value, p);
            return result;
        }

        bool resolve_qualified(const string& key, shared_ptr<CTomlBase>* p = nullptr) const {
            auto parts = split(key, '.');
            auto last_key = parts.back();
            parts.pop_back();
            auto cur_table = this;
            for (const auto& part : parts) {
                cur_table = cur_table->get_table(part).get();
                if (!cur_table) {
                    if (!p)
                        return false;
                    throw out_of_range{key + " is not a valid key"};
                }
            }
            if (!p)
                return cur_table->map_.count(last_key) != 0;
            *p = cur_table->map_.at(last_key);
            return true;
        }

        string_to_base_map map_;
    };

    template <>
    inline typename array_of_trait<CTomlArray>::return_type CTomlTable::get_array_of<CTomlArray>(const string& key) const {
        if (auto v = get_array(key)) {
            vector<shared_ptr<CTomlArray>> result;
            result.reserve(v->get().size());
            for (const auto& b : v->get()) {
                if (auto val = b->as_array())
                    result.push_back(val);
                else
                    return {};
            }
            return {move(result)};
        }
        return {};
    }

    template <>
    inline typename array_of_trait<CTomlArray>::return_type CTomlTable::getQualifiedArrayOf<CTomlArray>(const string& key) const {
        if (auto v = getArrayQualified(key)) {
            vector<shared_ptr<CTomlArray>> result;
            result.reserve(v->get().size());
            for (const auto& b : v->get()) {
                if (auto val = b->as_array())
                    result.push_back(val);
                else
                    return {};
            }
            return {move(result)};
        }
        return {};
    }

    shared_ptr<CTomlTable> makeTable() {
        struct make_shared_enabler : public CTomlTable {
            make_shared_enabler() { }
        };
        return make_shared<make_shared_enabler>();
    }

    namespace detail {
        template <>
        inline shared_ptr<CTomlTable> make_element<CTomlTable>() {
            return makeTable();
        }
    } // namespace detail

    template <class T>
    shared_ptr<CTomlBase> CTomlValue<T>::clone() const {
        return make_value(data_);
    }

    inline shared_ptr<CTomlBase> CTomlArray::clone() const {
        auto result = make_array();
        result->reserve(values_.size());
        for (const auto& ptr : values_)
            result->values_.push_back(ptr->clone());
        return result;
    }

    inline shared_ptr<CTomlBase> CTomlTableArray::clone() const {
        auto result = makeTableArray(is_inline());
        result->reserve(array_.size());
        for (const auto& ptr : array_)
            result->array_.push_back(ptr->clone()->as_table());
        return result;
    }

    inline shared_ptr<CTomlBase> CTomlTable::clone() const {
        auto result = makeTable();
        for (const auto& pr : map_)
            result->insert(pr.first, pr.second->clone());
        return result;
    }

    //-----------------------------------------------------------------
    class parse_exception : public runtime_error {
    public:
        parse_exception(const string& err) : runtime_error{err} {
        }
        parse_exception(const string& err, size_t line_number)
          : runtime_error{err + " at line " + to_string(line_number)} {
        }
    };

    inline bool is_number(char c) {
        return c >= '0' && c <= '9';
    }

    inline bool is_hex(char c) {
        return is_number(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }

    using striter_t = string::iterator;

    template <class OnError>
    //-----------------------------------------------------------------
    class consumer {
    public:
        consumer(striter_t& it, const striter_t& end, OnError&& on_error)
        : it_(it), end_(end), on_error_(forward<OnError>(on_error)) { }
        void operator()(char c) {
            if (it_ == end_ || *it_ != c)
                on_error_();
            ++it_;
        }
        template <size_t N>
        void operator()(const char (&str)[N]) {
            for_each(begin(str), end(str) - 1, [&](char c) { (*this)(c); });
        }
        void eat_or(char a, char b) {
            if (it_ == end_ || (*it_ != a && *it_ != b))
                on_error_();
            ++it_;
        }
        int eat_digits(int len) {
            int val = 0;
            for (int i = 0; i < len; ++i) {
                if (!is_number(*it_) || it_ == end_)
                    on_error_();
                val = 10 * val + (*it_++ - '0');
            }
            return val;
        }
        void error(void) {
            on_error_();
        }
    private:
        striter_t& it_;
        const striter_t& end_;
        OnError on_error_;
    };

    template <class OnError>
    consumer<OnError> make_consumer(striter_t& it, const striter_t& end, OnError&& on_error) {
        return consumer<OnError>(it, end, forward<OnError>(on_error));
    }

    namespace detail {
        inline istream& getline(istream& input, string& line) {
            line.clear();
            istream::sentry sentry{input, true};
            auto sb = input.rdbuf();
            while (true) {
                auto c = sb->sbumpc();
                if (c == '\r') {
                    if (sb->sgetc() == '\n')
                        c = sb->sbumpc();
                }
                if (c == '\n')
                    return input;
                if (c == istream::traits_type::eof()) {
                    if (line.empty())
                        input.setstate(ios::eofbit);
                    return input;
                }
                line.push_back(static_cast<char>(c));
            }
        }
    } // namespace detail

    //-----------------------------------------------------------------
    class parser {
    public:
        parser(istream& stream) : input_(stream) { }
        parser& operator=(const parser& parser) = delete;
        shared_ptr<CTomlTable> parse() {
            shared_ptr<CTomlTable> root = makeTable();
            CTomlTable* curr_table = root.get();
            while (detail::getline(input_, line_)) {
                line_number_++;
                auto it = line_.begin();
                auto end = line_.end();
                consume_whitespace(it, end);
                if (it == end || *it == '#')
                    continue;
                if (*it == '[') {
                    curr_table = root.get();
                    parse_table(it, end, curr_table);
                } else {
                    parse_key_value(it, end, curr_table);
                    consume_whitespace(it, end);
                    eol_or_comment(it, end);
                }
            }
            return root;
        }
    private:
        enum class parse_t {
            STRING = 1,
            LOCAL_TIME,
            LOCAL_DATE,
            LOCAL_DATETIME,
            OFFSET_DATETIME,
            INT,
            FLOAT,
            BOOL,
            ARRAY,
            INLINE_TABLE,
            INVALID_TYPE
        };

        //----
        void throw_parse_exception(const string& err) {
            throw parse_exception{err, line_number_};
            return;
        }

        //----
        void parse_table(striter_t& it, const striter_t& end, CTomlTable*& curr_table) {
            ++it;
            if (it == end)
                throw_parse_exception("Unexpected end of table");
            if (*it == '[')
                parse_table_array(it, end, curr_table);
            else
                parse_single_table(it, end, curr_table);
        }

        //----
        void parse_single_table(striter_t& it, const striter_t& end, CTomlTable*& curr_table) {
            if (it == end || *it == ']')
                throw_parse_exception("Table name cannot be empty");
            string full_table_name;
            bool inserted = false;
            auto key_end = [](char c) { return c == ']'; };
            auto key_part_handler = [&](const string& part) {
                if (part.empty())
                    throw_parse_exception("Empty component of table name");
                if (!full_table_name.empty())
                    full_table_name += '.';
                full_table_name += part;

                if (curr_table->contains(part)) {
                    auto b = curr_table->get(part);
                    if (b->isTable())
                        curr_table = static_cast<CTomlTable*>(b.get());
                    else if (b->isTableArray())
                        curr_table = static_pointer_cast<CTomlTableArray>(b)
                        ->get()
                        .back()
                        .get();
                    else
                        throw_parse_exception("Key " + full_table_name
                                              + "already exists as a value");
                }
                else
                {
                    inserted = true;
                    curr_table->insert(part, makeTable());
                    curr_table = static_cast<CTomlTable*>(curr_table->get(part).get());
                }
            };

            key_part_handler(parse_key(it, end, key_end, key_part_handler));

            if (it == end)
                throw_parse_exception(
                                      "Unterminated table declaration; did you forget a ']'?");

            if (*it != ']')
            {
                string errmsg{"Unexpected character in table definition: "};
                errmsg += '"';
                errmsg += *it;
                errmsg += '"';
                throw_parse_exception(errmsg);
            }

            // table already existed
            if (!inserted)
            {
                auto isValue = [](const pair<const string&,
                     const shared_ptr<CTomlBase>&>& p) {
                    return p.second->isValue();
                };

                // if there are any values, we can't add values to this table
                // since it has already been defined. If there aren't any
                // values, then it was implicitly created by something like
                // [a.b]
                if (curr_table->empty()
                    || any_of(curr_table->begin(), curr_table->end(),
                                   isValue))
                {
                    throw_parse_exception("Redefinition of table "
                                          + full_table_name);
                }
            }

            ++it;
            consume_whitespace(it, end);
            eol_or_comment(it, end);
        }

        //----
        void parse_table_array(striter_t& it, const striter_t& end, CTomlTable*& curr_table) {
            ++it;
            if (it == end || *it == ']')
                throw_parse_exception("Table array name cannot be empty");

            auto key_end = [](char c) { return c == ']'; };

            string full_ta_name;
            auto key_part_handler = [&](const string& part) {
                if (part.empty())
                    throw_parse_exception("Empty component of table array name");

                if (!full_ta_name.empty())
                    full_ta_name += '.';
                full_ta_name += part;

                if (curr_table->contains(part))
                {
                    auto b = curr_table->get(part);

                    // if this is the end of the table array name, add an
                    // element to the table array that we just looked up,
                    // provided it was not declared inline
                    if (it != end && *it == ']') {
                        if (!b->isTableArray()) {
                            throw_parse_exception("Key " + full_ta_name + " is not a table array");
                        }

                        auto v = b->asTableArray();
                        if (v->is_inline()) {
                            throw_parse_exception("Static array " + full_ta_name + " cannot be appended to");
                        }

                        v->get().push_back(makeTable());
                        curr_table = v->get().back().get();
                    } else {
                        // otherwise, just keep traversing down the key name
                        if (b->isTable())
                            curr_table = static_cast<CTomlTable*>(b.get());
                        else if (b->isTableArray())
                            curr_table = static_pointer_cast<CTomlTableArray>(b)
                            ->get()
                            .back()
                            .get();
                        else
                            throw_parse_exception("Key " + full_ta_name
                                                  + " already exists as a value");
                    }
                }
                else
                {
                    // if this is the end of the table array name, add a new
                    // table array and a new table inside that array for us to
                    // add keys to next
                    if (it != end && *it == ']')
                    {
                        curr_table->insert(part, makeTableArray());
                        auto arr = static_pointer_cast<CTomlTableArray>(
                                                                         curr_table->get(part));
                        arr->get().push_back(makeTable());
                        curr_table = arr->get().back().get();
                    }
                    // otherwise, create the implicitly defined table and move
                    // down to it
                    else
                    {
                        curr_table->insert(part, makeTable());
                        curr_table
                        = static_cast<CTomlTable*>(curr_table->get(part).get());
                    }
                }
            };

            key_part_handler(parse_key(it, end, key_end, key_part_handler));

            // consume the last "]]"
            auto eat = make_consumer(it, end, [this]() {
                throw_parse_exception("Unterminated table array name");
            });
            eat(']');
            eat(']');

            consume_whitespace(it, end);
            eol_or_comment(it, end);
        }

        //----
        void parse_key_value(striter_t& it, striter_t& end, CTomlTable* curr_table) {
            auto key_end = [](char c) { return c == '='; };
            auto key_part_handler = [&](const string& part) {
                // two cases: this key part exists already, in which case it must
                // be a table, or it doesn't exist in which case we must create
                // an implicitly defined table
                if (curr_table->contains(part)) {
                    auto val = curr_table->get(part);
                    if (val->isTable()) {
                        curr_table = static_cast<CTomlTable*>(val.get());
                    } else {
                        throw_parse_exception("Key " + part
                                              + " already exists as a value");
                    }
                } else {
                    auto newtable = makeTable();
                    curr_table->insert(part, newtable);
                    curr_table = newtable.get();
                }
            };

            auto key = parse_key(it, end, key_end, key_part_handler);
            if (curr_table->contains(key))
                throw_parse_exception("Key " + key + " already present");
            if (it == end || *it != '=')
                throw_parse_exception("Value must follow after a '='");
            ++it;
            consume_whitespace(it, end);
            curr_table->insert(key, parse_value(it, end));
            consume_whitespace(it, end);
        }

        //----
        template <class KeyEndFinder, class KeyPartHandler>
        string parse_key(striter_t& it, const striter_t& end, KeyEndFinder&& key_end, KeyPartHandler&& key_part_handler) {
            // parse the key as a series of one or more simple-keys joined with '.'
            while (it != end && !key_end(*it)) {
                auto part = parse_simple_key(it, end);
                consume_whitespace(it, end);

                if (it == end || key_end(*it)) {
                    return part;
                }

                if (*it != '.') {
                    string errmsg{"Unexpected character in key: "};
                    errmsg += '"';
                    errmsg += *it;
                    errmsg += '"';
                    throw_parse_exception(errmsg);
                }

                key_part_handler(part);

                // consume the dot
                ++it;
            }

            throw_parse_exception("Unexpected end of key");
            return nullptr;
        }

        //----
        string parse_simple_key(striter_t& it, const striter_t& end) {
            consume_whitespace(it, end);
            if (it == end)
                throw_parse_exception("Unexpected end of key (blank key?)");
            if (*it == '"' || *it == '\'') {
                return string_literal(it, end, *it);
            } else {
                auto bke = find_if(it, end, [](char c) {
                    return c == '.' || c == '=' || c == ']';
                });
                return parse_bare_key(it, bke);
            }
        }

        //----
        string parse_bare_key(striter_t& it, const striter_t& end) {
            if (it == end) {
                throw_parse_exception("Bare key missing name");
            }

            auto key_end = end;
            --key_end;
            consume_backwards_whitespace(key_end, it);
            ++key_end;
            string key{it, key_end};
            if (find(it, key_end, '#') != key_end) {
                throw_parse_exception("Bare key " + key + " cannot contain #");
            }
            if (find_if(it, key_end, [](char c) { return c == ' ' || c == '\t'; }) != key_end) {
                throw_parse_exception("Bare key " + key + " cannot contain whitespace");
            }
            if (find_if(it, key_end, [](char c) { return c == '[' || c == ']'; }) != key_end) {
                throw_parse_exception("Bare key " + key + " cannot contain '[' or ']'");
            }
            it = end;
            return key;
        }

        //----
        shared_ptr<CTomlBase> parse_value(striter_t& it, striter_t& end) {
            parse_t type = determine_value_type(it, end);
            switch (type) {
                case parse_t::STRING:
                    return parse_string(it, end);
                case parse_t::LOCAL_TIME:
                    return parse_time(it, end);
                case parse_t::LOCAL_DATE:
                case parse_t::LOCAL_DATETIME:
                case parse_t::OFFSET_DATETIME:
                    return parse_date(it, end);
                case parse_t::INT:
                case parse_t::FLOAT:
                    return parse_number(it, end);
                case parse_t::BOOL:
                    return parse_bool(it, end);
                case parse_t::ARRAY:
                    return parse_array(it, end);
                case parse_t::INLINE_TABLE:
                    return parse_inline_table(it, end);
                default:
                    throw_parse_exception("Failed to parse value");
            }
            return nullptr;
        }

        //----
        parse_t determine_value_type(const striter_t& it, const striter_t& end) {
            if (it == end) {
                throw_parse_exception("Failed to parse value type");
            }
            if (*it == '"' || *it == '\'') {
                return parse_t::STRING;
            } else if (is_time(it, end)) {
                return parse_t::LOCAL_TIME;
            } else if (auto dtype = date_type(it, end)) {
                return *dtype;
            } else if (is_number(*it) || *it == '-' || *it == '+'
                     || (*it == 'i' && it + 1 != end && it[1] == 'n'
                         && it + 2 != end && it[2] == 'f')
                     || (*it == 'n' && it + 1 != end && it[1] == 'a'
                         && it + 2 != end && it[2] == 'n')) {
                return determine_number_type(it, end);
            } else if (*it == 't' || *it == 'f') {
                return parse_t::BOOL;
            } else if (*it == '[') {
                return parse_t::ARRAY;
            } else if (*it == '{') {
                return parse_t::INLINE_TABLE;
            }
            throw_parse_exception("Failed to parse value type");
            return parse_t::INVALID_TYPE;
        }

        //----
        parse_t determine_number_type(const striter_t& it, const striter_t& end) {
            // determine if we are an integer or a float
            auto check_it = it;
            if (*check_it == '-' || *check_it == '+')
                ++check_it;
            if (check_it == end)
                throw_parse_exception("Malformed number");
            if (*check_it == 'i' || *check_it == 'n')
                return parse_t::FLOAT;
            while (check_it != end && is_number(*check_it))
                ++check_it;
            if (check_it != end && *check_it == '.') {
                ++check_it;
                while (check_it != end && is_number(*check_it))
                    ++check_it;
                return parse_t::FLOAT;
            } else {
                return parse_t::INT;
            }
            return parse_t::INVALID_TYPE;
        }

        //----
        strvalptr_t parse_string(striter_t& it, striter_t& end) {
            auto delim = *it;
            assert(delim == '"' || delim == '\'');

            // end is non-const here because we have to be able to potentially
            // parse multiple lines in a string, not just one
            auto check_it = it;
            ++check_it;
            if (check_it != end && *check_it == delim) {
                ++check_it;
                if (check_it != end && *check_it == delim) {
                    it = ++check_it;
                    return parse_multiline_string(it, end, delim);
                }
            }
            return make_value<string>(string_literal(it, end, delim));
        }

        //----
        strvalptr_t parse_multiline_string(striter_t& it, striter_t& end, char delim) {
            stringstream ss;
            auto is_ws = [](char c) { return c == ' ' || c == '\t'; };
            bool consuming = false;
            strvalptr_t ret;

            auto handle_line = [&](striter_t& local_it, striter_t& local_end) {
                if (consuming) {
                    local_it = find_if_not(local_it, local_end, is_ws);
                    // whole line is whitespace
                    if (local_it == local_end)
                        return;
                }

                consuming = false;
                while (local_it != local_end) {
                    // handle escaped characters
                    if (delim == '"' && *local_it == '\\') {
                        auto check = local_it;
                        // check if this is an actual escape sequence or a
                        // whitespace escaping backslash
                        ++check;
                        consume_whitespace(check, local_end);
                        if (check == local_end) {
                            consuming = true;
                            break;
                        }
                        ss << parse_escape_code(local_it, local_end);
                        continue;
                    }

                    // if we can end the string
                    if (distance(local_it, local_end) >= 3) {
                        auto check = local_it;
                        // check for """
                        if (*check++ == delim && *check++ == delim && *check++ == delim) {
                            local_it = check;
                            ret = make_value<string>(ss.str());
                            break;
                        }
                    }
                    ss << *local_it++;
                }
            };

            // handle the remainder of the current line
            handle_line(it, end);
            if (ret)
                return ret;

            // start eating lines
            while (detail::getline(input_, line_)) {
                ++line_number_;
                it = line_.begin();
                end = line_.end();
                handle_line(it, end);
                if (ret)
                    return ret;
                if (!consuming)
                    ss << endl;
            }
            throw_parse_exception("Unterminated multi-line basic string");
            return nullptr;
        }

        //----
        string string_literal(striter_t& it, const striter_t& end, char delim) {
            ++it;
            string val;
            while (it != end) {
                // handle escaped characters
                if (delim == '"' && *it == '\\') {
                    val += parse_escape_code(it, end);
                } else if (*it == delim) {
                    ++it;
                    consume_whitespace(it, end);
                    return val;
                } else {
                    val += *it++;
                }
            }
            throw_parse_exception("Unterminated string literal");
            return nullptr;
        }

        //----
        string parse_escape_code(striter_t& it, const striter_t& end) {
            ++it;
            if (it == end)
                throw_parse_exception("Invalid escape sequence");
            char value = 0;
            if (*it == 'b') {
                value = '\b';
            } else if (*it == 't') {
                value = '\t';
            } else if (*it == 'n') {
                value = '\n';
            } else if (*it == 'f') {
                value = '\f';
            } else if (*it == 'r') {
                value = '\r';
            } else if (*it == '"') {
                value = '"';
            } else if (*it == '\\') {
                value = '\\';
            } else if (*it == 'u' || *it == 'U') {
                return parse_unicode(it, end);
            } else {
                throw_parse_exception("Invalid escape sequence");
            }
            ++it;
            return string(1, value);
        }

        //----
        string parse_unicode(striter_t& it, const striter_t& end) {
            bool large = *it++ == 'U';
            auto codepoint = parse_hex(it, end, large ? 0x10000000 : 0x1000);

            if ((codepoint > 0xd7ff && codepoint < 0xe000) || codepoint > 0x10ffff) {
                throw_parse_exception("Unicode escape sequence is not a Unicode scalar value");
            }

            string result;
            // See Table 3-6 of the Unicode standard
            if (codepoint <= 0x7f) {
                // 1-byte codepoints: 00000000 0xxxxxxx
                // repr: 0xxxxxxx
                result += static_cast<char>(codepoint & 0x7f);
            } else if (codepoint <= 0x7ff) {
                // 2-byte codepoints: 00000yyy yyxxxxxx
                // repr: 110yyyyy 10xxxxxx
                //
                // 0x1f = 00011111
                // 0xc0 = 11000000
                //
                result += static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f));
                //
                // 0x80 = 10000000
                // 0x3f = 00111111
                //
                result += static_cast<char>(0x80 | (codepoint & 0x3f));
            } else if (codepoint <= 0xffff) {
                // 3-byte codepoints: zzzzyyyy yyxxxxxx
                // repr: 1110zzzz 10yyyyyy 10xxxxxx
                //
                // 0xe0 = 11100000
                // 0x0f = 00001111
                //
                result += static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f));
                result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x1f));
                result += static_cast<char>(0x80 | (codepoint & 0x3f));
            } else {
                // 4-byte codepoints: 000uuuuu zzzzyyyy yyxxxxxx
                // repr: 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
                //
                // 0xf0 = 11110000
                // 0x07 = 00000111
                //
                result += static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07));
                result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f));
                result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f));
                result += static_cast<char>(0x80 | (codepoint & 0x3f));
            }
            return result;
        }

        //----
        uint32_t parse_hex(striter_t& it, const striter_t& end, uint32_t place) {
            uint32_t value = 0;
            while (place > 0) {
                if (it == end)
                    throw_parse_exception("Unexpected end of unicode sequence");
                if (!is_hex(*it))
                    throw_parse_exception("Invalid unicode escape sequence");
                value += place * hex_to_digit(*it++);
                place /= 16;
            }
            return value;
        }

        //----
        uint32_t hex_to_digit(char c) {
            if (is_number(c))
                return static_cast<uint32_t>(c - '0');
            return 10 + static_cast<uint32_t>(c - ((c >= 'a' && c <= 'f') ? 'a' : 'A'));
        }

        //----
        shared_ptr<CTomlBase> parse_number(striter_t& it, const striter_t& end) {
            auto check_it = it;
            auto check_end = find_end_of_number(it, end);
            auto eat_sign = [&]() {
                if (check_it != end && (*check_it == '-' || *check_it == '+'))
                    ++check_it;
            };
            auto check_no_leading_zero = [&]() {
                if (check_it != end && *check_it == '0' && check_it + 1 != check_end && check_it[1] != '.') {
                    throw_parse_exception("Numbers may not have leading zeros");
                }
            };

            auto eat_digits = [&](bool (*check_char)(char)) {
                auto beg = check_it;
                while (check_it != end && check_char(*check_it)) {
                    ++check_it;
                    if (check_it != end && *check_it == '_') {
                        ++check_it;
                        if (check_it == end || !check_char(*check_it))
                            throw_parse_exception("Malformed number");
                    }
                }
                if (check_it == beg)
                    throw_parse_exception("Malformed number");
            };

            auto eat_hex = [&]() { eat_digits(&is_hex); };
            auto eat_numbers = [&]() { eat_digits(&is_number); };

            if (check_it != end && *check_it == '0' && check_it + 1 != check_end
                && (check_it[1] == 'x' || check_it[1] == 'o' || check_it[1] == 'b'))
            {
                ++check_it;
                char CTomlBase = *check_it;
                ++check_it;
                if (CTomlBase == 'x') {
                    eat_hex();
                    return parse_int(it, check_it, 16);
                } else if (CTomlBase == 'o') {
                    auto start = check_it;
                    eat_numbers();
                    auto val = parse_int(start, check_it, 8, "0");
                    it = start;
                    return val;
                } else {
                    auto start = check_it;
                    eat_numbers();
                    auto val = parse_int(start, check_it, 2);
                    it = start;
                    return val;
                }
            }

            eat_sign();
            check_no_leading_zero();

            if (check_it != end && check_it + 1 != end && check_it + 2 != end) {
                if (check_it[0] == 'i' && check_it[1] == 'n' && check_it[2] == 'f') {
                    auto val = numeric_limits<double>::infinity();
                    if (*it == '-')
                        val = -val;
                    it = check_it + 3;
                    return make_value(val);
                } else if (check_it[0] == 'n' && check_it[1] == 'a' && check_it[2] == 'n') {
                    auto val = numeric_limits<double>::quiet_NaN();
                    if (*it == '-')
                        val = -val;
                    it = check_it + 3;
                    return make_value(val);
                }
            }

            eat_numbers();
            if (check_it != end && (*check_it == '.' || *check_it == 'e' || *check_it == 'E')) {
                bool is_exp = *check_it == 'e' || *check_it == 'E';
                ++check_it;
                if (check_it == end)
                    throw_parse_exception("Floats must have trailing digits");
                auto eat_exp = [&]() {
                    eat_sign();
                    check_no_leading_zero();
                    eat_numbers();
                };

                if (is_exp)
                    eat_exp();
                else
                    eat_numbers();

                if (!is_exp && check_it != end && (*check_it == 'e' || *check_it == 'E')) {
                    ++check_it;
                    eat_exp();
                }

                return parse_float(it, check_it);
            } else {
                return parse_int(it, check_it);
            }
        }

        //----
        shared_ptr<CTomlValue<int64_t>> parse_int(striter_t& it, const striter_t& end, int CTomlBase = 10, const char* prefix = "") {
            string v{it, end};
            v = prefix + v;
            v.erase(remove(v.begin(), v.end(), '_'), v.end());
            it = end;
            try {
                return make_value<int64_t>(stoll(v, nullptr, CTomlBase));
            } catch (const invalid_argument& ex) {
                throw_parse_exception("Malformed number (invalid argument: "
                                      + string{ex.what()} + ")");
            } catch (const out_of_range& ex) {
                throw_parse_exception("Malformed number (out of range: "
                                      + string{ex.what()} + ")");
            }
            return nullptr;
        }

        //----
        shared_ptr<CTomlValue<double>> parse_float(striter_t& it, const striter_t& end) {
            string v{it, end};
            v.erase(remove(v.begin(), v.end(), '_'), v.end());
            it = end;
            char decimal_point = '.'; //localeconv()->decimal_point[0];
            replace(v.begin(), v.end(), '.', decimal_point);
            try {
                return make_value<double>(stod(v));

            } catch (const invalid_argument& ex) {
                throw_parse_exception("Malformed number (invalid argument: "
                                     + string{ex.what()} + ")");
            } catch (const out_of_range& ex) {
                throw_parse_exception("Malformed number (out of range: "
                                      + string{ex.what()} + ")");
            }
            return nullptr;
        }

        //----
        shared_ptr<CTomlValue<bool>> parse_bool(striter_t& it, const striter_t& end) {
            auto eat = make_consumer(it, end, [this]() {
                throw_parse_exception("Attempted to parse invalid boolean value");
            });

            if (*it == 't') {
                eat("true");
                return make_value<bool>(true);
            } else if (*it == 'f') {
                eat("false");
                return make_value<bool>(false);
            }
            eat.error();
            return nullptr;
        }

        //----
        striter_t find_end_of_number(striter_t it, striter_t end) {
            auto ret = find_if(it, end, [](char c) {
                return !is_number(c) && c != '_' && c != '.' && c != 'e' && c != 'E'
                && c != '-' && c != '+' && c != 'x' && c != 'o' && c != 'b';
            });
            if (ret != end && ret + 1 != end && ret + 2 != end)
            {
                if ((ret[0] == 'i' && ret[1] == 'n' && ret[2] == 'f')
                    || (ret[0] == 'n' && ret[1] == 'a' && ret[2] == 'n'))
                {
                    ret = ret + 3;
                }
            }
            return ret;
        }

        //----
        striter_t find_end_of_date(striter_t it, striter_t end) {
            auto end_of_date = find_if(it, end, [](char c) {
                return !is_number(c) && c != '-';
            });
            if (end_of_date != end && *end_of_date == ' ' && end_of_date + 1 != end
                && is_number(end_of_date[1]))
                end_of_date++;
            return find_if(end_of_date, end, [](char c) {
                return !is_number(c) && c != 'T' && c != 'Z' && c != ':'
                && c != '-' && c != '+' && c != '.';
            });
        }

        //----
        striter_t find_end_of_time(striter_t it, striter_t end) {
            return find_if(it, end, [](char c) {
                return !is_number(c) && c != ':' && c != '.';
            });
        }

        //----
        loc_time_t read_time(striter_t& it, const striter_t& end) {
            auto time_end = find_end_of_time(it, end);
            auto eat = make_consumer(it, time_end, [&]() { throw_parse_exception("Malformed time"); });

            loc_time_t ltime;

            ltime.hour = eat.eat_digits(2); eat(':');
            ltime.minute = eat.eat_digits(2); eat(':');
            ltime.second = eat.eat_digits(2);

            int power = 100000;
            if (it != time_end && *it == '.') {
                ++it;
                while (it != time_end && is_number(*it)) {
                    ltime.microsecond += power * (*it++ - '0');
                    power /= 10;
                }
            }

            if (it != time_end)
                throw_parse_exception("Malformed time");

            return ltime;
        }

        //----
        shared_ptr<CTomlValue<loc_time_t>>
        parse_time(striter_t& it, const striter_t& end) {
            return make_value(read_time(it, end));
        }

        //----
        shared_ptr<CTomlBase> parse_date(striter_t& it, const striter_t& end) {
            auto date_end = find_end_of_date(it, end);
            auto eat = make_consumer(it, date_end, [&]() { throw_parse_exception("Malformed date"); });

            loc_date_t ldate;
            ldate.year = eat.eat_digits(4);
            eat('-');
            ldate.month = eat.eat_digits(2);
            eat('-');
            ldate.day = eat.eat_digits(2);

            if (it == date_end)
                return make_value(ldate);

            eat.eat_or('T', ' ');

            loc_datetime_t ldt;
            static_cast<loc_date_t&>(ldt) = ldate;
            static_cast<loc_time_t&>(ldt) = read_time(it, date_end);

            if (it == date_end)
                return make_value(ldt);

            off_datetime_t dt;
            static_cast<loc_datetime_t&>(dt) = ldt;

            int hoff = 0;
            int moff = 0;
            if (*it == '+' || *it == '-') {
                auto plus = *it == '+';
                ++it;

                hoff = eat.eat_digits(2);
                dt.hour_offset = (plus) ? hoff : -hoff;
                eat(':');
                moff = eat.eat_digits(2);
                dt.minute_offset = (plus) ? moff : -moff;

            } else if (*it == 'Z') {
                ++it;
            }

            if (it != date_end)
                throw_parse_exception("Malformed date");

            return make_value(dt);
        }

        //----
        shared_ptr<CTomlBase> parse_array(striter_t& it, striter_t& end) {
            // this gets ugly because of the "homogeneity" restriction:
            // arrays can either be of only one type, or contain arrays
            // (each of those arrays could be of different types, though)
            //
            // because of the latter portion, we don't really have a choice
            // but to represent them as arrays of CTomlBase values...
            ++it;

            // ugh---have to read the first value to determine array type...
            skip_whitespace_and_comments(it, end);

            // edge case---empty array
            if (*it == ']') {
                ++it;
                return make_array();
            }

            auto val_end = find_if(it, end, [](char c) { return c == ',' || c == ']' || c == '#'; });
            parse_t type = determine_value_type(it, val_end);
            switch (type) {
                case parse_t::STRING:
                    return parse_value_array<string>(it, end);
                case parse_t::LOCAL_TIME:
                    return parse_value_array<loc_time_t>(it, end);
                case parse_t::LOCAL_DATE:
                    return parse_value_array<loc_date_t>(it, end);
                case parse_t::LOCAL_DATETIME:
                    return parse_value_array<loc_datetime_t>(it, end);
                case parse_t::OFFSET_DATETIME:
                    return parse_value_array<off_datetime_t>(it, end);
                case parse_t::INT:
                    return parse_value_array<int64_t>(it, end);
                case parse_t::FLOAT:
                    return parse_value_array<double>(it, end);
                case parse_t::BOOL:
                    return parse_value_array<bool>(it, end);
                case parse_t::ARRAY:
                    return parse_object_array<CTomlArray>(&parser::parse_array, '[', it, end);
                case parse_t::INLINE_TABLE:
                    return parse_object_array<CTomlTableArray>(&parser::parse_inline_table, '{', it, end);
                default:
                    throw_parse_exception("Unable to parse array");
            }
            return nullptr;
        }

        //----
        template <class Value>
        shared_ptr<CTomlArray> parse_value_array(striter_t& it, striter_t& end) {
            auto arr = make_array();
            while (it != end && *it != ']') {
                auto val = parse_value(it, end);
                if (auto v = val->as<Value>())
                    arr->get().push_back(val);
                else
                    throw_parse_exception("Arrays must be homogeneous");
                skip_whitespace_and_comments(it, end);
                if (*it != ',')
                    break;
                ++it;
                skip_whitespace_and_comments(it, end);
            }
            if (it != end)
                ++it;
            return arr;
        }

        //----
        template <class Object, class Function>
        shared_ptr<Object> parse_object_array(Function&& fun, char delim, striter_t& it, striter_t& end) {
            auto arr = detail::make_element<Object>();
            while (it != end && *it != ']') {
                if (*it != delim)
                    throw_parse_exception("Unexpected character in array");
                arr->get().push_back(((*this).*fun)(it, end));
                skip_whitespace_and_comments(it, end);
                if (it == end || *it != ',')
                    break;
                ++it;
                skip_whitespace_and_comments(it, end);
            }
            if (it == end || *it != ']')
                throw_parse_exception("Unterminated array");
            ++it;
            return arr;
        }

        //----
        shared_ptr<CTomlTable> parse_inline_table(striter_t& it, striter_t& end) {
            auto tbl = makeTable();
            do {
                ++it;
                if (it == end)
                    throw_parse_exception("Unterminated inline table");
                consume_whitespace(it, end);
                if (it != end && *it != '}') {
                    parse_key_value(it, end, tbl.get());
                    consume_whitespace(it, end);
                }
            } while (*it == ',');

            if (it == end || *it != '}')
                throw_parse_exception("Unterminated inline table");
            ++it;
            consume_whitespace(it, end);

            return tbl;
        }

        //----
        void skip_whitespace_and_comments(striter_t& start, striter_t& end) {
            consume_whitespace(start, end);
            while (start == end || *start == '#') {
                if (!detail::getline(input_, line_))
                    throw_parse_exception("Unclosed array");
                line_number_++;
                start = line_.begin();
                end = line_.end();
                consume_whitespace(start, end);
            }
        }

        //----
        void consume_whitespace(striter_t& it, const striter_t& end) {
            while (it != end && (*it == ' ' || *it == '\t'))
                ++it;
        }

        //----
        void consume_backwards_whitespace(striter_t& back, const striter_t& front) {
            while (back != front && (*back == ' ' || *back == '\t'))
                --back;
        }

        //----
        void eol_or_comment(const striter_t& it, const striter_t& end) {
            if (it != end && *it != '#') {
                throw_parse_exception("Unidentified trailing character '"
                                      + string{*it}
                                      + "'---did you forget a '#'?");
            }
        }

        //----
        bool is_time(const striter_t& it, const striter_t& end) {
            auto time_end = find_end_of_time(it, end);
            auto len = distance(it, time_end);

            if (len < 8)
                return false;

            if (it[2] != ':' || it[5] != ':')
                return false;

            if (len > 8)
                return it[8] == '.' && len > 9;

            return true;
        }

        //----
        option<parse_t> date_type(const striter_t& it, const striter_t& end) {
            auto date_end = find_end_of_date(it, end);
            auto len = distance(it, date_end);

            if (len < 10)
                return {};

            if (it[4] != '-' || it[7] != '-')
                return {};

            if (len >= 19 && (it[10] == 'T' || it[10] == ' ') && is_time(it + 11, date_end)) {
                auto time_end = find_end_of_time(it + 11, date_end);
                if (time_end == date_end)
                    return {parse_t::LOCAL_DATETIME};
                else
                    return {parse_t::OFFSET_DATETIME};

            } else if (len == 10) {
                return {parse_t::LOCAL_DATE};
            }

            return {};
        }

        //----
        istream& input_;
        string line_;
        size_t line_number_ = 0;
    };

    /**
     * Utility function to parse a file as a TOML file. Returns the root table.
     * Throws a parse_exception if the file cannot be opened.
     */
    inline shared_ptr<CTomlTable> parse_file(const string& filename) {
        ifstream file{filename};
        if (!file.is_open())
            throw parse_exception{filename + " could not be opened for parsing"};
        parser p{file};
        return p.parse();
    }

    template <class... Ts>
    struct value_accept;

    template <>
    struct value_accept<> {
        template <class Visitor, class... Args>
        static void accept(const CTomlBase&, Visitor&&, Args&&...) { }
    };

    template <class T, class... Ts>
    struct value_accept<T, Ts...> {
        template <class Visitor, class... Args>
        static void accept(const CTomlBase& b, Visitor&& visitor, Args&&... args) {
            if (auto v = b.as<T>()) {
                visitor.visit(*v, forward<Args>(args)...);
            } else {
                value_accept<Ts...>::accept(b, forward<Visitor>(visitor),
                                            forward<Args>(args)...);
            }
        }
    };

    /**
     * CTomlBase implementation of accept() that calls visitor.visit() on the concrete
     * class.
     */
    template <class Visitor, class... Args>
    void CTomlBase::accept(Visitor&& visitor, Args&&... args) const {
        if (isValue()) {
            using value_acceptor = value_accept<string, int64_t, double, bool, loc_date_t,
            loc_time_t, loc_datetime_t, off_datetime_t>;
            value_acceptor::accept(*this, forward<Visitor>(visitor), forward<Args>(args)...);

        } else if (isTable()) {
            visitor.visit(static_cast<const CTomlTable&>(*this), forward<Args>(args)...);

        } else if (isArray()) {
            visitor.visit(static_cast<const CTomlArray&>(*this), forward<Args>(args)...);

        } else if (isTableArray()) {
            visitor.visit(static_cast<const CTomlTableArray&>(*this), forward<Args>(args)...);
        }
    }

    //-----------------------------------------------------------------
    class CTomlWriter {
    public:
        CTomlWriter(ostream& s, const string& indent_space = "\t")
          : stream_(s), indent_(indent_space), has_naked_endline_(false) { }

    public:
        template <class T>
        void visit(const CTomlValue<T>& v, bool = false) {
            write(v);
        }

        void visit(const CTomlTable& t, bool in_array = false) {
            write_table_header(in_array);
            CStringArray values;
            CStringArray tables;

            for (const auto& i : t) {
                if (i.second->isTable() || i.second->isTableArray()) {
                    tables.push_back(i.first);
                } else {
                    values.push_back(i.first);
                }
            }

            for (unsigned int i = 0; i < values.size(); ++i) {
                path_.push_back(values[i]);
                if (i > 0)
                    endline();
                write_table_item_header(*t.get(values[i]));
                t.get(values[i])->accept(*this, false);
                path_.pop_back();
            }

            for (unsigned int i = 0; i < tables.size(); ++i) {
                path_.push_back(tables[i]);
                if (values.size() > 0 || i > 0)
                    endline();
                write_table_item_header(*t.get(tables[i]));
                t.get(tables[i])->accept(*this, false);
                path_.pop_back();
            }
            endline();
        }

        void visit(const CTomlArray& a, bool = false) {
            write("[");
            for (unsigned int i = 0; i < a.get().size(); ++i) {
                if (i > 0)
                    write(", ");
                if (a.get()[i]->isArray()) {
                    a.get()[i]->as_array()->accept(*this, true);
                } else {
                    a.get()[i]->accept(*this, true);
                }
            }
            write("]");
        }

        void visit(const CTomlTableArray& t, bool = false) {
            for (unsigned int j = 0; j < t.get().size(); ++j) {
                if (j > 0)
                    endline();
                t.get()[j]->accept(*this, true);
            }
            endline();
        }

        static string escape_string(const string& str) {
            string res;
            for (auto it = str.begin(); it != str.end(); ++it) {
                if (*it == '\b') {
                    res += "\\b";
                } else if (*it == '\t') {
                    res += "\\t";
                } else if (*it == '\n') {
                    res += "\\n";
                } else if (*it == '\f') {
                    res += "\\f";
                } else if (*it == '\r') {
                    res += "\\r";
                } else if (*it == '"') {
                    res += "\\\"";
                } else if (*it == '\\') {
                    res += "\\\\";
                } else if (static_cast<uint32_t>(*it) <= UINT32_C(0x001f)) {
                    res += "\\u";
                    stringstream ss;
                    ss << hex << static_cast<uint32_t>(*it);
                    res += ss.str();
                } else {
                    res += *it;
                }
            }
            return res;
        }

    protected:
        void write(const strval_t& v) {
            write("\"");
            write(escape_string(v.get()));
            write("\"");
        }

        void write(const CTomlValue<double>& v) {
            stringstream ss;
            ss << showpoint
            << setprecision(numeric_limits<double>::max_digits10)
            << v.get();
            auto double_str = ss.str();
            auto pos = double_str.find("e0");
            if (pos != string::npos)
                double_str.replace(pos, 2, "e");
            pos = double_str.find("e-0");
            if (pos != string::npos)
                double_str.replace(pos, 3, "e-");
            stream_ << double_str;
            has_naked_endline_ = false;
        }

        template <class T>
        typename enable_if<
        is_one_of<T, int64_t, loc_date_t, loc_time_t, loc_datetime_t, off_datetime_t>::value>::type
        write(const CTomlValue<T>& v) {
            write(v.get());
        }

        void write(const CTomlValue<bool>& v) {
            write((v.get() ? "true" : "false"));
        }

        void write_table_header(bool in_array = false) {
            if (!path_.empty()) {
                indent();
                write("[");
                if (in_array) {
                    write("[");
                }

                for (unsigned int i = 0; i < path_.size(); ++i) {
                    if (i > 0) {
                        write(".");
                    }

                    if (path_[i].find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-")
                        == string::npos) {
                        write(path_[i]);
                    } else {
                        write("\"");
                        write(escape_string(path_[i]));
                        write("\"");
                    }
                }

                if (in_array)
                {
                    write("]");
                }

                write("]");
                endline();
            }
        }

        void write_table_item_header(const CTomlBase& b) {
            if (!b.isTable() && !b.isTableArray()) {
                indent();
                if (path_.back().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-")
                    == string::npos) {
                    write(path_.back());
                } else {
                    write("\"");
                    write(escape_string(path_.back()));
                    write("\"");
                }

                write(" = ");
            }
        }

    private:
        void indent() {
            for (size_t i = 1; i < path_.size(); ++i)
                write(indent_);
        }
        template <class T>
        void write(const T& v) {
            stream_ << v;
            has_naked_endline_ = false;
        }
        void endline() {
            if (!has_naked_endline_) {
                stream_ << "\n";
                has_naked_endline_ = true;
            }
        }

    private:
        ostream& stream_;
        const string indent_;
        CStringArray path_;
        bool has_naked_endline_;
    };

    inline ostream& operator<<(ostream& stream, const CTomlBase& b) {
        CTomlWriter writer{stream};
        b.accept(writer);
        return stream;
    }

    template <class T>
    ostream& operator<<(ostream& stream, const CTomlValue<T>& v) {
        CTomlWriter writer{stream};
        v.accept(writer);
        return stream;
    }

    inline ostream& operator<<(ostream& stream, const CTomlTable& t) {
        CTomlWriter writer{stream};
        t.accept(writer);
        return stream;
    }

    inline ostream& operator<<(ostream& stream, const CTomlTableArray& t) {
        CTomlWriter writer{stream};
        t.accept(writer);
        return stream;
    }

    inline ostream& operator<<(ostream& stream, const CTomlArray& a) {
        CTomlWriter writer{stream};
        a.accept(writer);
        return stream;
    }

    class CNewToml {
    public:
        CNewToml(const string_q& path) {
            toml = parse_file(path);
        }
    public:
        shared_ptr<CTomlTable> toml;
    };

    inline ostream& operator<<(ostream& stream, const CNewToml& a) {
        stream << *a.toml;
        return stream;
    }

} // namespace cpptoml
using namespace cpptoml;
