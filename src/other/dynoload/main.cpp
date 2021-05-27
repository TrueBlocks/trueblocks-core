// From https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html

#include "acctlib.h"

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
    CDynamicTraverser lib(fileName);
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
