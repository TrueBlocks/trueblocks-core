package base

type IpfsHash string

func (h IpfsHash) String() string {
	return string(h)
}
