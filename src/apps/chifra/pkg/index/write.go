package index

// import (
// 	"bytes"
// 	"encoding/binary"
// 	"encoding/hex"

// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
// 	"github.com/ethereum/go-ethereum/common"
// )

// func Write(addressToAppearances map[string][]AppearanceRecord) (*bytes.Buffer, error) {
// 	// First, prepare some placeholders for our data
// 	addressTable := make([]AddressRecord, 0, len(addressToAppearances))
// 	appearanceCount := 0
// 	// We need to write appearance table after address table, but we can build it first,
// 	// so we will use another buffer to store it as bytes
// 	appearanceTableBuf := bytes.Buffer{}

// 	for address, appearances := range addressToAppearances {
// 		// Convert string with an address into bytes
// 		addressHex, err := hex.DecodeString(address[2:])
// 		if err != nil {
// 			return nil, err
// 		}
// 		ethAddr := common.Address{}
// 		copy(ethAddr[:], addressHex)

// 		// Append record to address table. Note that StartRecord is just total
// 		// number of previous records (counting from 0)
// 		addressTable = append(addressTable, AddressRecord{
// 			Address: ethAddr,
// 			Offset:  uint32(appearanceCount),
// 			Count:   uint32(len(appearances)),
// 		})
// 		appearanceCount += len(appearances)

// 		// Write appearance data into the "placeholder" buffer
// 		binary.Write(&appearanceTableBuf, binary.LittleEndian, &appearances)
// 	}

// 	// This is our main buffer. We will write all chunk contents there
// 	buf := bytes.Buffer{}
// 	header := HeaderRecord{
// 		Magic:           file.MagicNumber,
// 		Hash:            common.Hash{},
// 		AddressCount:    uint32(len(addressToAppearances)),
// 		AppearanceCount: uint32(appearanceCount),
// 	}

// 	// Write header
// 	err := binary.Write(&buf, binary.LittleEndian, &header)
// 	if err != nil {
// 		return nil, err
// 	}

// 	// Write address table
// 	err = binary.Write(&buf, binary.LittleEndian, &addressTable)
// 	if err != nil {
// 		return nil, err
// 	}
// 	if buf.Len() != (44 + 84) {
// 		panic("oh no")
// 	}

// 	// Write appearance table
// 	_, err = appearanceTableBuf.WriteTo(&buf)
// 	if err != nil {
// 		return nil, err
// 	}

// 	return &buf, nil
// }
