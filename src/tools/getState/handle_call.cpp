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
#include "options.h"

//---------------------------------------------------------------------------------------------------
bool COptions::handle_call(void) {
    CStringArray vars;
    explode(vars, call, '!');
    if (vars.size() == 0)
        return usage("You must supply the address of a smart contract for the --call option.");
    if (vars.size() == 1) {
        if (!isTestMode() && !isApiMode()) {
            cout << doCommand("chifra abis " + vars[0]);
            return false;
        }
        return usage("You must provide a four-byte code to the smart contract you're calling.");
    }
    if (!isAddress(vars[0]))
        return usage("The first part of the call data to --call must be an address.");
    if (!isHexStr(vars[1]))
        return usage("The four byte must be a hex string.");

    theCall.address = vars[0];
    theCall.encoding = vars[1];
    theCall.bytes = vars.size() > 2 ? vars[2] : "";
    theCall.blockNumber = isTestMode() ? 10092000 : getBlockProgress(BP_CLIENT).client;
    // We load known abis first (so we have something, if possible) then lay over from etherscan to get better names
    theCall.abi_spec.loadAbisFromKnown();
    theCall.abi_spec.loadAbiFromEtherscan(theCall.address, false);
    if (doEthCall(theCall)) {
        CTransaction art;
        art.input = theCall.encoding + theCall.bytes;
        abi_spec.articulateTransaction(&art);
        theCall.result.inputs = art.articulatedTx.inputs;
        theCall.address = vars[0];

        // Display formatting
        expContext().exportFmt = JSON1;
        string_q format = STR_DISPLAY_FUNCTION;
        configureDisplay("getState", "CEthState", format.empty() ? STR_DISPLAY_ETHSTATE : format);
        manageFields(
            "CParameter:str_default,indexed,internalType,components,no_write,is_pointer,is_array,is_object,is_"
            "builtin,"
            "is_minimal,type|CFunction:stateMutability,type,constant|CEthCall:abi_spec",
            FLD_HIDE);
        manageFields("CFunction:address|CEthState:result,address", FLD_SHOW);

        return true;
    }
    return usage("No result from call to " + theCall.address + " with fourbyte " + theCall.encoding + ".");
}
