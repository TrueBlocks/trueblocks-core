package config

// IsChainConfigured returns true if the chain is configured in the config file.
func IsChainConfigured(needle string) bool {
	return GetRootConfig().Chains[needle] != chainGroup{}
}
