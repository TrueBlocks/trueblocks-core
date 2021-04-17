#include "pinlib.h"
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

        pinlib_readPinList(options.localPins, true);

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

//----------------------------------------------------------------
// void COptions::handle_pin(void) {
// LOG8("Handle pin: ", pin);

// string_q pins;
// if (!pinlib_getPinList(lic, pins)) {
//     usageStr(pins + ".");
//     return;
// }
// cout << pins << endl;
// }

//----------------------------------------------------------------
// void COptions::handle_unpin(void) {
// LOG8("Handle unpin: ", unpin);

// string_q pins;
// if (!pinlib_getPinList(lic, pins)) {
//     usageStr(pins + ".");
//     return;
// }

// CPinataPinlist pinList;
// pinList.parseJson3(pins);
// while (str_2_Uint(pinList.count) != 0) {
//     cout << pinList.count << endl;
//     for (auto thePin : pinList.rows) {
//         cout << "Unpinning " << thePin.metadata.name << ": " << thePin.ipfs_pin_hash << " ";
//         pinlib_unpinOneFile(thePin.ipfs_pin_hash);
//         usleep(1000000);
//     }
//     usleep(300000);
//     pins = "";
//     pins.clear();
//     pinlib_getPinList(lic, pins);
//     pinList = CPinataPinlist();
//     pinList.parseJson3(pins);
// }
// }
