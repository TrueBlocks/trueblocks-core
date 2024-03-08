package sdk

import (
	"fmt"
	"reflect"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// assignValuesFromArgs assumes the args array is a valid url (with & and = separating options and bools with no value)
func assignValuesFromArgs(target interface{}, globals *Globals, args []string) error {
	if len(args) == 0 || (len(args) == 1 && args[0] == "") {
		return nil
	}

	val := reflect.ValueOf(target).Elem()
	t := val.Type()

	for _, arg := range args {
		parts := strings.SplitN(arg, "=", 2)
		option := parts[0]
		var value string
		if len(parts) == 2 {
			value = parts[1]
		}

		found := false
		for i := 0; i < t.NumField(); i++ {
			field := val.Field(i)
			tag := t.Field(i).Tag.Get("arg")

			// Splitting the tag to support both single letter and full word options
			tags := strings.Split(tag, ",")
			for _, t := range tags {
				if t == option {
					found = true
					kind := field.Kind()
					switch kind {
					case reflect.Slice:
						field.Set(reflect.Append(field, reflect.ValueOf(value)))
					case reflect.Bool:
						field.SetBool(true)
					case reflect.String:
						field.SetString(value)
					case reflect.Uint64:
						if num, err := strconv.ParseUint(value, 10, 64); err != nil {
							return fmt.Errorf("error parsing uint64 for option %s: %v", option, err)
						} else {
							field.SetUint(num)
						}
					default:
						err := fmt.Errorf("unsupported field kind %s for option %s", kind, option)
						logger.Error(err)
					}
					break
				}
			}
			if found {
				break
			}
		}

		if !found && globals != nil {
			if err := assignValuesFromArgs(globals, nil, []string{arg}); err != nil {
				return err
			}
			found = true
		}

		if !found && option != "fmt" && option != "noHeader" {
			//lint:ignore ST1005 sorry
			return fmt.Errorf("Invalid key (%s) in route.", option)
		}
	}

	return nil
}
