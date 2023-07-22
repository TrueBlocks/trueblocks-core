// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// Package whenPkg handles the chifra when command. It The  tool answers one of two questions: (1) "At what date and time did a given block occur?" or (2) "What block occurred at or before a given date and time?" In the first case, supply a block number or hash and the date and time of that block are displayed. In the later case, supply a date (and optionally a time) and the block number that occurred at or just prior to that date is displayed. The values for date and time are specified in JSON format. hour/minute/second are optional, and if omitted, default to zero in each case. Block numbers may be specified as either integers or hexadecimal number or block hashes. You may specify any number of dates and/or blocks per invocation. 
package whenPkg
