package tslibPkg

import (
	"encoding/binary"
	"errors"
	"os"
	"sort"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/araddon/dateparse"
)

type Timestamp struct {
	Bn uint32 `json:"bn"`
	Ts uint32 `json:"ts"`
}

// recordCount keeps track of whether or not we've already loaded this database
var recordCount uint64 = 0

// nRecords returns the number of records in the timestamp file
func nRecords() (uint64, error) {
	if recordCount > 0 {
		return recordCount, nil
	}

	tsPath := config.GetPathToIndex() + "ts.bin"

	fileStat, err := os.Stat(tsPath)
	if err != nil {
		return 0, err
	}
	recordCount = uint64(fileStat.Size()) / 8
	return recordCount, nil
}

var loaded = false
var memory []Timestamp

// loadTimestamps loads the timestamp data from the file into memory. If the timestamps are already loaded, we short circiut.
func loadTimestamps() error {
	if loaded {
		return nil
	}

	cnt, err := nRecords()
	if err != nil {
		return err
	}

	tsPath := config.GetPathToIndex() + "ts.bin"

	tsFile, err := os.Open(tsPath)
	if err != nil {
		return err
	}
	defer tsFile.Close()

	memory = make([]Timestamp, cnt)
	err = binary.Read(tsFile, binary.LittleEndian, memory)
	if err != nil {
		return err
	}
	loaded = true
	return nil
}

// fromTs is a local function that returns a Timestamp record given a Unix timestamp. It
// loads the timestamp file into memory if it isn't already
func fromTs(ts uint64) (*Timestamp, error) {
	cnt, err := nRecords()
	if err != nil {
		return &Timestamp{}, err
	}

	err = loadTimestamps()
	if err != nil {
		return &Timestamp{}, err
	}

	// Go docs: Search uses binary search to find and return the smallest index i in [0, n) at which f(i) is true,
	index := sort.Search(int(cnt), func(i int) bool {
		d := memory[i]
		v := uint64(d.Ts)
		return v >= ts
	})

	if uint64(memory[index].Ts) != ts {
		return &Timestamp{}, errors.New("timestamp not found")
	}

	return &memory[index], nil
}

// fromTs is a local function that returns a Timestamp record given a blockNum. It
// loads the timestamp file into memory if it isn't already
func fromBn(bn uint64) (*Timestamp, error) {
	cnt, err := nRecords()
	if err != nil {
		return &Timestamp{}, err
	}

	if bn > cnt {
		return &Timestamp{}, errors.New("invalid block number")
	}

	err = loadTimestamps()
	if err != nil {
		return &Timestamp{}, err
	}

	return &memory[bn], nil
}

// BnFromTs returns a block number given a Unit timestamp
func BnFromTs(ts uint64) (uint64, error) {
	ret, err := fromTs(ts)
	return uint64(ret.Bn), err
}

// BnFromDate returns a block number given a date string (both date.space.time and date.T.time work)
func BnFromDate(date string) (uint64, error) {
	ts, err := TsFromDate(date)
	if err != nil {
		return 0, err
	}
	ret, err := fromTs(ts)
	return uint64(ret.Bn), err
}

// TsFromBn returns a Unix timestamp given a block number
func TsFromBn(bn uint64) (uint64, error) {
	ret, err := fromBn(bn)
	return uint64(ret.Ts), err
}

// TsFromBn returns a timestamp given a date string (both date.space.time and date.T.time work)
func TsFromDate(date string) (uint64, error) {
	t, err := time.Parse("2006-01-02 15:04:05", strings.Replace(date, "T", " ", -1))
	if err != nil {
		return 0, err
	}
	return uint64(t.Unix()), nil
}

// DateFromBn returns a date string given a block number
func DateFromBn(bn uint64) (string, error) {
	ts, err := TsFromBn(bn)
	if err != nil {
		return "", err
	}
	return DateFromTs(ts)
}

// DateFromTs returns a date string given a Unix timestamp
func DateFromTs(ts uint64) (string, error) {
	// TODO: can be initialized at the start of the program as all dates are UTC
	time.Local, _ = time.LoadLocation("UTC")
	tm := time.Unix(int64(ts), 0)
	return tm.Format("2006-01-02 15:04:05 UTC"), nil
}

// NameFromBn returns the block's name (if found) given its number
func NameFromBn(needle uint64) (string, bool) {
	specials := GetSpecials()
	for _, value := range specials {
		if value.BlockNumber == needle {
			return value.Name, true
		}
	}
	return "", false
}

// BnFromName returns the block's number (if found) given its name
func BnFromName(needle string) (uint64, bool) {
	if needle == "latest" {
		return rpcClient.GetMeta(false).Latest, true
	}

	specials := GetSpecials()
	for _, value := range specials {
		if value.Name == needle {
			return value.BlockNumber, true
		}
	}

	return uint64(utils.NOPOS), false
}

// DateFromName returns the block's date (if found) given its name
func DateFromName(needle string) time.Time {
	if needle == "latest" {
		ts := rpcClient.GetBlockTimestamp(rpcClient.GetMeta(false).Latest)
		date, _ := DateFromTs(ts)
		dt, _ := dateparse.ParseLocal(date)
		return dt
	}

	specials := GetSpecials()
	for _, value := range specials {
		if value.Name == needle {
			date, _ := DateFromBn(value.BlockNumber)
			dt, _ := dateparse.ParseLocal(date)
			return dt
		}
	}

	// default to first
	dt, _ := dateparse.ParseLocal(specials[0].Date)
	return dt
}
