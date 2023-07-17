package locations

type ItemInfo struct {
	fileSize int
}

func (s *ItemInfo) Size() int {
	return s.fileSize
}
