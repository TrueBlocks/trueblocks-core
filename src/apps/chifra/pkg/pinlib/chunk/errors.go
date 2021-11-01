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
package chunk

import "fmt"

// Errors that can happen when validating and saving a file

type ErrSavingCorruptedDownload struct {
	fileName string
	expected int64
	read     int64
}

func (err *ErrSavingCorruptedDownload) Error() string {
	return fmt.Sprintf("file corrupted: %s (expected %d but got %d bytes)", err.fileName, err.expected, err.read)
}

type ErrSavingCannotDecompress struct {
	fileName string
	reason   error
}

func (err *ErrSavingCannotDecompress) Error() string {
	return fmt.Sprintf("error unpacking %s: %s", err.fileName, err.reason)
}

type ErrSavingCreateFile struct {
	fileName string
	reason   error
}

func (err *ErrSavingCreateFile) Error() string {
	return fmt.Sprintf("error creating file %s: %s", err.fileName, err.reason)
}

type ErrSavingCopy struct {
	fileName string
	reason   error
}

func (err *ErrSavingCopy) Error() string {
	return fmt.Sprintf("error copying %s: %s", err.fileName, err.reason)
}
