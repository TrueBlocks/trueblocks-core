package sdk

import (
	"encoding/json"
	"net/url"
)

// Globals is a subset of globally available options from the command line
// that make sense in the SDK context
type Globals struct {
	Ether   bool   `json:"ether,omitempty"`
	Raw     bool   `json:"raw,omitempty"`
	Cache   bool   `json:"cache,omitempty"`
	Decache bool   `json:"decache,omitempty"`
	Verbose bool   `json:"verbose,omitempty"`
	Chain   string `json:"chain,omitempty"`
	Output  string `json:"output,omitempty"`
	Append  bool   `json:"append,omitempty"`
	// Probably can't support
	// --file
	// Global things ignored in the SDK
	// --help
	// --wei
	// --fmt
	// --version
	// --noop
	// --nocolor
	// --no_header
}

func (g *Globals) String() string {
	bytes, _ := json.Marshal(g)
	return string(bytes)
}

func (g Globals) mapGlobals(values url.Values) {
	if g.Ether {
		values.Set("ether", "true")
	}
	if g.Raw {
		values.Set("raw", "true")
	}
	if g.Cache {
		values.Set("cache", "true")
	}
	if g.Decache {
		values.Set("decache", "true")
	}
	if g.Verbose {
		values.Set("verbose", "true")
	}
	if len(g.Chain) > 0 {
		values.Set("chain", g.Chain)
	}
	if len(g.Output) > 0 {
		values.Set("output", g.Output)
	}
	if g.Append {
		values.Set("append", "true")
	}
}
