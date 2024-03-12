package sdk

import (
	"fmt"
	"reflect"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// ParseFunc is a function that takes a key/value pair and returns `true,nil` if the key/value pair
// was parsed and the value is valid. If the key is found, but the value is invalid, it returns
// `false, error` with a non-nil error. If it doesn't recognize the key, it returns `false, nil`.
type ParseFunc func(target interface{}, key, value string) (bool, error)

// assignValuesFromArgs assumes the args array is a valid url (with & and = separating options and bools
// with no value). It parses the URL into the target.
func assignValuesFromArgs(args []string, parseFunc ParseFunc, target interface{}, globals *Globals) error {
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
			tag := t.Field(i).Tag.Get("json")

			// Splitting the tag to support both single letter and full word options
			tags := strings.Split(tag, ",")
			for _, t := range tags {
				if t == option {
					found = true
					if parseFunc != nil {
						if ok, err := parseFunc(target, option, value); err != nil {
							return err
						} else if ok {
							continue
						}
					}
					kind := field.Kind()
					switch kind {
					case reflect.Slice:
						field.Set(reflect.Append(field, reflect.ValueOf(value)))
					case reflect.Bool:
						field.SetBool(true)
					case reflect.Int:
						field.SetInt(utils.MustParseInt(value))
					case reflect.String:
						field.SetString(value)
					case reflect.Uint64:
						if num, err := strconv.ParseUint(value, 10, 64); err != nil {
							return fmt.Errorf("error parsing uint64 for option %s: %v", option, err)
						} else {
							field.SetUint(num)
						}
					default:
						return fmt.Errorf("unsupported field kind %s for option %s", kind, option)
					}
					break
				}
			}
			if found {
				break
			}
		}

		if !found && globals != nil {
			if err := assignValuesFromArgs([]string{arg}, nil, globals, nil); err != nil {
				return err
			}
			found = true
		}

		if !found {
			//lint:ignore ST1005 sorry
			return fmt.Errorf("Invalid key (%s) in route.", option)
		}
	}

	return nil
}
