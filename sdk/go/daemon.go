// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"io"
	"net/url"

	daemon "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type DaemonOptions struct {
	Url     string
	Api     DaemonApi
	Scrape  DaemonScrape
	Monitor bool
	Grpc    bool
	Port    string
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Daemon implements the chifra daemon command for the SDK.
func (opts *DaemonOptions) Daemon(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return daemon.Daemon(w, values)
}

type DaemonApi int

const (
	NoDA DaemonApi = iota
	DAOff
	DAOn
)

func (v DaemonApi) String() string {
	return []string{
		"noda",
		"off",
		"on",
	}[v]
}

type DaemonScrape int

const (
	NoDS DaemonScrape = iota
	DSOff
	DSBlooms
	DSIndex
)

func (v DaemonScrape) String() string {
	return []string{
		"nods",
		"off",
		"blooms",
		"index",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

