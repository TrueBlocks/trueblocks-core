// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated with makeClass --gocmds. Edit only those parts of
 * the code outside of 'BEG_/END_' tags.
 */

package daemonPkg

// TODO: Once we're fully ported to Go, this can go away when CallOne and PassItOn goes away
// TODO: camelCase vs. snake_case - issue #1974
func convertToCommandLine(in string) string {
	// BEG_CONVERT_CODE
	switch in {
	case "accountFor":
		return "account_for"
	case "allowMissing":
		return "allow_missing"
	case "appsPerChunk":
		return "apps_per_chunk"
	case "bigRange":
		return "big_range"
	case "blockCnt":
		return "block_cnt"
	case "byAcct":
		return "by_acct"
	case "cacheTraces":
		return "cache_traces"
	case "channelCount":
		return "channel_count"
	case "firstBlock":
		return "first_block"
	case "firstRecord":
		return "first_record"
	case "firstSnap":
		return "first_snap"
	case "lastBlock":
		return "last_block"
	case "listCount":
		return "list_count"
	case "logLevel":
		return "log_level"
	case "matchCase":
		return "match_case"
	case "maxRecords":
		return "max_records"
	case "noHeader":
		return "no_header"
	case "noZero":
		return "no_zero"
	case "proxyFor":
		return "proxy_for"
	case "skipDdos":
		return "skip_ddos"
	case "snapToGrid":
		return "snap_to_grid"
	case "startBlock":
		return "start_block"
	case "toCustom":
		return "to_custom"
	case "toFile":
		return "to_file"
	case "unripeDist":
		return "unripe_dist"
	}
	// END_CONVERT_CODE
	return in
}
