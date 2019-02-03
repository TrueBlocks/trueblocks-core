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
#include "utillib.h"

inline string_q wei_2_Ether(const string_q& _value)
{
        // Make sure the wei number is at least 18 characters long. Once it is,
        // reverse it, put a decimal at the 18th position, reverse it back,
        // strip leading '0's except the tens digit.
        string_q ether = _value;
        if (ether.length()<18)
                ether = padLeft(_value,18).Substitute(" ","0");
        ether.Reverse();
        ether = extract(ether, 0, 18) + "." + extract(ether, 18);
        ether.Reverse();
        ether = trimLeading(ether, '0');
        if (startsWith(ether, '.'))
                ether = "0" + ether;
        return ether;
}

int main(int argc, const char *argv[]) {
    etherlib_init("binary", quickQuitHandler);

    string_q contents;
    asciiFileToString("file.txt", contents);
	while (!contents.empty())
	{
		string_q addr = nextTokenClear(contents,'\n');
		string_q amt = nextTokenClear(addr,'\t');
		while (amt.length()<24)
			amt.Replace("0x","0x0");
		cout << amt << "\t" << str_2_Wei(amt) << "\t" << wei_2_Ether(to_string(str_2_Wei(amt))) << "\t" << addr << "\n";
	}
}
