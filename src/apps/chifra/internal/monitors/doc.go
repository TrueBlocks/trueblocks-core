// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// monitorsPkg implements the chifra monitors command.
//
// chifra monitors has allows you to display information about the current set of monitors. This is
// not to be confused with the monitoring function available from the khedra tool. Please see that
// [Khedra Book for more information](https://khedra.trueblocks.io/).
//
// ### Crud commands
//
// chifra list creates a new monitor. See that tool's help file for more information.
//
// The chifra monitors --delete command deletes (or --undelete if already deleted) an address but does not remove it from your hard drive. The monitor is marked as being deleted, making it invisible to other tools.
//
// Use the --remove command to permanently remove a monitor from your computer. This is an
// irreversible operation and requires the monitor to have been previously deleted.
//
// The --decache option will remove not only the monitor but all of the cached data associated with
// the monitor (for example, transactions or traces). This is an irreversible operation (except
// for the fact that the cache can be easily re-created with chifra list <address>). The monitor need not have been previously deleted.
package monitorsPkg
