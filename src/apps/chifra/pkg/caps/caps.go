package caps

import "strings"

type Capability int

const (
	Caching Capability = 1 << iota
	Format
	Raw
	NoHeader
	Wei
	Ether
	File
	Output
	Append
	Verbose
	Version
	Noop
	NoColor
	LogLevel
	Chain
	Always  = Verbose | Version | Noop | NoColor | LogLevel | Chain
	Default = Always | Format | Raw | NoHeader | File | Output | Append
)

var AllCaps = []Capability{
	Default,
	Caching,
	Format,
	Raw,
	NoHeader,
	Wei,
	Ether,
	File,
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
	case Format:
		return "fmt"
	case Raw:
		return "raw"
	case NoHeader:
		return "noHeader"
	case Wei:
		return "wei"
	case Ether:
		return "ether"
	case File:
		return "file"
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
	case Always:
		return "always"
	case Default:
		return "default"
	default:
		return "unknown"
	}
}

func (c Capability) HasKey(key string) bool {
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
