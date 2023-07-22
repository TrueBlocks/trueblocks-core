// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// Package daemonPkg handles the chifra daemon command. It  manages chifra's long-running processes include its JSON API server. Each of the chifra commands along with all of its options, are provided not only by the command line, but also the API server. We call this process the flame server, which is written in Go. In the future, this daemon may also manage other long-running processes. Another way to get help to run chifra --help or chifra <cmd> --help on your command line. See below for an example of converting command line options to a call to the API. There's a one-to-one correspondence between the command line tools and options and the API routes and their options. 
package daemonPkg
