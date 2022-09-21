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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("mode", "m", "enum[test|cache|index]", OPT_FLAG, "either effectuate or test to see if a migration is necessary"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Run migrations."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern void establishTestData(void);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    string_q mode = "";
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-m:") || startsWith(arg, "--mode:")) {
            if (!confirmEnum("mode", mode, arg))
                return false;
        } else if (arg == "-m" || arg == "--mode") {
            return flag_required("mode");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, 0);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    // END_CODE_INIT

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//---------------------------------------------------------------------------------------------------
COptions::~COptions(void) {
}

// err = validate.ValidateEnum("--migrate", opts.Migrate, "[test|cache|index]")
// if err != nil {
// 	return err
// }

// if (migrate == "test") {
//     handle_migrate_test(cachePaths);
//     return false;
// } else if (migrate == "cache") {
//     handle_migrate(cachePaths);
//     return false;
// } else if (!migrate.empty()) {
//     // do nothing here for --migrate index. It's an error if
//     // it got this far. It should have been handled in the go code
//     return usage("Invalid migration: " + migrate);
// }

// if len(opts.Migrate) > 0 {
// 	for _, m := range opts.Modes {
// 		if m == "index" {
// 			if opts.Migrate == "test" {
// 				// Note this does not return if a migration is needed
// 				index.CheckBackLevelIndex(opts.Globals.Chain)
// 			}
// 		}
// 	}
// }
