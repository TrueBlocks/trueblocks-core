// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"bytes"
	"encoding/json"
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	// EXISTING_CODE
)

type DaemonOptions struct {
	Url       string            `json:"url,omitempty"`
	Api       DaemonApi         `json:"api,omitempty"`
	Scrape    DaemonScrape      `json:"scrape,omitempty"`
	Monitor   bool              `json:"monitor,omitempty"`
	Silent    bool              `json:"silent,omitempty"`
	RenderCtx *output.RenderCtx `json:"-"`
	Globals
}

// String implements the stringer interface
func (opts DaemonOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

type DaemonApi int

const (
	NoDA  DaemonApi = 0
	DAOff           = 1 << iota
	DAOn
)

func (v DaemonApi) String() string {
	switch v {
	case NoDA:
		return "none"
	}

	var m = map[DaemonApi]string{
		DAOff: "off",
		DAOn:  "on",
	}

	var ret []string
	for _, val := range []DaemonApi{DAOff, DAOn} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromDaemonApi(values []string) (DaemonApi, error) {
	if len(values) == 0 {
		return NoDA, fmt.Errorf("no value provided for api option")
	}

	var result DaemonApi
	for _, val := range values {
		switch val {
		case "off":
			result |= DAOff
		case "on":
			result |= DAOn
		default:
			return NoDA, fmt.Errorf("unknown api: %s", val)
		}
	}

	return result, nil
}

type DaemonScrape int

const (
	NoDS  DaemonScrape = 0
	DSOff              = 1 << iota
	DSBlooms
	DSIndex
)

func (v DaemonScrape) String() string {
	switch v {
	case NoDS:
		return "none"
	}

	var m = map[DaemonScrape]string{
		DSOff:    "off",
		DSBlooms: "blooms",
		DSIndex:  "index",
	}

	var ret []string
	for _, val := range []DaemonScrape{DSOff, DSBlooms, DSIndex} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromDaemonScrape(values []string) (DaemonScrape, error) {
	if len(values) == 0 {
		return NoDS, fmt.Errorf("no value provided for scrape option")
	}

	var result DaemonScrape
	for _, val := range values {
		switch val {
		case "off":
			result |= DSOff
		case "blooms":
			result |= DSBlooms
		case "index":
			result |= DSIndex
		default:
			return NoDS, fmt.Errorf("unknown scrape: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
func (opts *DaemonOptions) Start(ready chan<- bool) {
	go func() {
		in := opts.toInternal()
		buffer := bytes.Buffer{}
		if err := in.DaemonBytes(&buffer); err != nil {
			logger.Fatal(err)
		}
	}()

	time.Sleep(1 * time.Second)
	ready <- true
}

func NewDaemon(url string) *DaemonOptions {
	return &DaemonOptions{
		Silent: true,
		Url:    url,
	}
}

// EXISTING_CODE
