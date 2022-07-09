package index

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"sort"

	"github.com/ethereum/go-ethereum/common"
)

const (
	// AddrRecordWidth - size of Address Record
	AddrRecordWidth = 28
)

// AddressRecord is a single record in the Address table
type AddressRecord struct {
	Address common.Address
	Offset  uint32
	Count   uint32
}

func (addressRec *AddressRecord) ReadAddress(file *os.File) (err error) {
	return binary.Read(file, binary.LittleEndian, addressRec)
	return
}

func (chunk *ChunkData) searchForAddressRecord(address common.Address) int {
	compareFunc := func(pos int) bool {
		if pos == -1 {
			return false
		}

		if pos == int(chunk.Header.AddressCount) {
			return true
		}

		readLocation := int64(HeaderWidth + pos*AddrRecordWidth)
		_, err := chunk.File.Seek(readLocation, io.SeekStart)
		if err != nil {
			fmt.Println(err)
			return false
		}

		addressRec := AddressRecord{}
		err = addressRec.ReadAddress(chunk.File)
		if err != nil {
			fmt.Println(err)
			return false
		}

		return bytes.Compare(addressRec.Address[:], address[:]) >= 0
	}

	pos := sort.Search(int(chunk.Header.AddressCount), compareFunc)

	readLocation := int64(HeaderWidth + pos*AddrRecordWidth)
	chunk.File.Seek(readLocation, io.SeekStart)
	rec := AddressRecord{}
	err := rec.ReadAddress(chunk.File)
	if err != nil {
		return -1
	}

	if bytes.Compare(rec.Address[:], address[:]) != 0 {
		return -1
	}

	return pos
}
