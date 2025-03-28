package file

import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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

// ParseCommandsFile parses a text file into `CommandsFile` struct. While parsing, the function validates flags
// present on the current line.
func ParseCommandsFile(cmd *cobra.Command, filePath string) (cf CommandsFile, err error) {
	// TODO: parallelize
	inputFile, err := os.OpenFile(filePath, os.O_RDONLY, 0)
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
		trimmed := strings.TrimSpace(scanner.Text())
		lineNumber++

		// ignore comments and empty lines
		if len(trimmed) == 0 || trimmed[0] == '#' || trimmed[0] == ';' {
			continue
		}

		// --file inside file is forbidden
		if strings.Contains(trimmed, "--file") {
			err = reportErrWithLineNumber(errors.New("file uses --file flag recursively"), lineNumber)
			return
		}

		// remove unwanted whitespace including duplicate spaces, etc.
		trimmed = strings.Join(strings.Fields(trimmed), " ")

		// both cobra and pflags packages expect their parameters to be slices of strings
		tokens := strings.Split(trimmed, " ")
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

// RunWithFileSupport returns a function to run Cobra command. The command runs in the usual
// way unless `--file` is specified. If it is specified, this function will parse the file
// and then run the command in series of independent calls (just like calling `chifra`
// N times on the command line, but without wasting time and resources for the startup)
func RunWithFileSupport(
	mode string,
	run func(cmd *cobra.Command, args []string) error,
	resetOptions func(testMode bool),
) func(cmd *cobra.Command, args []string) error {

	return func(cmd *cobra.Command, args []string) error {
		// try to open the file
		filePath, err := cmd.Flags().GetString("file")
		if err != nil {
			return err
		}

		// TODO: see issue #2444 - probably better ways to do this
		forced := map[string]bool{
			"monitors": true,
		}
		if filePath == "" || forced[mode] {
			// `--file` has not been provided, run the command as usual
			return run(cmd, args)
		}

		// TODO: see issue #2444 - probably better ways to do this
		disallowed := map[string]bool{
			"init": true,
		}
		if disallowed[mode] {
			msg := fmt.Sprintf("The --file option is not allowed in %s mode.", mode)
			return errors.New(msg)
		}

		// parse commands file
		commandsFile, err := ParseCommandsFile(cmd, filePath)
		if err != nil {
			return err
		}

		testMode := base.IsTestMode()
		for _, line := range commandsFile.Lines {
			resetOptions(testMode)
			// first, parse flags from the command line
			_ = cmd.ParseFlags(os.Args[1:])
			// next, parse flags from the file
			err = cmd.ParseFlags(line.Flags)
			if err != nil {
				return err
			}
			// build arguments using both ones from command line and the file
			var callArgs []string
			callArgs = append(callArgs, args...)
			callArgs = append(callArgs, line.Args...)
			err = run(cmd, callArgs)
			if err != nil {
				return err
			}
		}
		return nil
	}
}
