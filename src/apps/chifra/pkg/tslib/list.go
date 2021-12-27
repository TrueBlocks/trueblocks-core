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
		TimeStamp:   1438269960,
		Date:        "2015-07-30 15:26:00",
	},
	{
		BlockNumber: 46147,
		Name:        "firstTrans",
		TimeStamp:   1438918233,
		Date:        "2015-08-07 03:30:33",
	},
	{
		BlockNumber: 50111,
		Name:        "firstContract",
		TimeStamp:   1438984075,
		Date:        "2015-08-07 21:47:55",
	},
	{
		BlockNumber: 200000,
		Name:        "iceage",
		TimeStamp:   1441661589,
		Date:        "2015-09-07 21:33:09",
	},
	{
		BlockNumber: 543626,
		Name:        "devcon1",
		TimeStamp:   1447577992,
		Date:        "2015-11-15 08:59:52",
	},
	{
		BlockNumber: 1150000,
		Name:        "homestead",
		TimeStamp:   1457981393,
		Date:        "2016-03-14 18:49:53",
	},
	{
		BlockNumber: 1428756,
		Name:        "daofund",
		TimeStamp:   1461980573,
		Date:        "2016-04-30 01:42:53",
	},
	{
		BlockNumber: 1718497,
		Name:        "daohack",
		TimeStamp:   1466134488,
		Date:        "2016-06-17 03:34:48",
	},
	{
		BlockNumber: 1920000,
		Name:        "daofork",
		TimeStamp:   1469020840,
		Date:        "2016-07-20 13:20:40",
	},
	{
		BlockNumber: 2286910,
		Name:        "devcon2",
		TimeStamp:   1474275599,
		Date:        "2016-09-19 08:59:59",
	},
	{
		BlockNumber: 2463000,
		Name:        "tangerine",
		TimeStamp:   1476796771,
		Date:        "2016-10-18 13:19:31",
	},
	{
		BlockNumber: 2675000,
		Name:        "spurious",
		TimeStamp:   1479831344,
		Date:        "2016-11-22 16:15:44",
	},
	{
		BlockNumber: 2717576,
		Name:        "stateclear",
		TimeStamp:   1480448309,
		Date:        "2016-11-29 19:38:29",
	},
	{
		BlockNumber: 3265360,
		Name:        "eea",
		TimeStamp:   1488272400,
		Date:        "2017-02-28 09:00:00",
	},
	{
		BlockNumber: 3327417,
		Name:        "ens2",
		TimeStamp:   1489165544,
		Date:        "2017-03-10 17:05:44",
	},
	{
		BlockNumber: 4041179,
		Name:        "parityhack1",
		TimeStamp:   1500417203,
		Date:        "2017-07-18 22:33:23",
	},
	{
		BlockNumber: 4370000,
		Name:        "byzantium",
		TimeStamp:   1508131331,
		Date:        "2017-10-16 05:22:11",
	},
	{
		BlockNumber: 4469339,
		Name:        "devcon3",
		TimeStamp:   1509526798,
		Date:        "2017-11-01 08:59:58",
	},
	{
		BlockNumber: 4501969,
		Name:        "parityhack2",
		TimeStamp:   1509981921,
		Date:        "2017-11-06 15:25:21",
	},
	{
		BlockNumber: 4605167,
		Name:        "kitties",
		TimeStamp:   1511415679,
		Date:        "2017-11-23 05:41:19",
	},
	{
		BlockNumber: 4620855,
		Name:        "makerdao",
		TimeStamp:   1511634257,
		Date:        "2017-11-25 18:24:17",
	},
	{
		BlockNumber: 6610517,
		Name:        "devcon4",
		TimeStamp:   1540890000,
		Date:        "2018-10-30 09:00:00",
	},
	{
		BlockNumber: 6627917,
		Name:        "uniswap",
		TimeStamp:   1541137042,
		Date:        "2018-11-02 05:37:22",
	},
	{
		BlockNumber: 7280000,
		Name:        "constantinople",
		TimeStamp:   1551383524,
		Date:        "2019-02-28 19:52:04",
	},
	{
		BlockNumber: 8700401,
		Name:        "devcon5",
		TimeStamp:   1570525179,
		Date:        "2019-10-08 08:59:39",
	},
	{
		BlockNumber: 8928158,
		Name:        "mcdai",
		TimeStamp:   1573672677,
		Date:        "2019-11-13 19:17:57",
	},
	{
		BlockNumber: 9069000,
		Name:        "istanbul",
		TimeStamp:   1575764709,
		Date:        "2019-12-08 00:25:09",
	},
	{
		BlockNumber: 9200000,
		Name:        "muirglacier",
		TimeStamp:   1577953849,
		Date:        "2020-01-02 08:30:49",
	},
	{
		BlockNumber: 12244000,
		Name:        "berlin",
		TimeStamp:   1618481223,
		Date:        "2021-04-15 10:07:03",
	},
	{
		BlockNumber: 12965000,
		Name:        "london",
		TimeStamp:   1628166822,
		Date:        "2021-08-05 12:33:42",
	},
	{
		BlockNumber: 13773000,
		Name:        "arrowglacier",
		TimeStamp:   1639079723,
		Date:        "2021-12-09 19:55:23",
	},
}

func GetSpecials() []NamedBlock {
	return specials
}

func IsSpecialBlock(needle string) bool {
	_, found := BnFromName(needle)
	return found
}
