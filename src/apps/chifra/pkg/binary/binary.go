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

func reverseBytes(original []byte) (reversed []byte) {
	length := len(original)
	reversed = make([]byte, length)

	for index, value := range original {
		reversed[length-1-index] = value
	}

	return
}
