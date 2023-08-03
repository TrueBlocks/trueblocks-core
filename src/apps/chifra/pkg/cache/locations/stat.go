package locations

type ItemInfo struct {
	fileSize int
	name     string
}

func (s *ItemInfo) Size() int {
	return s.fileSize
}

func (s *ItemInfo) Name() string {
	return s.name
}
