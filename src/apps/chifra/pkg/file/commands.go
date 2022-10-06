package file

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"github.com/spf13/cobra"
)

// CommandsFile is data structure representing the text file with commands
// (flags and arguments) on each line. Lines trigger independent invocations
// of given chifra subcommand.
type CommandsFile struct {
	Lines []CommandFileLine
}

// CommandFileLine stores line number for future reference, flag (usually a string
// that starts with "-" or "--") and args (any other string) information
type CommandFileLine struct {
	LineNumber uint
	Flags      []string
	Args       []string
}

// ParseCommandFile parses a text file into `CommandsFile` struct. While parsing, the function validates flags
// present on the current line.
func ParseCommandsFile(cmd *cobra.Command, filePath string) (cf CommandsFile, err error) {
	// TODO: parallelize
	inputFile, err := os.Open(filePath)
	if err != nil {
		return
	}

	// scanner to get file lines
	scanner := bufio.NewScanner(inputFile)
	// we will use a simple counter to keep track of line numbers for improved error reporting
	lineNumber := uint(0)
	// this helper function reduce repeated code
	reportErrWithLineNumber := func(err error, lineNumber uint) error {
		return fmt.Errorf("on line %d: %s", lineNumber, err)
	}
	for scanner.Scan() {
		rawLine := scanner.Text()
		lineNumber++

		// both cobra and pflags packages expect their parameters to be slices of strings
		tokens := strings.Split(rawLine, " ")
		// validate flags (we assume that `Flags()` returns `FlagSet` with both local and
		// global flags defined)
		err = cmd.ParseFlags(tokens)
		if err != nil {
			err = reportErrWithLineNumber(err, lineNumber)
			return
		}
		// line has been parsed successfully
		cf.Lines = append(cf.Lines, CommandFileLine{
			LineNumber: uint(lineNumber),
			Flags:      tokens,
			Args:       cmd.Flags().Args(),
		})
	}
	if err = scanner.Err(); err != nil {
		err = reportErrWithLineNumber(err, lineNumber)
		return
	}

	return
}
