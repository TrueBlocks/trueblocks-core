package sdk

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

func (g Globals) mapGlobals(options map[string]string) {
	if g.Ether {
		options["ether"] = "true"
	}
	if g.Raw {
		options["raw"] = "true"
	}
	if g.NoHeader {
		options["noHeader"] = "true"
	}
	if g.Cache {
		options["cache"] = "true"
	}
	if g.Decache {
		options["decache"] = "true"
	}
	if g.Fmt != NoFmt {
		options["fmt"] = g.Fmt.String()
	}
	if g.Verbose {
		options["verbose"] = "true"
	}
}
