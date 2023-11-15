package parser

// ContractCall is a call to a smart contract in any of the 3 supported forms
// (see Input syntax above)
type ContractCall struct {
	// Four byte selector, e.g.
	// 0xcdba2fd4(105)
	SelectorCall *SelectorContractCall `parser:"@@"`

	// Everything encoded, e.g.
	// 0xcdba2fd40000000000000000000000000000000000000000000000000000000000007a69
	Encoded string `parser:" | @Hex"`

	// Function name, e.g.
	// someName(105)
	FunctionNameCall *FunctionContractCall `parser:" | @@"`
}

type SelectorContractCall struct {
	Selector  Selector            `parser:"@Hex '('"`
	Arguments []*ContractArgument `parser:" (@@ (',' @@)*)? ')'"`
}

type FunctionContractCall struct {
	Name      string              `parser:"@SolidityIdent '('"`
	Arguments []*ContractArgument `parser:" (@@ (',' @@)*)? ')'"`
}

// ParseCall turns smart contract method call string and parses it into
// a nice structures, from which we can easily extract the data to
// make the call.
func ParseCall(source string) (*ContractCall, error) {
	return ourParser.ParseString("", source)
}
