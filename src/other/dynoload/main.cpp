// From https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html

#include "acctlib.h"

class TraverserObj {
private:
    void* m_handle;
    string_q m_path;
    
public:
    explicit TraverserObj(const string_q& lib_path) {
        m_handle = ::dlopen(lib_path.c_str(), RTLD_LAZY);
        m_path = lib_path;
        if (m_handle == nullptr)
            LOG_ERR("Failed to load shared object " + lib_path + ": " + string_q(::dlerror()));
    }
    ~TraverserObj() {
        if (m_handle != nullptr) {
            if (::dlclose(m_handle))  // returns non-zero on failure
                LOG_ERR("Failed to unload the shared object: " + string_q(::dlerror()));
        }
    }
    template <typename FunctionSignature>
    FunctionSignature* get_function(const string_q& func_name) {
        void* func_ptr = ::dlsym(m_handle, func_name.c_str());
        if (func_ptr == nullptr)
            LOG_ERR("Failed to get [func_name:" + func_name + "]: " + ::dlerror());
        return reinterpret_cast<FunctionSignature*>(func_ptr);
    }
    bool is_valid(void) const { return m_handle != nullptr; }
private:
    TraverserObj(void) = delete;
    TraverserObj(const TraverserObj&) = delete;
    TraverserObj& operator=(const TraverserObj&) = delete;
};

class COptions : public COptionsBase {
public:
    COptions(void) {}
    bool parseArguments(string_q &command) override { return true; }
    void Init() override {}
};

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    string fileName = "libshared.dylib";
    TraverserObj lib(fileName);
    if (lib.is_valid()) {
        cout << "Library loaded" << endl;
        auto factory = lib.get_function<CTraverser*(void)>("makeTraverser");
        CTraverser trav = *factory();
        CAppearanceArray_base apps;
        apps.push_back(CAppearance_base(10001001,0));
        CTraverserArray travs;
        travs.push_back(trav);
        forEveryAppearance(travs, apps, &options);
    }

    etherlib_cleanup();
    return 1;
}
