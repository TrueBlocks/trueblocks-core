// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// Package daemonPkg handles the chifra daemon command. It  manages chifra's API server. Each of the chifra commands along with all of its options, are provided not only by the command line, but also the API server. We call this process the flame server, which is written in Go. chifra serve is an alias for the  command. In the future, this daemon may also manage other long-running processes such as chifra scrape and chifra monitors, but for now, it's only managing the API server. The --grpc option turns on a GRPC server that may speed up certain command such as chifra names, although this option is experimental and therefore not recommended for production use. If the default port for the API server is in use, you may change it with the --port option. To get help for any command, please see the API documentation on our website. But, you may also run chifra --help or chifra <cmd> --help on your command line to get help. See below for an example of converting command line options to a call to the API. There's a one-to-one correspondence between the command line tools and options and the API routes and their options. 
package daemonPkg

