package abi

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// This code sucks

func ExtractSigs(code string) []types.Function {
	code = prepareCode(code)

	ret := []types.Function{}
	for _, line := range strings.Split(code, "\n") {
		if !strings.Contains(line, "function") && !strings.Contains(line, "event") {
			continue
		}

		funcType := "function"
		if strings.Contains(line, "event") {
			funcType = "event"
		}
		line = strings.ReplaceAll(strings.ReplaceAll(line, "event", ""), "function", "")

		if p1, p2, ok := splitLine(line); ok {
			parts := strings.Split(p2, ",")
			x := []string{}
			for _, p := range parts {
				pp := strings.Split(p, " ")
				x = append(x, pp[0])
			}
			ps := []string{p1}
			f := types.Function{
				FunctionType: funcType,
				Name:         ps[0],
				Signature:    ps[0] + "(" + strings.Join(x, ",") + ")",
			}
			ret = append(ret, f)
		}
	}
	return ret
}

var keywords = []string{
	"indexed",
	"memory",
}

func splitLine(line string) (string, string, bool) {
	openIndex := strings.Index(line, "(")
	if openIndex == -1 {
		return "", "", false
	}

	nestingLevel := 1
	start := openIndex + 1
	for i := openIndex + 1; i < len(line); i++ {
		switch line[i] {
		case '(':
			nestingLevel++
		case ')':
			nestingLevel--
			if nestingLevel == 0 {
				p1 := line[:openIndex]
				p1 = strings.ReplaceAll(p1, "function ", "")
				p1 = strings.ReplaceAll(p1, "event ", "")
				p1 = strings.TrimSpace(p1)

				p2 := strings.TrimSpace(line[start : i+1])
				p2 = strings.ReplaceAll(p2, "\t", " ")
				for _, k := range keywords {
					p2 = strings.ReplaceAll(p2, k, " ")
				}
				for {
					if !strings.Contains(p2, "  ") {
						break
					}
					p2 = strings.ReplaceAll(p2, "  ", " ")
				}
				p2 = strings.ReplaceAll(p2, ", ", ",")
				p2 = strings.TrimSuffix(p2, ")")

				return p1, p2, true
			}
		}
	}

	return "", "", false
}

func prepareCode(code string) string {
	if !strings.Contains(code, "function") && !strings.Contains(code, "event") {
		if code[0:1] == strings.ToUpper(code[0:1]) {
			code = "event " + code
		} else {
			code = "function " + code
		}
	}
	code = strings.ReplaceAll(code, "|", "")
	code = strings.ReplaceAll(code, "\n", "")
	code = strings.ReplaceAll(code, "event", "}event")
	code = strings.ReplaceAll(code, "function", "}function")
	code = strings.ReplaceAll(code, "}", "\n")
	return code
}
