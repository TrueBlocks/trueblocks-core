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
#include "utillib.h"
#include "markdown.h"

namespace qblocks {

void CMarkdownTable::CMarkdownRow::addCol(const string_q& s) {
    cols.push_back(s);
}

string_q CMarkdownTable::CMarkdownRow::render(size_t* wids) const {
    ostringstream os;
    os << "| ";
    size_t cnt = 0;
    for (auto c : cols) {
        os << padRight(c, wids[cnt++]) << " |";
    }
    os << endl;
    return os.str();
}

CMarkdownTable::CMarkdownTable(void) {
    bzero(wids, sizeof(wids));
}

void CMarkdownTable::addHeader(const string_q& s1, const string_q& s2, const string_q& s3) {
    header.addCol(s1);
    header.addCol(s2);
    header.addCol(s3);
}

void CMarkdownTable::addRow(const string_q& s1, const string_q& s2, const string_q& s3) {
    CMarkdownRow row;
    row.addCol(s1);
    row.addCol(s2);
    row.addCol(s3);
    rows.push_back(row);
}

string_q CMarkdownTable::render(void) {
    bzero(wids, sizeof(wids));
    for (auto row : rows) {
        size_t cnt = 0;
        for (auto c : row.cols) {
            if (cnt >= MAX_MARKDOWN_COLS)
                break;
            wids[cnt] = max(wids[cnt], c.length());
            cnt++;
        }
    }

    ostringstream os;
    for (auto row : rows) {
        os << row.render(wids) << endl;
    }
    return os.str();
}

string_q markDownRow(const string_q& s1, const string_q& s2, const string_q& s3, size_t* widths) {
    string_q ss1 = (s1 == "-" ? string_q(widths[0], '-') : s1);
    string_q ss2 = (s1 == "-" ? string_q(widths[1], '-') : s2);
    string_q ss3 = (s1 == "-" ? string_q(widths[2], '-') : s3);

    ostringstream os;
    os << "| ";
    os << padRight(ss1, widths[0]) << " | ";
    os << padRight(ss2, widths[1]) << " | ";
    os << padRight(ss3, widths[2]);
    os << " |";
    os << endl;
    return os.str();
}

}  // namespace qblocks