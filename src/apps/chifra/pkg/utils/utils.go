// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package utils

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"math/big"
	"net/http"
	"os"
	"os/exec"
	"reflect"
	"runtime"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"golang.org/x/term"
)

// IsTestModeServer return true if we are running from the testing harness
func IsTestModeServer(r *http.Request) bool {
	return r.Header.Get("User-Agent") == "testRunner"
}

// IsServerWriter tries to cast `w` into `http.ResponseWriter`
// and returns true if the cast was successful
func IsServerWriter(w io.Writer) bool {
	_, ok := w.(http.ResponseWriter)
	return ok
}

func IsTerminal() bool {
	return term.IsTerminal(int(os.Stdout.Fd()))
}

func AsciiFileToString(fileName string) string {
	return file.AsciiFileToString(fileName)
}

func AsciiFileToLines(fileName string) []string {
	return file.AsciiFileToLines(fileName)
}

func OpenBrowser(url string) {
	var err error
	switch runtime.GOOS {
	case "linux":
		err = exec.Command("xdg-open", url).Start()
	case "windows":
		err = exec.Command("rundll32", "url.dll,FileProtocolHandler", url).Start()
	case "darwin":
		err = exec.Command("open", url).Start()
	default:
		err = fmt.Errorf("unsupported platform")
	}
	if err != nil {
		log.Fatal(err)
	}
}

func PadNum(n int, totalLen int) string {
	return PadLeft(strconv.Itoa(n), totalLen, '0')
}

func PadLeft(str string, totalLen int, pad rune) string {
	if len(str) >= totalLen {
		return str
	}
	if pad == 0 {
		pad = ' '
	}
	lead := ""
	for i := 0; i < totalLen-len(str); i++ {
		lead += string(pad)
	}
	return lead + str
}

func PadRight(str string, totalLen int, pad rune) string {
	if len(str) >= totalLen {
		return str
	}
	if pad == 0 {
		pad = ' '
	}
	tail := ""
	for i := 0; i < totalLen-len(str); i++ {
		tail += string(pad)
	}
	return str + tail
}

func ToCamelCase(in string) string {
	if len(in) == 0 {
		return in
	}

	var arr []string
	fields := strings.Fields(in)
	for _, field := range fields {
		arr = append(arr, strings.Title(field))
	}
	arr[0] = strings.ToLower(arr[0])
	return strings.Join(arr, "")
}

// maximum uint64
const NOPOS = ^uint64(0)

// Min calculates the minimum between two unsigned integers (golang has no such function)
func Min[T int | float64 | uint32 | uint64](x, y T) T {
	if x < y {
		return x
	}
	return y
}

// Max calculates the max between two unsigned integers (golang has no such function)
func Max[T int | float64 | uint32 | uint64](x, y T) T {
	if x > y {
		return x
	}
	return y
}

func MakeFirstLowerCase(s string) string {
	if len(s) < 2 {
		return strings.ToLower(s)
	}
	bts := []byte(s)
	lc := bytes.ToLower([]byte{bts[0]})
	rest := bts[1:]
	return string(bytes.Join([][]byte{lc, rest}, nil))
}

func MakeFirstUpperCase(s string) string {
	if len(s) < 2 {
		return strings.ToLower(s)
	}
	bts := []byte(s)
	lc := bytes.ToUpper([]byte{bts[0]})
	rest := bts[1:]
	return string(bytes.Join([][]byte{lc, rest}, nil))
}

// EarliestEvmTs - The timestamp of the first Ethereum block in summer 2015 was this value. Since Ethereum
// was the first EVM based blockchain, all other EVM based block chains have timestamps after this. We can
// use this fact to distinguish between block numbers and timestamps on the command line (any number in a block
// range smaller than this is a blockNumber, anything larger than this is a timestamp). This breaks when the
// block number gets larger than 1,4 billion, which may happen when the chain shards, but not until then.
const EarliestEvmTs = 1438269971

// TODO: Fix export without arrays
func GetFields(t *reflect.Type, format string, header bool) (fields []string, sep string, quote string) {
	sep = "\t"
	quote = ""
	if format == "csv" || strings.Contains(format, ",") {
		sep = ","
	}

	if format == "csv" || strings.Contains(format, "\"") {
		quote = "\""
	}

	if strings.Contains(format, "\t") || strings.Contains(format, ",") {
		custom := strings.Replace(format, "\t", ",", -1)
		custom = strings.Replace(custom, "\"", ",", -1)
		fields = strings.Split(custom, ",")

	} else {
		realType := *t

		if realType.Kind() == reflect.Pointer {
			realType = realType.Elem()
		}

		if realType.Kind() != reflect.Struct {
			logger.Fatal(realType.Name() + " is not a structure")
		}
		for i := 0; i < realType.NumField(); i++ {
			field := realType.Field(i)
			// We don't want to return private fields
			if !field.IsExported() {
				continue
			}

			fn := field.Name
			if header {
				fields = append(fields, MakeFirstLowerCase(fn))
			} else {
				fields = append(fields, fn)
			}
		}
	}

	return fields, sep, quote
}

func ToBigInt(str string) big.Int {
	ret := big.Int{}
	if len(str) > 2 && str[:2] == "0x" {
		ret.SetString(str[2:], 16)
	} else {
		ret.SetString(str, 10)
	}
	return ret
}
