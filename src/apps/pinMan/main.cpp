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
            break;

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"],
                                   (options.license ? "CPinApiLicense" : "CPinManifest"));

        if (options.license) {
            if (expContext().exportFmt == TXT1 || expContext().exportFmt == CSV1) {
                cout << trim(options.lic.Format(expContext().fmtMap["format"]), '\t') << endl;
            } else {
                indent();
                options.lic.toJson(cout);
                unindent();
            }

        } else {
            LOG_INFO("latestBlock:\t", cGreen,
                     (isTestMode() ? "--latest--" : uint_2_Str(getBlockProgress(BP_CLIENT).client)), cOff);
            LOG_INFO("unchainedIndexAddr:\t", cGreen, unchainedIndexAddr, cOff);
            if (!options.unpin.empty()) {
                options.handle_unpin();

            } else if (!options.pin.empty()) {
                options.handle_pin();

            } else {
                if (options.mode == "onchain") {
                    cout << doCommand("chifra state --call " + unchainedIndexAddr + "!" + manifestHashEncoding);

                } else if (options.mode == "local") {
                    cout << asciiFileToString(configPath("manifest/initial-manifest.json"));

                } else {
                    options.handle_pin();
                }
            }
        }

        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    pinlib_cleanup();

    return 0;
}

//----------------------------------------------------------------
void COptions::handle_pin(void) {
    LOG8("Handle pin: ", pin);

    string_q pins;
    if (!pinlib_getPinList(lic, pins)) {
        usageStr(pins + ".");
        return;
    }
    cout << pins << endl;
}

//----------------------------------------------------------------
void COptions::handle_unpin(void) {
    LOG8("Handle unpin: ", unpin);

    string_q pins;
    if (!pinlib_getPinList(lic, pins)) {
        usageStr(pins + ".");
        return;
    }

    CPinataPinlist pinList;
    pinList.parseJson3(pins);
    while (str_2_Uint(pinList.count) != 0) {
        cout << pinList.count << endl;
        for (auto thePin : pinList.rows) {
            cout << "Unpinning " << thePin.metadata.name << ": " << thePin.ipfs_pin_hash << " ";
            pinlib_unpinOneFile(thePin.ipfs_pin_hash);
            usleep(1000000);
        }
        usleep(300000);
        pins = "";
        pins.clear();
        pinlib_getPinList(lic, pins);
        pinList = CPinataPinlist();
        pinList.parseJson3(pins);
    }
}
