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
#include "options.h"

//-----------------------------------------------------------------------
bool COptions::handle_classes(void) {
#if 1
    return false;
#else
    for (auto item : abi_spec.interfaceMap) {
        CFunction func = item.second;
        establishFolder("./classes/classDefinitions/");
        ostringstream os;
        os << "[settings]" << endl;
        os << "base_class = CTransaction" << endl;
        os << "class = C" << toUpper(string_q(1, func.name[0])) << func.name.substr(1, 1000) << endl;
        os << "fields =";
        bool first = true;
        for (auto field : func.inputs) {
            if (!first)
                os << " |\\" << endl;
            os << "  " << field.type << " " << field.name;
            first = false;
        }
        cerr << "Writing to ./classes/classDefinitions/" << toLower(func.name) << ".txt" << endl;
        cerr << substitute(os.str(), "\t", "  ");
        stringToAsciiFile("./classes/classDefinitions/" + toLower(func.name) + ".txt", os.str());
    }
    return false;
#endif
}
