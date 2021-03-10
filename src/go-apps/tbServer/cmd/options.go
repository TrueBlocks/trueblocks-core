package trueblocks

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

// optionsType makes command line options available to the package
 type optionsType struct {
	Verbose int
	Port string
}

// Options carries command line options
var Options optionsType
