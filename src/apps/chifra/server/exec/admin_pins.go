package exec

import (
	"errors"
	"net/http"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/pins"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

var ErrMultipleModes = errors.New("only one of ?list or ?init is supported")
var ErrNoMode = errors.New("you must choose one of ?list or ?init")

// AdminPins runs pin-operating functions depending on query parameters
func AdminPins(request *http.Request) (*manifest.PinsList, error) {
	query := request.URL.Query()
	opts := &pins.PinsOptionsType{}

	for key, value := range query {
		switch key {
		case "init":
			opts.Init = true
		case "list":
			opts.List = true
		case "all":
			opts.All = true
		case "share":
			opts.Share = true
		case "sleep":
			num, err := strconv.ParseFloat(value[0], 64)
			if err != nil {
				return nil, err
			}
			opts.Sleep = num
		case "freshen":
			opts.Freshen = true
		case "remote":
			opts.Remote = true
		case "init_all":
			opts.InitAll = true
		}
	}

	err := pins.ValidateOptions(opts)
	if err != nil {
		return nil, err
	}

	// Make sure all required directories exist
	err = pinlib.EstablishDirectories()
	if err != nil {
		return nil, err
	}
	var responseBody *manifest.PinsList
	var responseError error

	if opts.List {
		pins, err := pins.List()
		responseBody = &pins
		responseError = err
	}

	if opts.Init {
		err := pins.Init(opts.All)
		responseError = err
	}

	if responseError != nil {
		return nil, responseError
	}

	return responseBody, nil
}
