package sdk

import (
	"fmt"
	"net/url"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// ParseFunc is a function that takes a key/value pair and returns `true,nil` if the key/value pair
// was parsed and the value is valid. If the key is found, but the value is invalid, it returns
// `false, error` with a non-nil error. If it doesn't recognize the key, it returns `false, nil`.
type ParseFunc func(target any, key, value string) (bool, error)

// assignValuesFromArgs assumes the args array is a valid url (with & and = separating options and bools
// with no value). It parses the URL into the target.
func assignValuesFromArgs(args []string, parseFunc ParseFunc, target any, globals *Globals) error {
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
						field.SetInt(base.MustParseInt64(value))
					case reflect.String:
						field.SetString(value)
					case reflect.Uint64:
						field.SetUint(base.MustParseUint64(value))
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

func structToValues(data any) (url.Values, error) {
	values := make(url.Values)
	dataVal := reflect.ValueOf(data)

	if dataVal.Kind() != reflect.Struct {
		return nil, fmt.Errorf("expected a struct but got %s", dataVal.Kind())
	}

	err := mapStructFields(dataVal, values, "")
	if err != nil {
		return nil, err
	}

	return values, nil
}

func mapStructFields(dataVal reflect.Value, values url.Values, prefix string) error {
	dataValType := dataVal.Type()

	for i := 0; i < dataVal.NumField(); i++ {
		field := dataVal.Field(i)
		typeField := dataValType.Field(i)

		// Check if the field is an embedded struct
		if typeField.Anonymous && field.Kind() == reflect.Struct {
			// Recursively handle the embedded struct without changing the prefix
			err := mapStructFields(field, values, prefix)
			if err != nil {
				return err
			}
			continue
		}

		jsonTag := typeField.Tag.Get("json")
		if jsonTag == "-" || jsonTag == "" {
			continue // Skip untagged fields or explicitly ignored fields
		}

		key := strings.Split(jsonTag, ",")[0] // Use the first part of the json tag as the key

		if prefix != "" {
			key = prefix + "." + key
		}

		switch field.Kind() {
		case reflect.Bool:
			if field.Bool() {
				values.Set(key, "true")
			}
		case reflect.Uint, reflect.Uint64:
			if field.Uint() > 0 {
				values.Set(key, fmt.Sprintf("%d", field.Uint()))
			}
		case reflect.Float64:
			if field.Float() != 0.0 {
				values.Set(key, fmt.Sprintf("%f", field.Float()))
			}
		case reflect.String:
			if field.String() != "" {
				values.Set(key, field.String())
			}
		case reflect.Slice:
			for j := 0; j < field.Len(); j++ {
				elem := field.Index(j)
				if elem.Kind() == reflect.String {
					if field.String() != "" {
						values.Add(key, elem.String())
					}
				}
			}
		case reflect.Struct:
			// Special handling for the base.Address type
			if field.Type() == reflect.TypeOf(base.Address{}) {
				// Assuming base.Address has a method String() for conversion
				addr := field.Interface().(base.Address)
				if !addr.IsZero() {
					values.Set(key, addr.Hex())
				}
			} else {
				// Recursively map nested structs, assuming they do not introduce infinite recursion
				err := mapStructFields(field, values, key)
				if err != nil {
					return err
				}
			}
		default:
			if field.Type().Implements(reflect.TypeOf((*fmt.Stringer)(nil)).Elem()) {
				// Handle types that implement fmt.Stringer
				val := field.Interface().(fmt.Stringer).String()
				if len(val) > 0 && val != "none" {
					values.Set(key, val)
				}
			} else {
				return fmt.Errorf("unsupported field type: %s", field.Type())
			}
		}
	}

	return nil
}
