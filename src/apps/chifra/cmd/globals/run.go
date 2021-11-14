package globals

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

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os"
	"os/exec"
	"os/user"
	"sync"
)

// dropNL2 drops new line characters (\n) from the progress stream
func dropNL2(data []byte) []byte {
	if len(data) > 0 && data[len(data)-1] == '\n' {
		return data[0 : len(data)-1]
	}
	return data
}

// ScanProgressLine2 looks for "lines" that end with `\r` not `\n` like usual
func ScanProgressLine2(data []byte, atEOF bool) (advance int, token []byte, err error) {
	if atEOF && len(data) == 0 {
		return 0, nil, nil
	}
	if i := bytes.IndexByte(data, '\n'); i >= 0 {
		fmt.Fprintf(os.Stderr, "%s\n", string(data[0:i]))
		return i + 1, data[0:i], nil
	}
	if i := bytes.IndexByte(data, '\r'); i >= 0 {
		fmt.Fprintf(os.Stderr, "%s\r", string(data[0:i]))
		return i + 1, dropNL2(data[0:i]), nil
	}
	return bufio.ScanLines(data, atEOF)
}

// ScanForProgress2 watches stderr and picks of progress messages
func ScanForProgress2(stderrPipe io.Reader, fn func(string)) {
	scanner := bufio.NewScanner(stderrPipe)
	scanner.Split(ScanProgressLine2)
	for scanner.Scan() {
		// we've already printed the token
	}
	if err := scanner.Err(); err != nil {
		fmt.Println("TB: Error while reading stderr -- ", err)
	}
}

// getCommandPath2 returns full path the the given tool
func getCommandPath2(cmd string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + cmd
}

func PassItOn(path string, opts *GlobalOptionsType, flags, arguments string) error {
	options := flags
	if opts.Raw {
		options += " --raw"
	}
	// if opts.Noop {
	// 	options += " --noop"
	// }
	if opts.Version {
		options += " --version"
	}
	if len(opts.Format) > 0 {
		options += " --fmt " + opts.Format
	}
	if opts.Verbose || opts.LogLevel > 0 {
		level := opts.LogLevel
		if level == 0 {
			level = 1
		}
		options += " --verbose " + fmt.Sprintf("%d", level)
	}
	if len(opts.OutputFn) > 0 {
		options += " --output " + opts.OutputFn
	}
	if opts.NoHeader {
		options += " --no_header"
	}
	if opts.Wei {
		options += " --wei"
	}
	if opts.Ether {
		options += " --ether"
	}
	if opts.Dollars {
		options += " --dollars"
	}
	if opts.ToFile {
		options += " --to_file"
	}
	if len(opts.File) > 0 {
		// TODO: one of the problems with this is that if the file contains invalid commands,
		// TODO: because we don't see those commands until we're doing into the tool, we
		// TODO: can't report on the 'bad command' in Cobra format. This will require us to
		// TODO: keep validation code down in the tools which we want to avoid. To fix this
		// TODO: the code below should open the file, read each command, and recursively call
		// TODO: into chifra here.
		options += " --file:" + opts.File
	}
	options += " " + arguments

	var wg sync.WaitGroup
	wg.Add(2)

	// fmt.Fprintf(os.Stderr, "Calling: %s %s\n", path, options)
	cmd := exec.Command(getCommandPath2(path), options)
	if os.Getenv("TEST_MODE") == "true" {
		cmd.Env = append(os.Environ(), "TEST_MODE=true")
	}

	stderrPipe, err := cmd.StderrPipe()
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s", err)
	} else {
		go func() {
			ScanForProgress2(stderrPipe, func(msg string) {
			})
			wg.Done()
		}()
	}

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s", err)
	} else {
		go func() {
			cmd.Start()
			scanner := bufio.NewScanner(stdout)
			buf := make([]byte, 1024*1024)
			scanner.Buffer(buf, 1024*1024)
			for scanner.Scan() {
				m := scanner.Text()
				fmt.Println(m)
			}
			wg.Done()
		}()
	}
	wg.Wait()
	cmd.Wait()
	return nil
}
