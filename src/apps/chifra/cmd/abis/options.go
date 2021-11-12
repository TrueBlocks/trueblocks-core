package abis

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type AbisOptionsType struct {
	Known   bool
	Sol     bool
	Find    []string
	Source  bool
	Classes bool
}

func ValidateOptions(opts *AbisOptionsType, args []string) error {
	if opts.Classes {
		return validate.Usage("the '{0}' option is not implemented", "--classes")
	}

	if len(opts.Find) == 0 && !opts.Known {
		err := validate.ValidateAtLeastOneAddr(args)
		if err != nil {
			return err
		}
	}

	if opts.Sol && len(opts.Find) > 0 {
		return validate.Usage("Please choose only one of --sol or --find.")
	}

	if opts.Sol {
		for _, sol := range args {
			if sol == "" {
				continue
			}
			cleaned := "./" + strings.Replace(sol, ".sol", "", 1) + ".sol"
			if !utils.FileExists(cleaned) {
				return validate.Usage("Solidity file not found at {0}", cleaned)
			}
		}
	} else {
		for _, term := range opts.Find {
			ok1, err1 := validate.IsValidFourByte(term)
			if !ok1 && len(term) < 10 {
				return err1
			}
			ok2, err2 := validate.IsValidTopic(term)
			if !ok2 && len(term) > 66 {
				return err2
			}
			if !ok1 && !ok2 {
				if len(term) > 43 {
					// more than halfway reports topic
					return err2
				}
				return err1
			} else {
				validate.Errors = nil
			}
		}
	}

	return nil
}
