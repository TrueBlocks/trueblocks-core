package caps

import "strings"

type Capability int

const (
	Caching Capability = 1 << iota
	Fmt
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
	Chain
	Names
	Default = Verbose | Fmt | Version | Noop | NoColor | Chain | NoHeader | File | Output | Append
)

var allCaps = []Capability{
	Default,
	Caching,
	Fmt,
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
	Chain,
	Names,
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
		return "cache,decache"
	case Fmt:
		return "fmt"
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
		return "nocolor"
	case Chain:
		return "chain"
	case Names:
		return "names"
	case Default:
		return "default"
	default:
		return "unknown"
	}
}

func (c Capability) HasKey(key string) bool {
	if key == "fmt" || key == "file" || key == "testRunner" {
		return true
	}

	if key == "cache" || key == "decache" {
		return c.Has(Caching)
	}

	for _, cap := range allCaps {
		if key == cap.Text() {
			return c.Has(cap)
		}
	}

	return false
}

func (c Capability) String() string {
	ret := []string{}
	for _, cap := range allCaps {
		if c.Has(cap) {
			ret = append(ret, cap.Text())
		}
	}
	return strings.Join(ret, ",")
}

func (c Capability) Show() string {
	cc := c.Remove(Names) // Names has no --names flag. It's for internal use only.
	ret := []string{}
	for _, cap := range allCaps {
		if cc.Has(cap) && !Default.Has(cap) {
			ret = append(ret, cap.Text())
		}
	}
	for _, cap := range allCaps {
		if !cc.Has(cap) && Default.Has(cap) {
			ret = append(ret, "-"+cap.Text())
		}
	}
	return strings.Join(ret, ",")
}
