package tslibPkg

import (
	"encoding/binary"
	"errors"
	"os"
	"sort"
	"strings"
	"time"
)

const tsPath = "/Users/jrush/Development/trueblocks-index/wild.0/unchained/ts.bin"

type Timestamp struct {
	Bn uint32 `json:"bn"`
	Ts uint32 `json:"ts"`
}

var recordCount uint64 = 0

func nRecords() (uint64, error) {
	if recordCount > 0 {
		return recordCount, nil
	}
	fileStat, err := os.Stat(tsPath)
	if err != nil {
		return 0, err
	}
	recordCount = uint64(fileStat.Size()) / 8
	return recordCount, nil
}

var loaded = false
var memory []Timestamp

func loadTimestamps() error {
	if loaded {
		return nil
	}

	cnt, err := nRecords()
	if err != nil {
		return err
	}

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

func fromTs(ts uint64) (*Timestamp, error) {
	cnt, err := nRecords()
	if err != nil {
		return &Timestamp{}, err
	}

	err = loadTimestamps()
	if err != nil {
		return &Timestamp{}, err
	}

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

func BnFromTs(ts uint64) (uint64, error) {
	ret, err := fromTs(ts)
	return uint64(ret.Bn), err
}

func BnFromDate(date string) (uint64, error) {
	ts, err := TsFromDate(date)
	if err != nil {
		return 0, err
	}
	ret, err := fromTs(ts)
	return uint64(ret.Bn), err
}

func TsFromBn(bn uint64) (uint64, error) {
	ret, err := fromBn(bn)
	return uint64(ret.Ts), err
}

func TsFromDate(date string) (uint64, error) {
	t, err := time.Parse("2006-01-02 15:04:05", strings.Replace(date, "T", " ", -1))
	if err != nil {
		return 0, err
	}
	return uint64(t.Unix()), nil
}

func DateFromBn(bn uint64) (string, error) {
	ts, err := TsFromBn(bn)
	if err != nil {
		return "", err
	}
	return DateFromTs(ts)
}

func DateFromTs(ts uint64) (string, error) {
	// TODO: can be initialized at the start of the program as all dates are UTC
	time.Local, _ = time.LoadLocation("UTC")
	tm := time.Unix(int64(ts), 0)
	return tm.Format("2006-01-02T15:04:05"), nil
}
