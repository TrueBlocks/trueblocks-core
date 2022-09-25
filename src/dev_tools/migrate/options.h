#pragma once
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
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "etherlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    // END_CODE_DECLARE

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool handle_migrate(const CStringArray& cachePaths);
    bool handle_migrate_test(const CStringArray& cachePaths);
};

//-------------------------------------------------------------------------
class CMigrationChecker {
  public:
    bool needs;
    string_q path;
    string_q msg;
    size_t nSeen;
    size_t nMigrated;
    size_t nSkipped;
    string_q which;

    CMigrationChecker(const string_q& p, const string_q& w)
        : needs(false), path(p), msg(""), nSeen(0), nMigrated(0), nSkipped(0), which(w) {
    }

    CMigrationChecker(const CMigrationChecker& mig) {
        nSeen = mig.nSeen;
        nMigrated = mig.nMigrated;
        nSkipped = mig.nSkipped;
        path = mig.path;
        msg = mig.msg;
    }

    CMigrationChecker& operator+=(const CMigrationChecker& mig) {
        nSeen += mig.nSeen;
        nMigrated += mig.nMigrated;
        nSkipped += mig.nSkipped;
        return *this;
    }

    string_q Report(void) const {
        ostringstream os;
        os << relativize(path) << ": ";
        os << nSeen << " files seen. ";
        os << nMigrated << " files migrated. ";
        os << (nSeen - nMigrated) << " files up to date. ";
        os << nSkipped << " files skipped.";
        return os.str();
    }

  private:
    CMigrationChecker(void) = delete;
    CMigrationChecker& operator=(const CMigrationChecker&) = delete;
};

extern bool needsMigration(const string_q& path, void* data);
