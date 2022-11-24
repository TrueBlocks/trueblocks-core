package cache

type cString struct {
	size    uint64
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
