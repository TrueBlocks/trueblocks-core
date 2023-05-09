package names

func PrepareRpc() error {
	_, err := LoadNamesMap("mainnet", Regular|Custom|Prefund, nil)
	return err
}
