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
	Chain    string
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
	if len(g.Chain) > 0 {
		values.Set("chain", g.Chain)
	}
}
