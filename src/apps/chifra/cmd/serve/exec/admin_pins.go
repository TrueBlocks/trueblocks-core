package exec

import (
	"errors"
	"net/http"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/pins"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

var ErrMultipleModes = errors.New("only one of ?list or ?init is supported")
var ErrNoMode = errors.New("you must choose one of ?list or ?init")

// AdminPins runs pin-operating functions depending on query parameters
func AdminPins(request *http.Request) ([]manifest.PinDescriptor, error) {
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
			opts.Init_all = true
		}
	}

	err := pins.ValidateOptions(opts)
	if err != nil {
		// Usage() adds newlines, which are not needed in API mode
		formattedErr := strings.TrimLeft(err.Error(), "\n ")
		formattedErr = strings.TrimRight(formattedErr, "\n")
		return nil, errors.New(formattedErr)
	}

	// Make sure all required directories exist
	err = pinlib.EstablishDirectories()
	if err != nil {
		return nil, err
	}
	var responseBody []manifest.PinDescriptor
	var responseError error

	if opts.List {
		pins, err := pins.List()
		responseBody = pins
		responseError = err
	}

	if opts.Init {
		err := pins.Init(opts.Init_all)
		responseError = err
	}

	if responseError != nil {
		return nil, responseError
	}

	return responseBody, nil
}
