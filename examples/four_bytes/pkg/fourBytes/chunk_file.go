package fourbytes

import (
	"encoding/binary"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

type Header struct {
	Magic          uint32
	Hash           base.Hash
	SignatureCount uint32
}

type SignatureRecord struct {
	Signature base.Hash
	// Where the string representation starts in the file?
	Offset uint32
	// How long is the string representation?
	Len uint32
}

type StringRecord struct {
	Value string
	Len   int
}

// write writes chunk file with given signatures and strings
func write(out io.Writer, sigRecords []SignatureRecord, strRecords []StringRecord) (err error) {
	h := Header{
		Magic:          file.MagicNumber,
		Hash:           base.HexToHash("0x" + strings.Repeat("5", 32)),
		SignatureCount: uint32(len(sigRecords)),
	}

	if err = binary.Write(out, binary.LittleEndian, h); err != nil {
		return
	}
	fixedLengthSignatures := make([]SignatureRecord, len(sigRecords))
	copy(fixedLengthSignatures, sigRecords)

	if err = binary.Write(out, binary.LittleEndian, fixedLengthSignatures); err != nil {
		return
	}
	for _, strRecord := range strRecords {
		asBytes := []byte(strRecord.Value)
		if err = binary.Write(out, binary.LittleEndian, asBytes); err != nil {
			return
		}
	}

	return
}

// readAll reads whole chunk
func readAll(in io.Reader) (sigRecords []SignatureRecord, strRecords []StringRecord, err error) {
	h := Header{}
	if err = binary.Read(in, binary.LittleEndian, &h); err != nil {
		return
	}
	if h.SignatureCount == 0 {
		return
	}
	sigRecords = make([]SignatureRecord, h.SignatureCount)
	strRecords = make([]StringRecord, h.SignatureCount)
	if err = binary.Read(in, binary.LittleEndian, sigRecords); err != nil {
		return
	}

	for i := 0; i < int(h.SignatureCount); i++ {
		b := make([]byte, sigRecords[i].Len)
		if err = binary.Read(in, binary.LittleEndian, &b); err != nil {
			return
		}
		strRecords[i] = StringRecord{Value: string(b), Len: int(sigRecords[i].Len)}
	}
	return
}
