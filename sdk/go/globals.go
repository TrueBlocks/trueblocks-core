package sdk

import (
	"encoding/json"
	"net/url"
)

// Globals is a subset of globally available options from the command line
// that make sense in the SDK context
type Globals struct {
	Ether   bool   `arg:"ether,omitempty" json:"ether,omitempty"`
	Raw     bool   `arg:"raw,omitempty" json:"raw,omitempty"`
	Cache   bool   `arg:"cache,omitempty" json:"cache,omitempty"`
	Decache bool   `arg:"decache,omitempty" json:"decache,omitempty"`
	Verbose bool   `arg:"verbose,omitempty" json:"verbose,omitempty"`
	Chain   string `arg:"chain,omitempty" json:"chain,omitempty"`
	Output  string `arg:"output,omitempty" json:"output,omitempty"`
	Append  bool   `arg:"append,omitempty" json:"append,omitempty"`
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
