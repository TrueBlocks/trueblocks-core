package index

import (
	"encoding/binary"
	"fmt"
	"os"
)

func validateFileSize(file *os.File) error {
	header, err := ReadHeader(file)
	if err != nil {
		return err
	}

	stat, err := file.Stat()
	if err != nil {
		return err
	}

	addressTableSize := AddrRecordWidth * header.AddressCount
	appearanceTableSize := AppRecordWidth * header.AppearanceCount
	expectedSize := HeaderWidth + addressTableSize + appearanceTableSize

	if stat.Size() != int64(expectedSize) {
		return fmt.Errorf("wrong file size, expected %d, but got %d", expectedSize, stat.Size())
	}

	return nil
}

func validateStructure(chunk IndexChunk) error {
	// Try to read first address
	_, err := chunk.File.Seek(chunk.AddrTableStart, 0)
	if err != nil {
		return err
	}
	err = binary.Read(chunk.File, binary.LittleEndian, &AddressRecord{})
	if err != nil {
		return err
	}

	// Try to read first appearance
	_, err = chunk.File.Seek(chunk.AppTableStart, 0)
	if err != nil {
		return err
	}
	err = binary.Read(chunk.File, binary.LittleEndian, &AppearanceRecord{})
	if err != nil {
		return err
	}

	return nil
}
