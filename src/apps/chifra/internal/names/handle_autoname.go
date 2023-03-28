package namesPkg

import "fmt"

func (opts *NamesOptions) HandleAutoname() error {
	return fmt.Errorf("the chifra names --autoname option is currently not available")
}

// } else if (!autoname.empty()) {
//     abi_spec.loadAbisFromKnown(true);
//     crudCommands.push_back("create");
//     terms.push_back(autoname);
//     // ::setenv("TB_NAME_ADDRESS", autoname.c_str(), true);
//     ::setenv("TB_NAME_NAME", addr_2_Petname(autoname, '-').c_str(), true);
//     ::setenv("TB_NAME_TAG", "50-Tokens:ERC20", true);
//     ::setenv("TB_NAME_SOURCE", "TrueBlocks.io", true);
//     ::setenv("TB_NAME_SYMBOL", "", true);
//     ::setenv("TB_NAME_DECIMALS", "18", true);
//     ::setenv("TB_NAME_CUSTOM", "false", true);
//     if (!handle_editcmds(true))  // returns true on success
//         return false;
// }
