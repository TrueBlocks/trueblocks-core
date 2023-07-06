package cache

import (
	"errors"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/binary"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// Total header size in bytes
const HeaderByteSize = 8

type header struct {
	Version uint64
}

var currentHeader *header

func init() {
	// Set currentHeader, so that we don't have to parse version
	// and build a header each time we want to encode/decode cache
	// item.
	ver, err := version.NewVersion(version.LibraryVersion)
	if err != nil {
		panic(err)
	}
	currentHeader = &header{
		Version: ver.Uint64(),
	}
}

type Item struct {
	// We can set writer, so it's easier to test. Cache<Layout?> struct can
	// set it to a file automatically
	readWriter    io.ReadWriter
	header        *header
	headerWritten bool
}

func NewItem(rw io.ReadWriter) *Item {
	return &Item{
		readWriter: rw,
	}
}

func (i *Item) writeHeader() error {
	i.headerWritten = true
	return i.marshal(currentHeader)
}

func (i *Item) readHeader() (h *header, err error) {
	h = new(header)
	i.header = h
	i.unmarshal(h)
	return
}

func (i *Item) marshal(value any) (err error) {
	return binary.WriteValue(i.readWriter, value)
}

func (i *Item) unmarshal(value any) (err error) {
	// i.header.Version will be 0 when reading header
	return binary.ReadValue(i.readWriter, value, i.header.Version)
}

func (i *Item) Encode(value any) (err error) {
	if !i.headerWritten {
		if err = i.writeHeader(); err != nil {
			return
		}
	}

	return i.marshal(value)
}

func (i *Item) Decode(value any) (err error) {
	h, err := i.readHeader()
	if err != nil {
		return
	}
	if h.Version < currentHeader.Version {
		// TODO: implement migration (back levels)
		return errors.New("migration needed")
	}

	return i.unmarshal(value)
}
