package chunk

import (
	"bytes"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"os"
)

// import (
// 	"bytes"
// 	"encoding/binary"
// 	"encoding/hex"
// 	"fmt"
// 	"os"
// )

const (
	// AddressLength = number of bytes of an address
	AddressLength = 20
	// MagicNumber is used to check data validity
	MagicNumber = 0xdeadbeef
	// HashLength = number of bytes in a HASH
	HashLength = 32
	// HeaderSize - size of Header Record
// 	HeaderSize = 44
// 	// AddressSize - size of Address Record
// 	AddressSize = 28
// 	// AppearanceSize - size of Appearance Record
// 	AppearanceSize = 8
)

type EthHash [HashLength]byte

// TODO BOGUS -- LOT'S LEFT TO DO HERE
// type Chunk struct {
// 	*os.File
// 	Header                  HeaderRecord
// 	FirstAddressPosition    int64
// 	FirstAppearancePosition int64
// }

// HeaderRecord is the header record in the file. Carries sizes for the other tables
// format on disc | Magic (4 bytes) | Hash (32 bytes) | AddressCount (4 bytes) | AppearanceCount (4 bytes) | total: 44 bytes
type HeaderRecord struct {
	Magic           uint32
	Hash            EthHash
	AddressCount    uint32
	AppearanceCount uint32
}

func ReadHeader(file *os.File) (header HeaderRecord, err error) {
	err = binary.Read(file, binary.LittleEndian, &header)
	if err != nil {
		return
	}

	if header.Magic != MagicNumber {
		return header, fmt.Errorf("magic number is file %s is incorrect, expected %d, got %d", file.Name(), MagicNumber, header.Magic)
	}

	return
}

// AddressRecord is a single record in the Address table
type AddressRecord struct {
	Bytes  EthAddress
	Offset uint32
	Count  uint32
}

func ReadAddressRecord(f *os.File) (addressRec AddressRecord, err error) {
	err = binary.Read(f, binary.LittleEndian, &addressRec)
	return
}

// AppearanceRecord is a single record in the Appearance table
type AppearanceRecord struct {
	BlockNumber   uint32 `json:"blockNumber"`
	TransactionId uint32 `json:"transactionIndex"`
}

// func validateFileSize(file *os.File) error {
// 	header, err := getHeader(file)
// 	if err != nil {
// 		return err
// 	}

// 	addressTableSize := AddressSize * header.AddressCount
// 	appearanceTableSize := AppearanceSize * header.AppearanceCount
// 	expectedSize := HeaderSize + addressTableSize + appearanceTableSize

// 	stat, err := file.Stat()
// 	if err != nil {
// 		return err
// 	}

// 	if stat.Size() != int64(expectedSize) {
// 		return fmt.Errorf("wrong file size, expected %d, but got %d", expectedSize, stat.Size())
// 	}

// 	return nil
// }

// func GetAddressTablePosition() int64 {
// 	return HeaderSize
// }

// func GetAppearanceTablePosition(header HeaderRecord) int64 {
// 	return GetAddressTablePosition() + (int64(header.AddressCount) * AddressSize)
// }

// func Load(path string) (Chunk, error) {
// 	file, err := os.Open(path)
// 	if err != nil {
// 		return Chunk{}, err
// 	}

// 	header, err := getHeader(file)
// 	if err != nil {
// 		return Chunk{}, err
// 	}
// 	if header.Magic != MagicNumber {
// 		return Chunk{}, fmt.Errorf("wrong magic number %x", header.Magic)
// 	}

// 	// validate file size here?

// 	chunk := Chunk{
// 		Header:                  header,
// 		FirstAddressPosition:    GetAddressTablePosition(),
// 		FirstAppearancePosition: GetAppearanceTablePosition(header),
// 	}
// 	chunk.File = file

// 	return chunk, nil
// }

// func validateStructure(chunk Chunk) error {
// 	// Try to read first address
// 	_, err := chunk.File.Seek(chunk.FirstAddressPosition, 0)
// 	if err != nil {
// 		return err
// 	}

// 	err = binary.Read(chunk.File, binary.LittleEndian, &AddressRecord{})
// 	if err != nil {
// 		return err
// 	}

// 	// Try to read first appearance
// 	_, err = chunk.File.Seek(chunk.FirstAppearancePosition, 0)
// 	if err != nil {
// 		return err
// 	}

// 	err = binary.Read(chunk.File, binary.LittleEndian, &AppearanceRecord{})
// 	if err != nil {
// 		return err
// 	}

// 	return nil
// }

// func GetAddress(chunk Chunk, number int) (AddressRecord, error) {
// 	base := 0
// 	if number < 0 {
// 		base = int(chunk.Header.AddressCount)
// 	}
// 	addressPosition := chunk.FirstAddressPosition + int64((base+number)*AddressLength)

// 	record := AddressRecord{}
// 	_, err := chunk.File.Seek(addressPosition, 0)
// 	if err != nil {
// 		return record, err
// 	}
// 	err = binary.Read(chunk.File, binary.LittleEndian, &record)
// 	if err != nil {
// 		return record, err
// 	}

// 	return record, nil
// }

// EthAddress is 20 bytes
type EthAddress [AddressLength]byte

// Compares two addresses
func (a EthAddress) Compare(b EthAddress) int {
	return bytes.Compare(a[:], b[:])
}

func (a *EthAddress) setBytes(b []byte) {
	if len(b) > len(a) {
		b = b[len(b)-AddressLength:]
	}
	copy(a[AddressLength-len(b):], b)
}

// NewAddressFromHex Converts a hex string to an address
func NewAddressFromHex(hexString string) (newAddress EthAddress, err error) {
	if hexString[:2] == "0x" {
		hexString = hexString[2:]
	}
	hexBytes, err := hex.DecodeString(hexString)
	if err != nil {
		return EthAddress{}, err
	}
	newAddress.setBytes(hexBytes)
	return
}
