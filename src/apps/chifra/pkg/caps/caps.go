package caps

import "strings"

type Capability int

const (
	Caching Capability = 1 << iota
	Fmt
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
	Chain
	Default = Verbose | Fmt | Version | Noop | NoColor | Chain | NoHeader | File | Output | Append
)

var AllCaps = []Capability{
	Default,
	Caching,
	Fmt,
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
		return "cache,decache"
	case Fmt:
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
		return "nocolor"
	case Chain:
		return "chain"
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

func (c Capability) Show() string {
	ret := []string{}
	for _, cap := range AllCaps {
		if c.Has(cap) && !Default.Has(cap) {
			ret = append(ret, cap.Text())
		}
	}
	for _, cap := range AllCaps {
		if !c.Has(cap) && Default.Has(cap) {
			ret = append(ret, "-"+cap.Text())
		}
	}
	return strings.Join(ret, ",")
}
