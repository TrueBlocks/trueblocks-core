// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

// import (
// 	// EXISTING_CODE
// 	"bytes"
// 	"encoding/json"
// 	"fmt"
// 	"io"
// 	"strings"
// 	"time"

// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
// 	daemon "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
// 	// EXISTING_CODE
// )

// type DaemonOptions struct {
// 	Url     string       `json:"url,omitempty"`
// 	Api     DaemonApi    `json:"api,omitempty"`
// 	Scrape  DaemonScrape `json:"scrape,omitempty"`
// 	Monitor bool         `json:"monitor,omitempty"`
// 	Grpc    bool         `json:"grpc,omitempty"`
// 	Port    string       `json:"port,omitempty"`
// 	Silent  bool         `json:"silent,omitempty"`
// 	Globals
// }

// // String implements the stringer interface
// func (opts *DaemonOptions) String() string {
// 	bytes, _ := json.Marshal(opts)
// 	return string(bytes)
// }

// // DaemonBytes implements the chifra daemon command for the SDK.
// func (opts *DaemonOptions) DaemonBytes(w io.Writer) error {
// 	values, err := structToValues(*opts)
// 	if err != nil {
// 		return fmt.Errorf("error converting daemon struct to URL values: %v", err)
// 	}

// 	return daemon.Daemon(w, values)
// }

// // daemonParseFunc handles special cases such as structs and enums (if any).
// func daemonParseFunc(target interface{}, key, value string) (bool, error) {
// 	var found bool
// 	opts, ok := target.(*DaemonOptions)
// 	if !ok {
// 		return false, fmt.Errorf("parseFunc(daemon): target is not of correct type")
// 	}

// 	if key == "api" {
// 		var err error
// 		values := strings.Split(value, ",")
// 		if opts.Api, err = enumFromDaemonApi(values); err != nil {
// 			return false, err
// 		} else {
// 			found = true
// 		}
// 	}
// 	if key == "scrape" {
// 		var err error
// 		values := strings.Split(value, ",")
// 		if opts.Scrape, err = enumFromDaemonScrape(values); err != nil {
// 			return false, err
// 		} else {
// 			found = true
// 		}
// 	}

// 	// EXISTING_CODE
// 	// EXISTING_CODE

// 	return found, nil
// }

// // GetDaemonOptions returns a filled-in options instance given a string array of arguments.
// func GetDaemonOptions(args []string) (*DaemonOptions, error) {
// 	var opts DaemonOptions
// 	if err := assignValuesFromArgs(args, daemonParseFunc, &opts, &opts.Globals); err != nil {
// 		return nil, err
// 	}

// 	return &opts, nil
// }

// type daemonGeneric interface {
// }

// func queryDaemon[T daemonGeneric](opts *DaemonOptions) ([]T, *types.MetaData, error) {
// 	buffer := bytes.Buffer{}
// 	if err := opts.DaemonBytes(&buffer); err != nil {
// 		return nil, nil, err
// 	}

// 	var result Result[T]
// 	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
// 		return nil, nil, err
// 	} else {
// 		return result.Data, &result.Meta, nil
// 	}
// }

// type DaemonApi int

// const (
// 	NoDA  DaemonApi = 0
// 	DAOff           = 1 << iota
// 	DAOn
// )

// func (v DaemonApi) String() string {
// 	switch v {
// 	case NoDA:
// 		return "none"
// 	}

// 	var m = map[DaemonApi]string{
// 		DAOff: "off",
// 		DAOn:  "on",
// 	}

// 	var ret []string
// 	for _, val := range []DaemonApi{DAOff, DAOn} {
// 		if v&val != 0 {
// 			ret = append(ret, m[val])
// 		}
// 	}

// 	return strings.Join(ret, ",")
// }

// func enumFromDaemonApi(values []string) (DaemonApi, error) {
// 	if len(values) == 0 {
// 		return NoDA, fmt.Errorf("no value provided for api option")
// 	}

// 	var result DaemonApi
// 	for _, val := range values {
// 		switch val {
// 		case "off":
// 			result |= DAOff
// 		case "on":
// 			result |= DAOn
// 		default:
// 			return NoDA, fmt.Errorf("unknown api: %s", val)
// 		}
// 	}

// 	return result, nil
// }

// type DaemonScrape int

// const (
// 	NoDS  DaemonScrape = 0
// 	DSOff              = 1 << iota
// 	DSBlooms
// 	DSIndex
// )

// func (v DaemonScrape) String() string {
// 	switch v {
// 	case NoDS:
// 		return "none"
// 	}

// 	var m = map[DaemonScrape]string{
// 		DSOff:    "off",
// 		DSBlooms: "blooms",
// 		DSIndex:  "index",
// 	}

// 	var ret []string
// 	for _, val := range []DaemonScrape{DSOff, DSBlooms, DSIndex} {
// 		if v&val != 0 {
// 			ret = append(ret, m[val])
// 		}
// 	}

// 	return strings.Join(ret, ",")
// }

// func enumFromDaemonScrape(values []string) (DaemonScrape, error) {
// 	if len(values) == 0 {
// 		return NoDS, fmt.Errorf("no value provided for scrape option")
// 	}

// 	var result DaemonScrape
// 	for _, val := range values {
// 		switch val {
// 		case "off":
// 			result |= DSOff
// 		case "blooms":
// 			result |= DSBlooms
// 		case "index":
// 			result |= DSIndex
// 		default:
// 			return NoDS, fmt.Errorf("unknown scrape: %s", val)
// 		}
// 	}

// 	return result, nil
// }

// // EXISTING_CODE
// func (opts *DaemonOptions) Start(ready chan<- bool) {
// 	go func() {
// 		buffer := bytes.Buffer{}
// 		if err := opts.DaemonBytes(&buffer); err != nil {
// 			logger.Fatal(err)
// 		}
// 	}()

// 	time.Sleep(1 * time.Second)
// 	ready <- true
// }

// func NewDaemon(url string) *DaemonOptions {
// 	return &DaemonOptions{
// 		Silent: true,
// 		Url:    url,
// 	}
// }

// // EXISTING_CODE
