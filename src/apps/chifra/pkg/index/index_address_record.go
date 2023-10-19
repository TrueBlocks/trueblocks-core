package index

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

const (
	// AddrRecordWidth - size of Address Record
	AddrRecordWidth = 28
)

// AddressRecord is a single record in the Address table
type AddressRecord struct {
	Address base.Address `json:"address"`
	Offset  uint32       `json:"offset"`
	Count   uint32       `json:"count"`
}

func (addressRec *AddressRecord) ReadAddress(file *os.File) (err error) {
	return binary.Read(file, binary.LittleEndian, addressRec)
}

func (chunk *ChunkData) searchForAddressRecord(address base.Address) int {
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

		return bytes.Compare(addressRec.Address.Bytes(), address.Bytes()) >= 0
	}

	pos := sort.Search(int(chunk.Header.AddressCount), compareFunc)

	readLocation := int64(HeaderWidth + pos*AddrRecordWidth)
	_, _ = chunk.File.Seek(readLocation, io.SeekStart)
	rec := AddressRecord{}
	err := rec.ReadAddress(chunk.File)
	if err != nil {
		return -1
	}

	if !bytes.Equal(rec.Address.Bytes(), address.Bytes()) {
		return -1
	}

	return pos
}
