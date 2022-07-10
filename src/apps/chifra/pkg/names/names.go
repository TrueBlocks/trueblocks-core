package names

type Name struct {
	Tags        string `json:"tags"`
	Address     string `json:"address"`
	Name        string `json:"name"`
	Symbol      string `json:"symbol"`
	Source      string `json:"source"`
	Decimals    string `json:"decimals"`
	Description string `json:"description"`
	Deleted     bool   `json:"deleted"`
	IsCustom    bool   `json:"isCustom"`
	IsPrefund   bool   `json:"isPrefund"`
	IsContract  bool   `json:"isContract"`
	IsErc20     bool   `json:"isErc20"`
	IsErc721    bool   `json:"isErc721"`
}
