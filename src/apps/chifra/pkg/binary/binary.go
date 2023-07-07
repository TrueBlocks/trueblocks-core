package binary

import (
	"io"
)

type CacheUnmarshaler interface {
	UnmarshalCache(version uint64, reader io.Reader) error
}

type CacheMarshaler interface {
	MarshalCache(writer io.Writer) error
}
