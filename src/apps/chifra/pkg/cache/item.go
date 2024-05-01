package cache

import (
	"errors"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// HeaderByteSize is the total size of a cache item's header in bytes.
const HeaderByteSize = 4 + 8

// Magic is the first bytes of a cache item's header. It is always set to 0xdeadbeef.
const Magic uint32 = 3735928559 // 0xdeadbeef

var ErrInvalidMagic = errors.New("invalid magic number")
var ErrIncompatibleVersion = errors.New("incompatible version")

type header struct {
	Magic   uint32
	Version uint64
}

var currentHeader *header

func init() {
	// Set currentHeader, so that we don't have to parse version
	// and build a header each time we want to encode/decode cache
	// item.
	ver := version.NewVersion(version.LibraryVersion)
	currentHeader = &header{
		Magic:   Magic,
		Version: ver.Uint64(),
	}
}

type Item struct {
	// We can set writer, so it's easier to test. Cache<Layout?> struct can
	// set it to a file automatically
	readWriter io.ReadWriter
	header     *header
}

func NewItem(rw io.ReadWriter) *Item {
	return &Item{
		readWriter: rw,
	}
}

func (i *Item) writeHeader() error {
	return i.marshal(currentHeader)
}

func (i *Item) readHeader() (h *header, err error) {
	h = new(header)
	i.header = h
	if err = i.unmarshal(h); err != nil {
		return
	}
	if h.Magic != Magic {
		// This should be silently ignored
		return nil, ErrInvalidMagic
	}

	return
}

func (i *Item) marshal(value any) (err error) {
	return WriteValue(i.readWriter, value)
}

func (i *Item) unmarshal(value any) (err error) {
	// i.header.Version will be 0 when reading header
	return ReadValue(i.readWriter, value, i.header.Version)
}

func (i *Item) Encode(value any) (err error) {
	if err = i.writeHeader(); err != nil {
		return
	}

	return i.marshal(value)
}

func (i *Item) Decode(value any) (err error) {
	if _, err = i.readHeader(); err != nil {
		return
	}
	return i.unmarshal(value)
}
