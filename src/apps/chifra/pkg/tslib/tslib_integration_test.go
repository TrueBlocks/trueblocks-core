//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslibPkg

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Expected struct {
	name string
	bn   uint64
	ts   uint64
	date string
}

func TestLoadTimestampsPass(t *testing.T) {
	expected := []Expected{
		{name: "Block Zero", bn: 1, ts: utils.BlockOneTs, date: "2015-07-30 15:26:28"},
		{name: "Block 1 Mil", bn: 1000000, ts: 1455404053, date: "2016-02-13 22:54:13"},
	}

	for _, e := range expected {
		bn, err := FromTsToBn(GetTestChain(), e.ts)
		if err != nil {
			t.Error(err)
		} else if bn != e.bn {
			t.Error("Expected block number", e.bn, "got", bn)
		}

		bn, err = FromDateToBn(GetTestChain(), e.date)
		if err != nil {
			t.Error(err)
		} else if bn != e.bn {
			t.Error("Expected block number", e.bn, "got", bn)
		}

		ts, err := FromBnToTs(GetTestChain(), e.bn)
		if err != nil {
			t.Error(err)
		} else if bn != e.bn {
			t.Error("Expected timestamp", e.ts, "got", ts)
		}

		ts, err = FromDateToTs(e.date)
		if err != nil {
			t.Error(err)
		} else if bn != e.bn {
			t.Error("Expected timestamp", e.ts, "got", ts)
		}

		d, err := FromBnToDate(GetTestChain(), e.bn)
		s := d.Format("YYYY-MM-DD HH:mm:ss")
		if err != nil {
			t.Error(err)
		} else if s != e.date {
			t.Error("Expected date", e.date, "got", d)
		}

		d, err = FromTsToDate(e.ts)
		s = d.Format("YYYY-MM-DD HH:mm:ss")
		if err != nil {
			t.Error(err)
		} else if s != e.date {
			t.Error("Expected date", e.date, "got", d)
		}
	}
}

func TestLoadTimestampsFail(t *testing.T) {
	expected := []Expected{
		{name: "Bad Block", bn: 100000000, ts: 12, date: "2012-02-13 22:54:13"},
	}

	for _, e := range expected {
		_, err := FromTsToBn(GetTestChain(), e.ts)
		if err == nil {
			t.Error("Expected failure for FromTsToBn")
		}

		_, err = FromDateToBn(GetTestChain(), e.date)
		if err == nil {
			t.Error("Expected failure for FromDateToBn")
		}

		_, err = FromBnToTs(GetTestChain(), e.bn)
		if err == nil {
			t.Error("Expected failure for FromBnToTs")
		}

		// Never fails
		// _, err = FromDateToTs(e.date)
		// if err == nil {
		// 	t.Error("Expected failure for FromDateToTs")
		// }

		_, err = FromBnToDate(GetTestChain(), e.bn)
		if err == nil {
			t.Error("Expected failure for FromBnToDate")
		}

		// Never fails
		// _, err = FromTsToDate(e.ts)
		// if err == nil {
		// 	t.Error("Expected failure for FromTsToDate")
		// }
	}
}

// TODO: Test FromDateToBn with periods of hourly, daily, weekly, monthly, and annually
