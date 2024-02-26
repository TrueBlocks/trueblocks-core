package sdk

import "net/url"

type Fmt int

const (
	NoFmt Fmt = iota
	Json
	Txt
	Csv
)

func (f Fmt) String() string {
	switch f {
	case Json:
		return "json"
	case Txt:
		return "txt"
	case Csv:
		return "csv"
	}
	return "unknown"
}

type Globals struct {
	Ether    bool
	Raw      bool
	NoHeader bool
	Cache    bool
	Decache  bool
	Fmt      Fmt
	Verbose  bool
}

func (g Globals) mapGlobals(values url.Values) {
	if g.Ether {
		values.Set("ether", "true")
	}
	if g.Raw {
		values.Set("raw", "true")
	}
	if g.NoHeader {
		values.Set("noHeader", "true")
	}
	if g.Cache {
		values.Set("cache", "true")
	}
	if g.Decache {
		values.Set("decache", "true")
	}
	if g.Fmt != NoFmt {
		values.Set("fmt", g.Fmt.String())
	}
	if g.Verbose {
		values.Set("verbose", "true")
	}
	// Wei     bool            `json:"wei,omitempty"`
	// Ether   bool            `json:"ether,omitempty"`
	// Help    bool            `json:"help,omitempty"`
	// File    string          `json:"file,omitempty"`
	// Version bool            `json:"version,omitempty"`
	// Noop    bool            `json:"noop,omitempty"`
	// NoColor bool            `json:"noColor,omitempty"`
	// Cache   bool            `json:"cache,omitempty"`
	// Decache bool            `json:"decache,omitempty"`
	// Caps    caps.Capability `json:"-"`
	// output.OutputOptions
}
