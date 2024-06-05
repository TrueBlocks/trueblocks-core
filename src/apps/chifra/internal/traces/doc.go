// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// tracesPkg implements the chifra traces command.
//
// The chifra traces tool retrieves a transaction's traces. You may specify multiple transaction
// identifiers per invocation.
//
// The --articulate option fetches the ABI from each encountered smart contract to better describe
// the reported data.
//
// The --filter option calls your node's trace_filter routine (if available) using a bang-separated
// string of the same values used by trace_fitler.
package tracesPkg
