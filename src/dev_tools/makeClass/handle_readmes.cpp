/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//------------------------------------------------------------------------------------------------------------
bool visitReadme(const string_q& templatePath, void *data) {
    if (endsWith(templatePath, "/")) {
        return forEveryFileInFolder(templatePath + "*", visitReadme, data);

    } else {
        if (!endsWith(templatePath, ".tmpl.md"))
            return true;

        string_q docPath = substitute(substitute(templatePath, "docs/readme_templates/", "docs/readmes/"), "tmpl.md", "md");
        string_q sourcePath = substitute(docPath, "docs/readmes/", "src/");

        CStringArray parts;
        explode(parts, templatePath, '/');
        int i = 0;
        for (auto part : parts)
            cout << (i++) << " " << part << endl;
        cout << endl;
        // string_q template = asciiFileToString(templatePath);
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_readmes(void) {
    forEveryFileInFolder("../docs/readme_templates", visitReadme, nullptr);
    return true;
}

#if 0
srcFolder = sys.argv[1]
toolFolder = sys.argv[2]
toolName = sys.argv[3]
routeStr = "chifra"

templateFile = toolFolder + "/docs/README.tmpl.md"
footerFile = srcFolder + "/other/docs/README.footer.md"
usageFile = "help.txt"
outputFile = toolFolder + "/README.md"

#printe('templateFile: ', templateFile)
#printe('footerFile: ', footerFile)
#printe('toolName: ', toolName)
#printe('usageFile: ', usageFile)
#printe('outputFile: ', outputFile)
#printe('toolName: ', toolName)
#printe('srcFolder: ', srcFolder)
#printe('toolFolder: ', toolFolder)

#Check that input file is present
if os.path.isfile(templateFile) == False:
    printe("ERROR: Could not find input file %s" % templateFile)
    exit(1)

#Check that footer file is present
if os.path.isfile(footerFile) == False:
    printe("ERROR: Could not find input file %s" % footerFile)
    exit(1)

#Check that content file is present
if os.path.isfile(usageFile) == False:
    printe("ERROR: Could not find content file %s" % usageFile)
    exit(1)

#Check that output file is NOT present, remove it otherwise
if os.path.isfile(outputFile) == True:
#printe("WARNING: Output file already present %s (removed)" % outputFile)
    os.remove(outputFile)

if os.path.isfile(outputFile) == True:
    printe("Weird")

#Get some data to use
with open(templateFile, 'r') as file:
    templateData = file.read()

with open(footerFile, 'r') as file:
    footerData = file.read()

with open(usageFile, 'r') as file:
    usageData = file.read()

#Do the replacements
outputData = templateData.replace("[{USAGE_TABLE}]", usageData)
outputData = outputData.replace("[{FOOTER}]", footerData)
outputData = outputData.replace("[{NAME}]", routeStr)
outputData = outputData.replace("[{TOOL_PATH}]", projName + "/" + toolName)
outputData = outputData.replace("chifra chifra", "chifra")

#printe('outputData: ', outputData)

#Generate output file
with open(outputFile, 'w') as file:
    printe("Wrote file %s" % outputFile)
    file.write(outputData)

map<string, string> cmdMap = {
    {"list", "acctExport --appearances"},
    {"export", "acctExport"},
    {"monitor", "acctExport --appearances"},
    {"names", "ethNames"},
//    {"entities", "ethNames --entities"},
//    {"tags", "ethNames --tags"},
    {"abis", "grabABI"},

    {"blocks", "getBlock"},
    {"transactions", "getTrans"},
    {"receipts", "getReceipts"},
    {"logs", "getLogs"},
    {"traces", "getTrace"},
    {"when", "whenBlock"},

    {"state", "getState"},
    {"tokens", "getTokens"},

    {"init", "pinMan local --init"},
    {"status", "cacheStatus"},
    {"scrape", "blockScrape"},
    {"serve", "flame"},
    {"pins", "pinMan"},

    {"explore", "ethscan.py"},
    {"slurp", "ethslurp"},
    {"quotes", "getQuotes"},
};

"ACCOUNTS|"
"  list          list every appearance of an address anywhere on the chain|"
"  export        export details for each appearance (as txs, logs, traces, balances, reconciliations, etc.)|"
"  monitor       add, remove, clean, and list appearances of address(es) on the chain|"
"  names         list and/or share named addresses|"
"  abis          list and/or share abi signatures|"
"DATA|"
"  blocks        export block-related data|"
"  transactions  export transaction-related data|"
"  receipts      export receipt-related data|"
"  logs          export log-related data|"
"  traces        export trace-related data|"
"  when          return a date given a block number or a block number given a date|"
"STATE|"
"  state         export parts of the state for given address(es)|"
"  tokens        export data related to ERC20 and/or ERC721 token(s)|"
"ADMIN|"
"  init          initialize TrueBlocks databases by downloading pinned bloom filters|"
"  status        query the status of the system|"
"  scrape        scrape the chain and build an index of address appearances (aka digests)|"
"  serve         serve the TrueBlocks API via the flame server|"
"  pins          query the status of the pinning system|"
"OTHER|"
"  explore       open the configured block explorer for the given address|"
"  slurp         export details by querying EtherScan (note: will not return as many appearances as --list)|"
"  quotes        return prices collected from configured remote API"

num   ,group     ,tags     ,api_route    ,tool         ,command,hotkey,def_val,is_required,customizeable,core_visible,docs_visible, generate, option_kind, data_type, description
10460 ,apps      ,Accounts ,export       ,acctExport   ,,,, false, false, true, true  ,-- ,description, ,Export full detail of transactions for one or more Ethereum addresses.
13880 ,tools     ,Accounts ,abis         ,grabABI      ,,,, false, false, true, true  ,-- ,description, ,Fetches the ABI for a smart contract.
12460 ,tools     ,Accounts ,names        ,ethNames     ,,,, false, false, true, true  ,-- ,description, ,Query addresses and/or names of well known accounts.
10660 ,apps      ,Admin    ,scrape       ,blockScrape  ,,,, false, false, true, true  ,-- ,description, ,Decentralized blockchain scraper and block cache.
10860 ,apps      ,Admin    ,status       ,cacheStatus  ,,,, false, false, true, true  ,-- ,description, ,Report on status of one or more TrueBlocks caches.
10980 ,apps      ,Admin    ,pins         ,pinMan       ,,,, false, false, true, true  ,-- ,description, ,Report on and manage the remotely pinned appearance index and associated bloom filters.
12640 ,tools     ,Data     ,blocks       ,getBlock     ,,,, false, false, true, true  ,-- ,description, ,Returns block(s) from local cache or directly from a running node.
12960 ,tools     ,Data     ,logs         ,getLogs      ,,,, false, false, true, true  ,-- ,description, ,Retrieve a transaction's logs from the cache or the node.
13060 ,tools     ,Data     ,receipts     ,getReceipts   ,,,, false, false, true, true  ,-- ,description, ,Retrieve a transaction's receipt from the cache or the node.
13500 ,tools     ,Data     ,traces       ,getTrace     ,,,, false, false, true, true  ,-- ,description, ,Retrieve a transaction's traces from the cache or the node.
13620 ,tools     ,Data     ,transactions ,getTrans     ,,,, false, false, true, true  ,-- ,description, ,Retrieve a transaction from the cache or the node.
13980 ,tools     ,Data     ,when         ,whenBlock    ,,,, false, false, true, false ,-- ,description, ,Finds block based on date&#44; blockNum&#44; timestamp&#44; or 'special'.
13220 ,tools     ,State    ,state        ,getState     ,,,, false, false, true, true  ,-- ,description, ,Retrieve the balance of one or more address at the given block(s).
13360 ,tools     ,State    ,tokens       ,getTokens ,,,, false, false, true, true  ,-- ,description, ,Retrieve token balances for one or more address at given block(s).
12080 ,tools     ,Other    ,quotes       ,getQuotes     ,,,, false, false, true, true  ,-- ,description, ,Freshen and/or display Ethereum price data.
12220 ,tools     ,Other    ,slurp        ,ethslurp     ,,,, false, false, true, true  ,-- ,description, ,Fetches data from EtherScan for an arbitrary address.
11824 ,dev_tools ,         ,             ,makeClass    ,,,, false, false, true, false ,-- ,description, ,Automatically writes C++ for various purposes.
11980 ,dev_tools ,         ,             ,testRunner   ,,,, false, false, true, false ,-- ,description, ,Run TrueBlocks' test cases with options.
#endif
