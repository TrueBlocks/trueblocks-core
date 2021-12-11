// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslibPkg

import (
	"testing"
)

type Expected struct {
	name string
	bn   uint64
	ts   uint64
	date string
}

func TestLoadTimestampsPass(t *testing.T) {
	expected := []Expected{
		{name: "Block Zero", bn: 0, ts: 1438269960, date: "2015-07-30 15:26:00"},
		{name: "Block One", bn: 1, ts: 1438269988, date: "2015-07-30 15:26:28"},
		{name: "Block 1 Mil", bn: 1000000, ts: 1455404053, date: "2016-02-13 22:54:13"},
	}

	for _, e := range expected {
		bn, err := BnFromTs(e.ts)
		if err != nil {
			t.Error(err)
		} else if bn != e.bn {
			t.Error("Expected block number", e.bn, "got", bn)
		}

		bn, err = BnFromDate(e.date)
		if err != nil {
			t.Error(err)
		} else if bn != e.bn {
			t.Error("Expected block number", e.bn, "got", bn)
		}

		ts, err := TsFromBn(e.bn)
		if err != nil {
			t.Error(err)
		} else if bn != e.bn {
			t.Error("Expected timestamp", e.ts, "got", ts)
		}

		ts, err = TsFromDate(e.date)
		if err != nil {
			t.Error(err)
		} else if bn != e.bn {
			t.Error("Expected timestamp", e.ts, "got", ts)
		}

		d, err := DateFromBn(e.bn)
		if err != nil {
			t.Error(err)
		} else if d != e.date+" UTC" {
			t.Error("Expected date", e.date+" UTC", "got", d)
		}

		d, err = DateFromTs(e.ts)
		if err != nil {
			t.Error(err)
		} else if d != e.date+" UTC" {
			t.Error("Expected date", e.date+" UTC", "got", d)
		}
	}
}

func TestLoadTimestampsFail(t *testing.T) {
	expected := []Expected{
		{name: "Bad Block", bn: 100000000, ts: 12, date: "2012-02-13 22:54:13"},
	}

	for _, e := range expected {
		_, err := BnFromTs(e.ts)
		if err == nil {
			t.Error("Expected failure for BnFromTs")
		}

		_, err = BnFromDate(e.date)
		if err == nil {
			t.Error("Expected failure for BnFromDate")
		}

		_, err = TsFromBn(e.bn)
		if err == nil {
			t.Error("Expected failure for TsFromBn")
		}

		// Never fails
		// _, err = TsFromDate(e.date)
		// if err == nil {
		// 	t.Error("Expected failure for TsFromDate")
		// }

		_, err = DateFromBn(e.bn)
		if err == nil {
			t.Error("Expected failure for DateFromBn")
		}

		// Never fails
		// _, err = DateFromTs(e.ts)
		// if err == nil {
		// 	t.Error("Expected failure for DateFromTs")
		// }
	}
}
