package fourbytes

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func Test_write_one(t *testing.T) {
	buf := &bytes.Buffer{}
	w := bufio.NewWriter(buf)
	testSignature := base.HexToHash("04c786e6d139b5bcd86f44bbd1ca9466ac277f3e25f8bc62ba39d8b4e5c13d19")
	strSignature := "absolute()"
	strLen := len(strSignature)
	sigRecords := []SignatureRecord{
		{
			Signature: testSignature,
			Offset:    0,
			Len:       uint32(strLen),
		},
	}
	strRecords := []StringRecord{
		{
			Value: strSignature,
			Len:   strLen,
		},
	}
	if err := write(w, sigRecords, strRecords); err != nil {
		t.Fatal(err)
	}
	if err := w.Flush(); err != nil {
		t.Fatal(err)
	}

	var h Header
	binary.Read(buf, binary.LittleEndian, &h)
	if h.Magic != 0xdeadbeef {
		t.Fatalf("wrong magic '%x'", h.Magic)
	}
	if h.Hash != base.HexToHash("0x"+strings.Repeat("5", 32)) {
		t.Fatalf("wrong hash '%x'", h.Hash)
	}
	if h.SignatureCount != 1 {
		t.Fatalf("wrong magic %d", h.SignatureCount)
	}

	var sig SignatureRecord
	binary.Read(buf, binary.LittleEndian, &sig)
	if sig.Signature != testSignature {
		t.Fatalf("wrong signature '%x'", sig.Signature)
	}
	if sig.Offset != 0 {
		t.Fatal("wrong offset", sig.Offset)
	}
	if sig.Len != uint32(strLen) {
		t.Fatal("wrong len", sig.Len)
	}

	b := make([]byte, sig.Len)
	binary.Read(buf, binary.LittleEndian, &b)
	str := string(b)
	if str != strSignature {
		t.Fatalf("wrong string signature '%s'", str)
	}
}

func Test_write_many(t *testing.T) {
	buf := &bytes.Buffer{}
	w := bufio.NewWriter(buf)
	testSignatures := []base.Hash{
		base.HexToHash("04c786e6d139b5bcd86f44bbd1ca9466ac277f3e25f8bc62ba39d8b4e5c13d19"),
		base.HexToHash("3884d63599bd4ea4f3deb3e2108ff8ea7043b963874fb6f80e98f2a61d8b9139"),
	}
	strSignatures := []string{
		"absolute()",
		"airdrop()",
	}
	strLens := []int{
		len(strSignatures[0]),
		len(strSignatures[1]),
	}
	sigRecords := []SignatureRecord{
		{
			Signature: testSignatures[0],
			Offset:    0,
			Len:       uint32(strLens[0]),
		},
		{
			Signature: testSignatures[1],
			Offset:    uint32(strLens[0]),
			Len:       uint32(strLens[1]),
		},
	}
	strRecords := []StringRecord{
		{
			Value: strSignatures[0],
			Len:   strLens[0],
		},
		{
			Value: strSignatures[1],
			Len:   strLens[1],
		},
	}
	if err := write(w, sigRecords, strRecords); err != nil {
		t.Fatal(err)
	}
	if err := w.Flush(); err != nil {
		t.Fatal(err)
	}

	var h Header
	binary.Read(buf, binary.LittleEndian, &h)
	if h.Magic != 0xdeadbeef {
		t.Fatalf("wrong magic '%x'", h.Magic)
	}
	if h.Hash != base.HexToHash("0x"+strings.Repeat("5", 32)) {
		t.Fatalf("wrong hash '%x'", h.Hash)
	}
	if h.SignatureCount != 2 {
		t.Fatalf("wrong magic %d", h.SignatureCount)
	}

	sigs := make([]SignatureRecord, len(testSignatures))
	binary.Read(buf, binary.LittleEndian, &sigs)
	for i, sig := range sigRecords {
		if sig.Signature != sigs[i].Signature {
			t.Fatalf("wrong signature '%x'", sigs[i].Signature)
		}
		if sig.Offset != sigs[i].Offset {
			t.Fatal("wrong offset", sigs[i].Signature)
		}
		if sig.Len != sigs[i].Len {
			t.Fatal("wrong len", sigs[i].Len)
		}
	}

	for i, sig := range sigs {
		b := make([]byte, sig.Len)
		binary.Read(buf, binary.LittleEndian, &b)
		str := string(b)
		if str != strSignatures[i] {
			t.Fatalf("wrong string signature '%s'", str)
		}
	}
}

func Test_readAll(t *testing.T) {
	buf := &bytes.Buffer{}
	w := bufio.NewWriter(buf)
	testSignatures := []base.Hash{
		base.HexToHash("04c786e6d139b5bcd86f44bbd1ca9466ac277f3e25f8bc62ba39d8b4e5c13d19"),
		base.HexToHash("3884d63599bd4ea4f3deb3e2108ff8ea7043b963874fb6f80e98f2a61d8b9139"),
	}
	strSignatures := []string{
		"absolute()",
		"airdrop()",
	}
	strLens := []int{
		len(strSignatures[0]),
		len(strSignatures[1]),
	}
	sigRecords := []SignatureRecord{
		{
			Signature: testSignatures[0],
			Offset:    0,
			Len:       uint32(strLens[0]),
		},
		{
			Signature: testSignatures[1],
			Offset:    uint32(strLens[0]),
			Len:       uint32(strLens[1]),
		},
	}
	strRecords := []StringRecord{
		{
			Value: strSignatures[0],
			Len:   strLens[0],
		},
		{
			Value: strSignatures[1],
			Len:   strLens[1],
		},
	}
	if err := write(w, sigRecords, strRecords); err != nil {
		t.Fatal(err)
	}
	if err := w.Flush(); err != nil {
		t.Fatal(err)
	}

	readSigs, readStrs, err := readAll(buf)
	if err != nil {
		t.Fatal(err)
	}

	for i, r := range sigRecords {
		if r != readSigs[i] {
			t.Fatal("signature records not equal", readSigs)
		}
	}

	for i, r := range strRecords {
		if r.Value != readStrs[i].Value {
			t.Fatal("string records not equal", r, readStrs[i])
		}
	}
}
