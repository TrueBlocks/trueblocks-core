package utils

import "os"

func IsPermitted() bool {
	isVersion := false
	isStatus := false
	isConfig := false
	hasPaths := false
	hasEdit := false
	cnt := len(os.Args)
	for _, arg := range os.Args {
		if arg == "help" {
			return false
		} else if arg == "status" {
			isStatus = true
		} else if arg == "config" {
			isConfig = true
		} else if arg == "--paths" {
			hasPaths = true
		} else if arg == "--version" || arg == "version" {
			isVersion = true
		} else if arg == "edit" {
			hasEdit = true
		} else if arg != "--verbose" {
			isStatus = false
			isConfig = false
		} else {
			cnt-- // allow --verbose
		}
	}

	if isVersion {
		return true
	}

	if isStatus && cnt == 2 {
		return true
	}

	return isConfig && (hasPaths || hasEdit) && cnt < 4
}
