package trueblocks

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

// optionsType makes command line options available to the package
 type optionsType struct {
	Port string
	Verbose int
}

// Options carries command line options
var Options optionsType
