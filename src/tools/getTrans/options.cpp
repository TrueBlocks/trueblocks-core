/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~!trans_list",    "a space-separated list of one or more transaction identifiers "
                                  "(tx_hash, bn.txID, blk_hash.txID)"),
    COption("-raw",            "retrieve raw transaction directly from the running node"),
    COption("-trace",          "display the transaction's trace"),
    COption("-articulate",     "articulate the transactions if an ABI is found"),
    COption("@belongs:<addr>", "report true or false if the given address is found anywhere in the transaction"),
    COption("@asStrs",         "when checking --belongs, treat input and log data as a string"),
    COption("",                "Retrieve an Ethereum transaction from the local cache or a running node."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-r" || arg == "--raw") {
            isRaw = true;

        } else if (arg == "-t" || arg == "--trace") {
            incTrace = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;
            verbose = true;

        } else if (arg == "--asStrs") {
            chkAsStr = true;

        } else if (startsWith(arg, "--belongs:")) {
            string_q orig = arg;
            arg = substitute(arg, "--belongs:", "");
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address.\n");
            filters.push_back(str_2_Addr(toLower(arg)));

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (!wrangleTxId(arg))
                return false;
            string_q ret = transList.parseTransList(arg);
            if (!ret.empty())
                return usage(ret);
        }
    }

    if (articulate) {
        // show certain fields and hide others
        manageFields(defHide, false);
        manageFields(defShow, true);
        manageFields("CParameter:strDefault", false);  // hide
        manageFields("CTransaction:price", false);  // hide
        manageFields("CFunction:outputs", true);  // show
        if (verbose) {
            manageFields("CTransaction:input", true);  // show
            manageFields("CLogEntry:topics", true);  // show
        }

        //    manageFields(toml.getConfigStr("fields", "hide", ""), false);
        //    manageFields(toml.getConfigStr("fields", "show", ""), true );
        abi_spec.loadAbiKnown("all");
    }

    if (!transList.hasTrans())
        return usage("Please specify at least one transaction identifier.");

extern const char* STR_DISPLAY_FORMAT;
    format = getGlobalConfig("getTrans")->getDisplayStr(!verbose, (verbose ? "" : STR_DISPLAY_FORMAT));

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    transList.Init();
    isRaw = false;
    incTrace = false;
    articulate = false;
    format = "";
    filters.clear();
    belongs = false;
    chkAsStr = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);
    HIDE_FIELD(CTransaction, "cumulativeGasUsed");

    Init();
    index = 0;
    if (isTestMode())
        UNHIDE_FIELD(CTransaction, "isError");
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "trans_list", "<transID> [transID...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{trans_list}] is one or more space-separated identifiers which may be either a transaction hash,|"
                "a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.\n";
        ret += "This tool checks for valid input syntax, but does not check that the transaction requested exists.\n";
        ret += "This tool retrieves information from the local node or rpcProvider if configured "
                    "(see documentation).\n";
        ret += "If the queried node does not store historical state, the results may be undefined.\n";
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
bool COptions::getDirectionalTxId(blknum_t bn, txnum_t txid, const string_q& dir, string_q& argOut) {

    blknum_t lastBlock = getLastBlock_client();

    CBlock block;
    getBlock(block, bn);

    argOut = "";
    txnum_t nextid = txid + 1;
    while (argOut.empty() && bn >= firstTransaction && bn <= lastBlock) {
        if (dir == "next") {
            if (nextid < block.transactions.size()) {
                argOut = uint_2_Str(block.blockNumber) + "." + uint_2_Str(nextid);
                return true;
            }
            block = CBlock();
            getBlock(block, ++bn);
            nextid = 0;
        } else if (dir == "prev") {
            if (txid > 0) {
                argOut = uint_2_Str(block.blockNumber) + "." + uint_2_Str(txid - 1);
                return true;
            }
            if (bn == 0)
                return true;
            block = CBlock();
            getBlock(block, --bn);
            txid = block.transactions.size();
        }
    }
    return usage("Could not find " + dir + " transaction to " + uint_2_Str(bn) + "." + uint_2_Str(txid));
}

//--------------------------------------------------------------------------------
bool COptions::wrangleTxId(string_q& argOut) {

    if (contains(argOut, "0x"))
        return true;

    // valid args are 'latest', 'bn.txid', 'bn.txid.next', or 'bn.txid.prev'
    CStringArray parts;
    explode(parts, argOut, '.');

    //txnum_t txid;
    if (parts.size() == 0 ||  // there are not enough
        (parts.size() == 1 && parts[0] != "latest") ||  // there's only one and it's not 'latest'
        ((parts.size() == 2 || parts.size() == 3) && (!isNumeral(parts[0]) || !isNumeral(parts[1]))) ||  // two or three, first two are not numbers
        parts.size() > 3)  // too many
        return usage(argOut + " does not appear to be a valid transaction index.");

    if (parts.size() == 2)
        return true;

    // it's directional
    if (parts[0] == "latest") {
        CBlock block;
        getBlock(block, "latest");
        if (block.transactions.size() > 0) {
            ostringstream os;
            os << block.blockNumber << "." << (block.transactions.size() - 1);
            argOut = os.str();
            return true;
        }
        parts[0] = uint_2_Str(block.blockNumber);
        parts[1] = "0";
        parts[2] = "prev";
    }
    ASSERT(parts[2] == "prev" || parts[2] == "next");
    return getDirectionalTxId(str_2_Uint(parts[0]), str_2_Uint(parts[1]), parts[2], argOut);
}

//--------------------------------------------------------------------------------
const char* STR_DISPLAY_FORMAT = "[{DATE}][\t{TIMESTAMP}][\t{BLOCKNUMBER}][\t{TRANSACTIONINDEX}][\t{HASH}]\\n";
