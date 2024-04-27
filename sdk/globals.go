package sdk

import (
	"encoding/json"
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

type CacheOp uint8

const (
	CacheOn CacheOp = iota
	CacheOff
	Decache
)

func (g *Globals) Caching(op CacheOp) {
	switch op {
	case CacheOn:
		g.Cache = true
		g.Decache = false
	case CacheOff:
		g.Cache = false
		g.Decache = false
	case Decache:
		g.Cache = false
		g.Decache = true
	}
}

type Cacher interface {
	Caching(op CacheOp)
}
