package cache

type unsignedLong = uint64
type cString struct {
	size    unsignedLong
	content []byte
}

type cacheHeader struct {
	deleted   uint64
	schema    uint64
	showing   uint64
	className cString
}

func reverseBytes(original []byte) (reversed []byte) {
	length := len(original)
	reversed = make([]byte, length)

	for index, value := range original {
		reversed[length-1-index] = value
	}

	return
}
