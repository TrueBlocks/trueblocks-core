package sdk

import (
	"flag"
	"fmt"
	"reflect"
	"strings"
)

// ParseOptions parses the command line arguments into the ListOptions struct
func ParseOptions[T any](args []string, opts *T) ([]string, error) {
	fs := flag.NewFlagSet("options", flag.ContinueOnError)

	optType := reflect.TypeOf(*opts)
	optValue := reflect.ValueOf(opts).Elem()

	for i := 0; i < optType.NumField()-2; i++ { // Skip the last field (Addrs)
		field := optType.Field(i)
		fieldValue := optValue.Field(i)
		fmt.Println("field.Name:", field.Name, "field.Type:", field.Type)
		fmt.Println()

		flagName, flagShort := parseFlagTag(field.Tag.Get("flag"))

		// Register flags based on the field type
		switch fieldValue.Kind() {
		case reflect.String:
			fs.StringVar(fieldValue.Addr().Interface().(*string), flagName, "", "")
			if flagShort != "" {
				fs.StringVar(fieldValue.Addr().Interface().(*string), flagShort, "", "")
			}
		case reflect.Uint:
			fs.UintVar(fieldValue.Addr().Interface().(*uint), flagName, 0, "")
			if flagShort != "" {
				fs.UintVar(fieldValue.Addr().Interface().(*uint), flagShort, 0, "")
			}
		case reflect.Bool:
			fs.BoolVar(fieldValue.Addr().Interface().(*bool), flagName, false, "")
			if flagShort != "" {
				fs.BoolVar(fieldValue.Addr().Interface().(*bool), flagShort, false, "")
			}
		// case reflect.Slice:
		// 	// switch fieldValue.Type().Elem().Kind() {
		// 	// case reflect.String:
		// 	// 	fs.StringSliceVar(fieldValue.Addr().Interface().(*[]string), flagName, []string{}, "")
		// 	// 	if flagShort != "" {
		// 	// 		fs.StringSliceVar(fieldValue.Addr().Interface().(*[]string), flagShort, []string{}, "")
		// 	// 	}
		// 	// case reflect.Uint:
		// 	// 	fs.UintSliceVar(fieldValue.Addr().Interface().(*[]uint), flagName, []uint{}, "")
		// 	// 	if flagShort != "" {
		// 	// 		fs.UintSliceVar(fieldValue.Addr().Interface().(*[]uint), flagShort, []uint{}, "")
		// 	// 	}
		// 	// }
		default:
			return []string{}, fmt.Errorf("unsupported flag type: %s", fieldValue.Type())
		}
	}

	for i, arg := range args {
		fmt.Println(i, arg)
	}
	fmt.Println("opts:", opts)
	err := fs.Parse(args)
	if err != nil {
		return fs.Args(), err
	}

	fmt.Println("opts:", opts)
	fmt.Println("fs.Args:", fs.Args())
	return fs.Args(), nil
}

// parseFlagTag splits the tag into the flag name and its short version, if present.
func parseFlagTag(tag string) (string, string) {
	parts := strings.Split(tag, ",")
	if len(parts) > 1 {
		return parts[0], parts[1]
	}
	return parts[0], ""
}
