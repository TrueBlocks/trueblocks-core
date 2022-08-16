package pinning

func (p *Service) PinFile(filepath string, local bool) (string, error) {
	if local {
		return p.pinFileLocally(filepath)
	}
	return p.pinFileRemotely(filepath)
}
