package pinning

func (p *PinningService) PinFile(filepath string) (string, error) {
	if p.Local {
		return p.pinFileLocally(filepath)
	}
	return p.pinFileRemotely(filepath)
}
