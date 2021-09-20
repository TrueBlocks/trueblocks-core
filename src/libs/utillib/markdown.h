#pragma once

#include <strings.h>
#include "basetypes.h"
#include "sfstring.h"

namespace qblocks {

class CMarkdownTable {
    class CMarkdownRow {
      public:
        CStringArray cols;
        void addCol(const string_q& s);
        string_q render(size_t* wids) const;
    };
    typedef vector<CMarkdownRow> CMarkdownRowArray;

#define MAX_MARKDOWN_COLS 10
    size_t wids[MAX_MARKDOWN_COLS];

  public:
    CMarkdownTable(void);
    CMarkdownRow header;
    CMarkdownRowArray rows;
    void addHeader(const string_q& s1, const string_q& s2, const string_q& s3);
    void addRow(const string_q& s1, const string_q& s2, const string_q& s3);
    string_q render(void);
};

extern string_q markDownRow(const string_q& s1, const string_q& s2, const string_q& s3, size_t* widths);

}  // namespace qblocks
