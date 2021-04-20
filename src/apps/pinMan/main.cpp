#include "options.h"

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    pinlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        pinlib_readPinList(options.localPins, options.list.empty() || options.list == "local");

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], "CPinnedChunk");

        if (options.init) {
            return options.handle_init();

        } else if (options.compare) {
            options.handle_compare();

        } else {
            ASSERT(!options.list.empty());
            if (options.list == "local") {
                options.handle_list_local();

            } else {
                ASSERT(options.list == "remote");
                options.handle_list_remote();
            }
        }
        once = false;
    }

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    pinlib_cleanup();

    return 0;
}
