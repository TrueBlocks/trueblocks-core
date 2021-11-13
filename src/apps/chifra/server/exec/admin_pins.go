package exec

import (
	"net/http"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/pins"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

func ServePins(r *http.Request) ([]manifest.PinDescriptor, error) {
	query := r.URL.Query()
	opts := &pins.PinsOptionsType{}

	var err error
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
			opts.Sleep, err = strconv.ParseFloat(value[0], 64)
			if err != nil {
				return nil, err
			}
		case "freshen":
			opts.Freshen = true
		case "remote":
			opts.Remote = true
		case "init_all":
			opts.InitAll = true
		}
	}

	// Make sure all required directories exist
	err = pinlib.EstablishIndexFolders()
	if err != nil {
		return nil, err
	}

	err = opts.ValidateOptionsPins()
	if err != nil {
		return nil, err
	}

	var responseBody []manifest.PinDescriptor
	var responseError error

	if opts.List {
		responseBody, responseError = pins.ListInternal(opts)
	}

	if opts.Init {
		responseError = pins.InitInternal(opts)
	}

	if responseError != nil {
		return nil, responseError
	}

	return responseBody, nil
}
