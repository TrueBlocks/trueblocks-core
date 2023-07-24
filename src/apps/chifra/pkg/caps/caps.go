package caps

import "strings"

type Capability int

const (
	Caching Capability = 1 << iota
	// Fmt
	Raw
	NoHeader
	Wei
	Ether
	// File
	Output
	Append
	Verbose
	Version
	Noop
	NoColor
	LogLevel
	Chain
	EveryTool = Verbose | Version | Noop | NoColor | LogLevel | Chain
	Default   = EveryTool | NoHeader | Output | Append
	// Fmt and File are different and don't really work right
)

var AllCaps = []Capability{
	Default,
	Caching,
	// Fmt,
	Raw,
	NoHeader,
	Wei,
	Ether,
	// File,
	Output,
	Append,
	Verbose,
	Version,
	Noop,
	NoColor,
	LogLevel,
	Chain,
}

func (c Capability) Has(cap Capability) bool {
	return c&cap != 0
}

func (c Capability) Add(cap Capability) Capability {
	v := c | cap
	return v
}

func (c Capability) Remove(cap Capability) Capability {
	v := c &^ cap
	if v == 0 {
		v = Default
	}
	return v
}

func (c Capability) Text() string {
	switch c {
	case Caching:
		return "cache"
	// case Fmt:
	// 	return "fmt"
	case Raw:
		return "raw"
	case NoHeader:
		return "noHeader"
	case Wei:
		return "wei"
	case Ether:
		return "ether"
	// case File:
	// 	return "file"
	case Output:
		return "output"
	case Append:
		return "append"
	case Verbose:
		return "verbose"
	case Version:
		return "version"
	case Noop:
		return "noop"
	case NoColor:
		return "noColor"
	case LogLevel:
		return "logLevel"
	case Chain:
		return "chain"
	case EveryTool:
		return "every"
	case Default:
		return "default"
	default:
		return "unknown"
	}
}

func (c Capability) HasKey(key string) bool {
	if key == "fmt" || key == "file" {
		return true
	}

	for _, cap := range AllCaps {
		if key == cap.Text() {
			return c.Has(cap)
		}
	}

	return false
}

func (c Capability) String() string {
	ret := []string{}
	for _, cap := range AllCaps {
		if c.Has(cap) {
			ret = append(ret, cap.Text())
		}
	}
	return strings.Join(ret, ",")
}
