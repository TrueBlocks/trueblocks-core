#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_list_remote(void) {
    if (!freshen_from_remote())
        return usage("Could not freshen remote pin data");
    cout << asciiFileToString(configPath("manifest/manifest_remote.txt"));
    return true;  // do not continue
}

//----------------------------------------------------------------
bool COptions::freshen_from_remote(void) {
    string_q location = doCommand("chifra state --call " + unchainedIndexAddrV3 + "!" + manifestHashEncoding);
    // cout << "0: " << location;
    location = substitute(substitute(location, "\"", "+"), " ", "");
    // cout << "1: " << location;
    location = substitute(location, "manifestHash+:+", "|");
    // cout << "A: " << location;
    nextTokenClear(location, '|');
    // cout << "B: " << location;
    location = nextTokenClear(location, '+');
    string_q remoteData = doCommand("curl -s \"http://gateway.ipfs.io/ipfs/" + location + "\"");

    string fn = configPath("manifest/manifest_remote.txt");
    stringToAsciiFile(fn, remoteData);
    return fileExists(fn);
}
