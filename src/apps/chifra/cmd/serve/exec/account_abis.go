package exec

import (
	"errors"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/abis"
)

// AccountsAbis runs abi querying functions depending on query parameters
func AccountsAbis(request *http.Request) ([]abis.Function, error) {
	query := request.URL.Query()
	opts := &abis.AbisOptionsType{}
	var args []string

	for key, value := range query {
		switch key {
		case "addrs":
			args = value
		case "known":
			opts.Known = true
		case "sol":
			opts.Sol = true
		case "find":
			opts.Find = value
		case "source":
			opts.Source = true
		case "classes":
			opts.Classes = true
		}
	}

	err := abis.ValidateOptions(opts, args)
	if err != nil {
		// Usage() adds newlines, which are not needed in API mode
		formattedErr := strings.TrimLeft(err.Error(), "\n ")
		formattedErr = strings.TrimRight(formattedErr, "\n")
		return nil, errors.New(formattedErr)
	}

	results := abis.Find(opts.Find)
	return results, nil
}
