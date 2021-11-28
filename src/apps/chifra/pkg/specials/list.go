// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package specials

import (
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/araddon/dateparse"
)

type NamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name"`
}
type NamedBlocks = []NamedBlock

var specials = NamedBlocks{
	{
		BlockNumber: 0,
		TimeStamp:   1438269960,
		Name:        "first",
		Date:        "2015-07-30 15:26:00 UTC",
	},
	{
		BlockNumber: 46147,
		TimeStamp:   1438918233,
		Name:        "firstTrans",
		Date:        "2015-08-07 03:30:33 UTC",
	},
	{
		BlockNumber: 50111,
		TimeStamp:   1438984075,
		Name:        "firstContract",
		Date:        "2015-08-07 21:47:55 UTC",
	},
	{
		BlockNumber: 200000,
		TimeStamp:   1441661589,
		Name:        "iceage",
		Date:        "2015-09-07 21:33:09 UTC",
	},
	{
		BlockNumber: 543626,
		TimeStamp:   1447577992,
		Name:        "devcon1",
		Date:        "2015-11-15 08:59:52 UTC",
	},
	{
		BlockNumber: 1150000,
		TimeStamp:   1457981393,
		Name:        "homestead",
		Date:        "2016-03-14 18:49:53 UTC",
	},
	{
		BlockNumber: 1428756,
		TimeStamp:   1461980573,
		Name:        "daofund",
		Date:        "2016-04-30 01:42:53 UTC",
	},
	{
		BlockNumber: 1718497,
		TimeStamp:   1466134488,
		Name:        "daohack",
		Date:        "2016-06-17 03:34:48 UTC",
	},
	{
		BlockNumber: 1920000,
		TimeStamp:   1469020840,
		Name:        "daofork",
		Date:        "2016-07-20 13:20:40 UTC",
	},
	{
		BlockNumber: 2286910,
		TimeStamp:   1474275599,
		Name:        "devcon2",
		Date:        "2016-09-19 08:59:59 UTC",
	},
	{
		BlockNumber: 2463000,
		TimeStamp:   1476796771,
		Name:        "tangerine",
		Date:        "2016-10-18 13:19:31 UTC",
	},
	{
		BlockNumber: 2675000,
		TimeStamp:   1479831344,
		Name:        "spurious",
		Date:        "2016-11-22 16:15:44 UTC",
	},
	{
		BlockNumber: 2717576,
		TimeStamp:   1480448309,
		Name:        "stateclear",
		Date:        "2016-11-29 19:38:29 UTC",
	},
	{
		BlockNumber: 3265360,
		TimeStamp:   1488272400,
		Name:        "eea",
		Date:        "2017-02-28 09:00:00 UTC",
	},
	{
		BlockNumber: 3327417,
		TimeStamp:   1489165544,
		Name:        "ens2",
		Date:        "2017-03-10 17:05:44 UTC",
	},
	{
		BlockNumber: 4041179,
		TimeStamp:   1500417203,
		Name:        "parityhack1",
		Date:        "2017-07-18 22:33:23 UTC",
	},
	{
		BlockNumber: 4370000,
		TimeStamp:   1508131331,
		Name:        "byzantium",
		Date:        "2017-10-16 05:22:11 UTC",
	},
	{
		BlockNumber: 4469339,
		TimeStamp:   1509526798,
		Name:        "devcon3",
		Date:        "2017-11-01 08:59:58 UTC",
	},
	{
		BlockNumber: 4501969,
		TimeStamp:   1509981921,
		Name:        "parityhack2",
		Date:        "2017-11-06 15:25:21 UTC",
	},
	{
		BlockNumber: 4605167,
		TimeStamp:   1511415679,
		Name:        "kitties",
		Date:        "2017-11-23 05:41:19 UTC",
	},
	{
		BlockNumber: 4620855,
		TimeStamp:   1511634257,
		Name:        "makerdao",
		Date:        "2017-11-25 18:24:17 UTC",
	},
	{
		BlockNumber: 6610517,
		TimeStamp:   1540890000,
		Name:        "devcon4",
		Date:        "2018-10-30 09:00:00 UTC",
	},
	{
		BlockNumber: 6627917,
		TimeStamp:   1541137042,
		Name:        "uniswap",
		Date:        "2018-11-02 05:37:22 UTC",
	},
	{
		BlockNumber: 7280000,
		TimeStamp:   1551383524,
		Name:        "constantinople",
		Date:        "2019-02-28 19:52:04 UTC",
	},
	{
		BlockNumber: 8700401,
		TimeStamp:   1570525179,
		Name:        "devcon5",
		Date:        "2019-10-08 08:59:39 UTC",
	},
	{
		BlockNumber: 8928158,
		TimeStamp:   1573672677,
		Name:        "mcdai",
		Date:        "2019-11-13 19:17:57 UTC",
	},
	{
		BlockNumber: 9069000,
		TimeStamp:   1575764709,
		Name:        "istanbul",
		Date:        "2019-12-08 00:25:09 UTC",
	},
	{
		BlockNumber: 9200000,
		TimeStamp:   1577953849,
		Name:        "muirglacier",
		Date:        "2020-01-02 08:30:49 UTC",
	},
	{
		BlockNumber: 12244000,
		TimeStamp:   1618481223,
		Name:        "berlin",
		Date:        "2021-04-15 10:07:03 UTC",
	},
	{
		BlockNumber: 12965000,
		TimeStamp:   1628166822,
		Name:        "london",
		Date:        "2021-08-05 12:33:42 UTC",
	},
	{
		BlockNumber: 13773000,
		TimeStamp:   1639022046,
		Name:        "arrowglacier (est)",
		Date:        "2021-12-09 03:54:06 UTC",
	},
}

func GetSpecials(testMode bool) NamedBlocks {
	if !testMode {
		return specials
	}

	latest := rpcClient.GetMeta(false).Client
	var ret = NamedBlocks{}
	for _, item := range specials {
		if item.BlockNumber < latest {
			ret = append(ret, item)
		}
	}
	return ret
}

func IsStringSpecialBlock(needle string) bool {
	_, found := GetValueByName(needle)
	return found
}

func GetNameByValue(needle uint64) (string, bool) {
	for _, value := range specials {
		if value.BlockNumber == needle {
			return value.Name, true
		}
	}
	return "", false
}

func GetValueByName(needle string) (uint64, bool) {
	for _, value := range specials {
		if value.Name == needle {
			return value.BlockNumber, true
		}
	}
	return uint64(utils.NOPOS), false
}

func GetDateByName(needle string) time.Time {
	for _, value := range specials {
		if value.Name == needle {
			dt, _ := dateparse.ParseLocal(value.Date)
			return dt
		}
	}
	// default to first
	dt, _ := dateparse.ParseLocal(specials[0].Date)
	return dt
}

// Useful: https://www.golangprograms.com/go-language/struct.html
