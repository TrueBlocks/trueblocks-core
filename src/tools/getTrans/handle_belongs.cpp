/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

/*
#ifdef BELONGS
CAddressArray filters;
bool belongs;
bool chkAsStr;

////////////////////////////////////////////////////////////
COption("belongs", "b", "<addr>", OPT_HIDDEN | OPT_FLAG,
        "report true or false if the given address is found anywhere in the transaction"),
    COption("asStrs", "a", "", OPT_HIDDEN | OPT_SWITCH,
            "when checking --belongs, treat input and log data as a string"),

////////////////////////////////////////////////////////////
} else if (arg == "--asStrs") {
    chkAsStr = true;
}
else if (startsWith(arg, "--belongs:")) {
    string_q orig = arg;
    arg = substitute(arg, "--belongs:", "");
    if (!isAddress(arg))
        return usa ge(arg + " does not appear to be a valid Ethereum address.\n");
    filters.push_back(str_2_Addr(toLower(arg)));

    ////////////////////////////////////////////////////////////
    if (chkAsStr && filters.size() == 0)
        return usa ge("chkAsStr only works with a --belongs filter.");

    ////////////////////////////////////////////////////////////
    if (options.filters.size() > 0) {
        bool on = options.chkAsStr;
        options.chkAsStr = false;
        forEveryTransaction(checkBelongs, &options, options.transList.queries);
        if (!options.belongs) {
            if (on) {
                options.chkAsStr = on;
                forEveryTransaction(checkBelongsDeep, &options, options.transList.queries);
            }
            if (!options.belongs) {
                for (auto addr : options.filters) {
                    cout << "\taddress " << cRed << addr << cOff << " not found ";
                    cout << options.transList.queries << "\n";
                }
            }
        }
    } else {
        ////////////////////////////////////////////////////////////
        filters.clear();
        belongs = false;
        chkAsStr = false;

        ////////////////////////////////////////////////////////////
        //--------------------------------------------------------------------------------
        bool visitAddrs(const CAppearance& item, void* data) {
            COptions* opt = (COptions*)data;

            if (opt->belongs)
                return false;

            for (auto addr : opt->filters) {
                if (addr % item.addr) {
                    cout << "\t" << cGreen << " found at " << cTeal << item << cOff << "                     ";
                    if (verbose)
                        cout << "\n";
                    else
                        cout << "\r";
                    cout.flush();
                    opt->belongs = true;
                    return true;  // we're done
                }
            }
            return true;
        }

        //--------------------------------------------------------------------------------
        bool checkBelongs(CTransaction & trans, void* data) {
            // if we've been told we're done (because we found the target), stop searching
            if (!trans.forEveryAddress(visitAddrs, NULL, data))
                return false;
            return true;
        }

        //--------------------------------------------------------------------------------
        bool checkBelongsDeep(CTransaction & trans, void* data) {
            COptions* opt = (COptions*)data;
            for (auto addr : opt->filters) {
                string_q bytes = substitute(addr, "0x", "");
                if (contains(trans.input, bytes)) {
                    cout << "\t" << cRed << addr << cOff << " found at input by string search at ";
                    cout << cTeal << trans.blockNumber << "." << trans.transactionIndex << cOff
                         << "                       \n";
                    opt->belongs = true;
                    return false;
                }
                for (auto l : trans.receipt.logs) {
                    if (contains(l.data, bytes)) {
                        cout << "\t" << cRed << addr << cOff << " found at log by string search at ";
                        cout << cTeal << trans.blockNumber << "." << trans.transactionIndex << cOff
                             << "                       \n";
                        opt->belongs = true;
                        return false;
                    }
                }
                getTraces(trans.traces, trans.hash);
                for (auto trace : trans.traces) {
                    if (contains(trace.action.input, bytes)) {
                        cout << "\t" << cRed << addr << cOff << " found at trace by string search at ";
                        cout << cTeal << trans.blockNumber << "." << trans.transactionIndex << cOff
                             << "                       \n";
                        opt->belongs = true;
                        return false;
                    }
                }
            }
            return true;
        }
#endif
*/
