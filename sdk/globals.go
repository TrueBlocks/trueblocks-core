package sdk

import (
	"encoding/json"
	"fmt"
	"os"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

// Globals is a subset of globally available options from the command line
// that make sense in the SDK context
type Globals struct {
	Ether   bool   `json:"ether,omitempty"`
	Cache   bool   `json:"cache,omitempty"`
	Decache bool   `json:"decache,omitempty"`
	Verbose bool   `json:"verbose,omitempty"`
	Chain   string `json:"chain,omitempty"`
	Output  string `json:"output,omitempty"`
	Append  bool   `json:"append,omitempty"`
	rCtx    output.RenderCtx
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

func (g Globals) String() string {
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

func convertObjectToArray(field, strIn string) string {
	convertToArray := func(field, str string) (string, bool) {
		find := "\"" + field + "\": {"
		start := strings.Index(str, find)
		if start == -1 {
			return str, false
		}

		braceCount := 0
		end := start + len(find)
		for i := end; i < len(str); i++ {
			if str[i] == '{' {
				braceCount++
			} else if str[i] == '}' {
				if braceCount == 0 {
					end = i + 1
					break
				}
				braceCount--
			}
		}

		beforeB := str[:start+len(find)-1]      // Adjust to include '{'
		afterB := str[end:]                     // after "}"
		objectB := str[start+len(find)-1 : end] // Adjust to start from '{'
		return beforeB + "[" + objectB + "]" + afterB, strings.Contains(str, find)
	}

	str := strIn
	for {
		var more bool
		str, more = convertToArray(field, str)
		if !more {
			break
		}
	}

	return str
}

func convertEmptyStrToZero(field, strIn string) string {
	convertToZero := func(field, str string) (string, bool) {
		find := "\"" + field + "\": \"\""
		start := strings.Index(str, find)
		if start == -1 {
			return str, false
		}

		end := start + len(find)
		for i := end; i < len(str); i++ {
			if str[i] == ',' || str[i] == '}' {
				end = i
				break
			}
		}

		beforeB := str[:start+len(find)-2] // Adjust to include '""'
		afterB := str[end:]                // after ","
		return beforeB + "\"0\"" + afterB, strings.Contains(str, find)
	}

	str := strIn
	for {
		var more bool
		str, more = convertToZero(field, str)
		if !more {
			break
		}
	}

	return str
}

func debugPrint(str string, t any, err error) {
	fmt.Println("======================================")
	fmt.Println(err)
	fmt.Println(reflect.TypeOf(t))
	fmt.Println(str[:2000])
	fmt.Println("======================================")
	os.Exit(1)
}
