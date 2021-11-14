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
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

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

func GetMeta(testMode bool) *Meta {
	if testMode {
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

// TODO: Can we use golang templates for speed?
// func getFields(t *reflect.Type) (fields []string, sep string, quote string) {
// 	for i := 0; i < (*t).NumField(); i++ {
// 		fields = append(fields, MakeFirstLowerCase((*t).Field(i).Name))
// 	}
// 	sep = "\t"
// 	quote = ""
// 	if Format == "csv" {
// 		sep = ","
// 		quote = "\""
// 	}
// 	return fields, sep, quote
// }

// func GetHeader(t *reflect.Type) string {
// 	if (*t).Kind() != reflect.Struct {
// 		logger.Fatal((*t).Name() + " is not a structure")
// 	}

// 	fields, sep, quote := getFields(t)
// 	var sb strings.Builder
// 	for i, field := range fields {
// 		if i > 0 {
// 			sb.WriteString(sep)
// 		}
// 		sb.WriteString(quote + field + quote)
// 	}
// 	return sb.String()
// }

// func GetRowTemplate(t *reflect.Type) (*template.Template, error) {
// 	fields, sep, quote := getFields(t)
// 	var sb strings.Builder
// 	for i, field := range fields {
// 		if i > 0 {
// 			sb.WriteString(sep)
// 		}
// 		sb.WriteString(quote + "{{." + MakeFirstUpperCase(field) + "}}" + quote)
// 	}
// 	// fmt.Println(sb.String() + "\n")
// 	tt, err := template.New("").Parse(sb.String() + "\n")
// 	return tt, err
// }

// ToStringRecords uses Reflect API to read data from the provided slice of structs and
// turns it into a slice of string slices that can be later passed to encoding package
// writers to convert between different output formats
func ToStringRecords(data interface{}, quote bool) ([][]string, error) {
	var records [][]string
	// We can quote the data now, so that we don't have to loop over it again
	// later.
	format := "%v"
	if quote {
		format = `"%v"`
	}
	header := []string{}
	// We only support slice of structs
	slice := reflect.ValueOf(data)
	if slice.Kind() != reflect.Slice {
		return nil, fmt.Errorf("%s is not a structure", slice.Type().Name())
	}
	insideType := reflect.TypeOf(slice.Index(0).Interface())
	if insideType.Kind() != reflect.Struct {
		return nil, fmt.Errorf("%s is not a struct", insideType.Name())
	}

	for i := 0; i < slice.Len(); i++ {
		var record []string
		// Read the struct
		content := slice.Index(i).Interface()
		strct := reflect.ValueOf(content)

		for j := 0; j < strct.NumField(); j++ {
			// Now read each field from it and put into record
			field := strct.Field(j).Interface()
			record = append(record, fmt.Sprintf(format, field))

			// If it's our first iteration, we save the struct's key names
			// to use them as headers
			if i == 0 {
				header = append(header, fmt.Sprintf(format, MakeFirstLowerCase(insideType.Field(j).Name)))
			}
		}
		records = append(records, record)
	}

	result := [][]string{
		header,
	}
	result = append(result, records...)
	return result, nil
}
