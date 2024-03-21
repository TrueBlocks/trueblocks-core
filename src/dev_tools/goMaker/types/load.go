package types

import (
	"sort"
	"strings"
)

// LoadCommands loads the two csv files and returns the codebase which
// contains all the commands (each with its own options and endpoint).
// This will also eventually carry the data types.
func LoadCommands(thePath string) (CodeBase, error) {
	theMap := make(map[string]Command)

	options, err := LoadCsv[CmdLineOption, any](thePath+"cmd-line-options.csv", ReadCmdOption, nil)
	if err != nil {
		return CodeBase{}, err
	}

	for _, opt := range options {
		cmd := Command{
			Route:   opt.ApiRoute,
			Options: append(theMap[opt.ApiRoute].Options, opt),
		}
		theMap[opt.ApiRoute] = cmd
	}

	endpoints, err := LoadCsv[CmdLineEndpoint, any](thePath+"cmd-line-endpoints.csv", ReadCmdEndpoint, nil)
	if err != nil {
		return CodeBase{}, err
	}

	for _, endpoint := range endpoints {
		theMap[endpoint.ApiRoute] = Command{
			Route:       endpoint.ApiRoute,
			Group:       endpoint.Group,
			Description: endpoint.Description,
			Options:     theMap[endpoint.ApiRoute].Options,
			Endpoint:    endpoint,
		}
	}

	var cb CodeBase
	cb.Commands = make([]Command, 0, len(theMap))
	for _, cmd := range theMap {
		cmd.clean()
		cb.Commands = append(cb.Commands, cmd)
	}
	sort.Slice(cb.Commands, func(i, j int) bool {
		return cb.Commands[i].Route < cb.Commands[j].Route
	})

	return cb, nil
}

func ReadCmdEndpoint(cmd *CmdLineEndpoint, data *any) (bool, error) {
	cmd.Description = strings.ReplaceAll(cmd.Description, "&#44;", ",")
	return true, nil
}

func ReadCmdOption(op *CmdLineOption, data *any) (bool, error) {
	op.Description = strings.ReplaceAll(op.Description, "&#44;", ",")
	return true, nil
}
