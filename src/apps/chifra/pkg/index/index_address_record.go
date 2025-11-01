package index

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

const (
	// AddrRecordWidth - size of Address Record
	AddrRecordWidth = 28
)

func (chunk *Index) searchForAddressRecord(address base.Address) int {
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

		addressRec := types.AddrRecord{}
		if err = binary.Read(chunk.File, binary.LittleEndian, &addressRec); err != nil {
			fmt.Println(err)
			return false
		}

		return bytes.Compare(addressRec.Address.Bytes(), address.Bytes()) >= 0
	}

	pos := sort.Search(int(chunk.Header.AddressCount), compareFunc)

	readLocation := int64(HeaderWidth + pos*AddrRecordWidth)
	_, _ = chunk.File.Seek(readLocation, io.SeekStart)
	rec := types.AddrRecord{}
	if err := binary.Read(chunk.File, binary.LittleEndian, &rec); err != nil {
		return -1
	}

	if !bytes.Equal(rec.Address.Bytes(), address.Bytes()) {
		return -1
	}

	return pos
}
