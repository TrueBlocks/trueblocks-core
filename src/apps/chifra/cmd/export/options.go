package export

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

type ExportOptionsType struct {
	appearances  bool
	receipts     bool
	statements   bool
	logs         bool
	traces       bool
	accounting   bool
	articulate   bool
	cache        bool
	cache_traces bool
	factory      bool
	count        bool
	first_record uint64
	max_records  uint64
	relevant     bool
	emitter      []string
	topic        []string
	clean        bool
	freshen      bool
	staging      bool
	unripe       bool
	load         string
	reversed     bool
	by_date      bool
	summarize_by string
	skip_ddos    bool
	max_traces   uint64
	first_block  uint64
	last_block   uint64
}

var Options ExportOptionsType
