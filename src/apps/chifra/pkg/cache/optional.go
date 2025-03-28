package cache

import (
	"encoding/binary"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// Optional is used to read/write values that can be missing. Most of the case, it will be pointers
// to articulated objects. Optional.MarshalCache first write a boolean indicating if the value
// is present or not. If it is not present, nothing is written next. However, if it is present
// the value will be written using the standard path through WriteValue.
//
// When reading a missing value, we get a correct pointer zero value of nil instead of a pointer to
// initialized zero value from new(Type). new(Type) != nil.
type Optional[T any] struct {
	Value *T
	ptr   *T
}

func (o *Optional[T]) MarshalCache(writer io.Writer) (err error) {
	present := o.Value != nil
	if err = binary.Write(writer, binary.LittleEndian, present); err != nil {
		return
	}
	if !present {
		return nil
	}

	return base.WriteValue(writer, o.Value)
}

func (o *Optional[T]) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	var present bool
	if err = binary.Read(reader, binary.LittleEndian, &present); err != nil {
		return
	}
	if !present {
		return
	}

	var value T
	if err = base.ReadValue(reader, &value, fileVersion); err != nil {
		return err
	}
	o.ptr = &value
	return
}

// Get returns pointer to the value that was unmarshaled. We cannot simply set o.Value to point to
// the value read, because nil pointers cannot be dereferenced.
func (o *Optional[T]) Get() *T {
	return o.ptr
}
