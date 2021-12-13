// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslibPkg

type NamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name"`
}

var specials = []NamedBlock{
	{
		BlockNumber: 0,
		Name:        "first",
	},
	{
		BlockNumber: 46147,
		Name:        "firstTrans",
	},
	{
		BlockNumber: 50111,
		Name:        "firstContract",
	},
	{
		BlockNumber: 200000,
		Name:        "iceage",
	},
	{
		BlockNumber: 543626,
		Name:        "devcon1",
	},
	{
		BlockNumber: 1150000,
		Name:        "homestead",
	},
	{
		BlockNumber: 1428756,
		Name:        "daofund",
	},
	{
		BlockNumber: 1718497,
		Name:        "daohack",
	},
	{
		BlockNumber: 1920000,
		Name:        "daofork",
	},
	{
		BlockNumber: 2286910,
		Name:        "devcon2",
	},
	{
		BlockNumber: 2463000,
		Name:        "tangerine",
	},
	{
		BlockNumber: 2675000,
		Name:        "spurious",
	},
	{
		BlockNumber: 2717576,
		Name:        "stateclear",
	},
	{
		BlockNumber: 3265360,
		Name:        "eea",
	},
	{
		BlockNumber: 3327417,
		Name:        "ens2",
	},
	{
		BlockNumber: 4041179,
		Name:        "parityhack1",
	},
	{
		BlockNumber: 4370000,
		Name:        "byzantium",
	},
	{
		BlockNumber: 4469339,
		Name:        "devcon3",
	},
	{
		BlockNumber: 4501969,
		Name:        "parityhack2",
	},
	{
		BlockNumber: 4605167,
		Name:        "kitties",
	},
	{
		BlockNumber: 4620855,
		Name:        "makerdao",
	},
	{
		BlockNumber: 6610517,
		Name:        "devcon4",
	},
	{
		BlockNumber: 6627917,
		Name:        "uniswap",
	},
	{
		BlockNumber: 7280000,
		Name:        "constantinople",
	},
	{
		BlockNumber: 8700401,
		Name:        "devcon5",
	},
	{
		BlockNumber: 8928158,
		Name:        "mcdai",
	},
	{
		BlockNumber: 9069000,
		Name:        "istanbul",
	},
	{
		BlockNumber: 9200000,
		Name:        "muirglacier",
	},
	{
		BlockNumber: 12244000,
		Name:        "berlin",
	},
	{
		BlockNumber: 12965000,
		Name:        "london",
	},
	{
		BlockNumber: 13773000,
		Name:        "arrowglacier",
	},
}

func GetSpecials() []NamedBlock {
	if specials[0].TimeStamp == 0 {
		for i := 0; i < len(specials); i++ {
			specials[i].TimeStamp, _ = TsFromBn(specials[i].BlockNumber)
			specials[i].Date, _ = DateFromTs(specials[i].TimeStamp)
		}
	}

	return specials
}

func IsSpecialBlock(needle string) bool {
	_, found := BnFromName(needle)
	return found
}
