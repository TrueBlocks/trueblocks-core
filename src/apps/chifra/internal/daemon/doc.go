// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// daemonPkg implements the chifra daemon command.
//
// chifra daemon manages chifra's API server. Each of the chifra commands along with all of its options,
// are provided not only by the command line, but also the API server. We call this process the
// flame server, which is written in Go. chifra serve is an alias for the chifra daemon command.
//
// In the future, this daemon may also manage other long-running processes such as chifra scrape
// and chifra monitors, but for now, it's only managing the API server.
//
// If the default port for the API server is in use, you may change it with the --url option.
//
// To get help for any command, please see the API documentation on our website. But, you may
// also run chifra --help or chifra <cmd> --help on your command line to get help.
//
// See below for an example of converting command line options to a call to the API. There's a
// one-to-one correspondence between the command line tools and options and the API routes and
// their options.
package daemonPkg
