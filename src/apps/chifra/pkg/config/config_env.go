package config

import "strings"

type ConfigEnv struct {
	Chain           string `json:"chain"`
	ConfigPath      string `json:"configPath"`
	ChainConfigPath string `json:"chainConfigPath"`
	CachePath       string `json:"cachePath"`
	IndexPath       string `json:"indexPath"`
	DefaultChain    string `json:"defChain"`
	RpcProvider     string `json:"rpcProvider"`
}

func (c *ConfigEnv) ToCSV() string {
	var ret []string
	ret = append(ret, c.Chain)
	ret = append(ret, c.ConfigPath)
	ret = append(ret, c.ChainConfigPath)
	ret = append(ret, c.CachePath)
	ret = append(ret, c.IndexPath)
	ret = append(ret, c.DefaultChain)
	ret = append(ret, c.RpcProvider)
	return strings.Join(ret, ",")
}
