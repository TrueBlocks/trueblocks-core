/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
package output

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"html/template"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var OutputFn string
var Format string

type Meta struct {
	Unripe    string `json:"unripe"`
	Ripe      string `json:"ripe"`
	Staging   string `json:"staging"`
	Finalized string `json:"finalized"`
	Client    string `json:"client"`
}

func (m Meta) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

func GetMeta() *Meta {
	if utils.IsTestMode() {
		return &Meta{
			Unripe:    "0xdeadbeef",
			Ripe:      "0xdeadbeef",
			Staging:   "0xdeadbeef",
			Finalized: "0xdeadbeef",
			Client:    "0xdeadbeef",
		}
	}
	client := rpcClient.Get()
	bn, _ := client.BlockNumber(context.Background())
	return &Meta{Client: fmt.Sprintf("%d", bn)}
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

func GetFields(t *reflect.Type) (fields []string, sep string, quote string) {
	for i := 0; i < (*t).NumField(); i++ {
		fields = append(fields, MakeFirstLowerCase((*t).Field(i).Name))
	}
	sep = "\t"
	quote = ""
	if Format == "csv" {
		sep = ","
		quote = "\""
	}
	return fields, sep, quote
}

func GetHeader(t *reflect.Type) string {
	if (*t).Kind() != reflect.Struct {
		logger.Fatal((*t).Name() + " is not a structure")
	}

	fields, sep, quote := GetFields(t)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + field + quote)
	}
	return sb.String()
}

func GetRowTemplate(t *reflect.Type) (*template.Template, error) {
	fields, sep, quote := GetFields(t)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + "{{." + MakeFirstUpperCase(field) + "}}" + quote)
	}
	// fmt.Println(sb.String() + "\n")
	tt, err := template.New("").Parse(sb.String() + "\n")
	return tt, err
}
