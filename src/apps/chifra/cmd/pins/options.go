package pins

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

type PinsOptionsType struct {
	List     bool
	Init     bool
	All      bool
	Share    bool
	Sleep    float64
	Freshen  bool
	Remote   bool
	Init_all bool
}

// ValidateOptions validates options passed to pins.* functions
func ValidateOptions(opts *PinsOptionsType) error {
	if opts.List && opts.Init {
		return validate.Usage("Please choose only a single option.")
	}

	if !opts.List && !opts.Init {
		return validate.Usage("You must choose at least one of --list or --init.")
	}

	if opts.All && !opts.Init {
		return validate.Usage("Use the --all option only with the --init options.")
	}

	if opts.Init_all {
		return validate.Deprecated("--init_all", "--init --all")
	}

	if opts.Freshen {
		return validate.Deprecated("--freshen", "--init")
	}

	if opts.Remote {
		return validate.Deprecated("--remote", "")
	}

	if opts.Share {
		return validate.Usage("The --share option is not yet implemented")
	}

	return nil
}
