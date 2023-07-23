package caps

import "strings"

type Capability int

const (
	None Capability = 1 << iota
	Caching
	NoHeader
)

var AllCaps = []Capability{None, Caching, NoHeader}

func (c Capability) Has(cap Capability) bool {
	return c&cap != 0
}

func (c Capability) Add(cap Capability) Capability {
	v := c | cap
	if cap != None {
		v = v.Remove(None)
	}
	return v
}

func (c Capability) Remove(cap Capability) Capability {
	v := c &^ cap
	if v == 0 {
		v = None
	}
	return v
}

func (c Capability) Text() string {
	switch c {
	case NoHeader:
		return "noHeader"
	case Caching:
		return "cache"
	case None:
		return "none"
	default:
		return "unknown"
	}
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
