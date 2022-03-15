package chunk

import (
	"encoding/binary"
	"fmt"
	"os"
)

const (
	// AddressLength = number of bytes of an address
	AddressLength = 20
	// MagicNumber is used to check data validity
	MagicNumber = 0xdeadbeaf
	// HashLength = number of bytes in a HASH
	HashLength = 32
	// HeaderSize - size of Header Record
	HeaderSize = 44
	// AddressSize - size of Address Record
	AddressSize = 28
	// AppearanceSize - size of Appearance Record
	AppearanceSize = 8
)

type EthHash [HashLength]byte
type EthAddress [AddressLength]byte

type Chunk struct {
	*os.File
	Header                  Header
	FirstAddressPosition    int64
	FirstAppearancePosition int64
}

// Header of the File
type Header struct {
	Magic               uint32
	Hash                EthHash
	NumberOfAddresses   uint32
	NumberOfAppearances uint32
}

type AddressRecord struct {
	Address         EthAddress
	StartRecord     uint32
	NumberOfRecords uint32
}

type AppearanceRecord struct {
	BlockNumber   uint32
	TransactionId uint32
}

func getHeader(file *os.File) (Header, error) {
	header := Header{}
	err := binary.Read(file, binary.LittleEndian, &header)
	return header, err
}

func validateFileSize(file *os.File) error {
	header, err := getHeader(file)
	if err != nil {
		return err
	}

	addressTableSize := AddressSize * header.NumberOfAddresses
	appearanceTableSize := AppearanceSize * header.NumberOfAppearances
	expectedSize := HeaderSize + addressTableSize + appearanceTableSize

	stat, err := file.Stat()
	if err != nil {
		return err
	}

	if stat.Size() != int64(expectedSize) {
		return fmt.Errorf("wrong file size, expected %d, but got %d", expectedSize, stat.Size())
	}

	return nil
}

func GetAddressTablePosition() int64 {
	return HeaderSize
}

func GetAppearanceTablePosition(header Header) int64 {
	return GetAddressTablePosition() + (int64(header.NumberOfAddresses) * AddressSize)
}

func Load(path string) (Chunk, error) {
	file, err := os.Open(path)
	if err != nil {
		return Chunk{}, err
	}

	header, err := getHeader(file)
	if err != nil {
		return Chunk{}, err
	}
	if header.Magic != MagicNumber {
		return Chunk{}, fmt.Errorf("wrong magic number %x", header.Magic)
	}

	// validate file size here?

	chunk := Chunk{
		Header:                  header,
		FirstAddressPosition:    GetAddressTablePosition(),
		FirstAppearancePosition: GetAppearanceTablePosition(header),
	}
	chunk.File = file

	return chunk, nil
}

func validateStructure(chunk Chunk) error {
	// Try to read first address
	_, err := chunk.File.Seek(chunk.FirstAddressPosition, 0)
	if err != nil {
		return err
	}

	err = binary.Read(chunk.File, binary.LittleEndian, &AddressRecord{})
	if err != nil {
		return err
	}

	// Try to read first appearance
	_, err = chunk.File.Seek(chunk.FirstAppearancePosition, 0)
	if err != nil {
		return err
	}

	err = binary.Read(chunk.File, binary.LittleEndian, &AppearanceRecord{})
	if err != nil {
		return err
	}

	return nil
}

func GetAddress(chunk Chunk, number int) (AddressRecord, error) {
	base := 0
	if number < 0 {
		base = int(chunk.Header.NumberOfAddresses)
	}
	addressPosition := chunk.FirstAddressPosition + int64((base+number)*AddressLength)

	record := AddressRecord{}
	_, err := chunk.File.Seek(addressPosition, 0)
	if err != nil {
		return record, err
	}
	err = binary.Read(chunk.File, binary.LittleEndian, &record)
	if err != nil {
		return record, err
	}

	return record, nil
}
