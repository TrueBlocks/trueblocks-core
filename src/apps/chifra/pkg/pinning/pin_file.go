package pinning

func (p *Service) PinFile(filepath string) (string, error) {
	if p.Local {
		return p.pinFileLocally(filepath)
	}
	return p.pinFileRemotely(filepath)
}
