package config

import (
	"fmt"
	"os"
	"reflect"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/configtypes"
)

// loadFromEnv loads configuration from environment variables
func loadFromEnv(prefix string, destination *configtypes.Config) (err error) {
	// First we get all env variables then filter by prefix and finally parse the values
	envs := os.Environ()
	for i := 0; i < len(envs); i++ {
		// Turn VAR=value into []string{"VAR", "value"}
		parsed := strings.Split(envs[i], "=")
		if !strings.HasPrefix(parsed[0], prefix) {
			continue
		}

		// Turn PARENT_CHILD into []string{"PARENT", "CHILD"}
		path := strings.Split(parsed[0][len(prefix):], "_")
		if err := setByPath(destination, path, parsed[1]); err != nil {
			return err
		}
	}
	return nil
}

// setByPath matches struct field by a string path and sets its value
func setByPath(structure any, path []string, value string) error {
	v := reflect.ValueOf(structure)
	return deepSetByPath(&v, path, value)
}

// deepSetByPath follows path recursively until it reaches a single struct field.
// Then it parses the value and sets the field.
func deepSetByPath(structure *reflect.Value, path []string, value string) error {
	structValue := *structure
	// Follow pointers
	if structValue.Kind() == reflect.Pointer {
		structValue = structValue.Elem()
	}

	structType := structValue.Type()
	fieldCount := structType.NumField()
	makeParseError := func(err error) error {
		return fmt.Errorf("parsing %v value: %w", path, err)
	}
	for i := 0; i < fieldCount; i++ {
		field := structType.Field(i)
		if !strings.EqualFold(field.Name, path[0]) {
			continue
		}

		fieldValue := structValue.Field(i)
		if !fieldValue.CanSet() {
			return fmt.Errorf("cannot set %s", field.Name)
		}
		switch fieldValue.Kind() {
		case reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64:
			i, err := strconv.ParseInt(value, 0, 64)
			if err != nil {
				return makeParseError(err)
			}
			fieldValue.SetInt(i)
		case reflect.Uint, reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64:
			u, err := strconv.ParseUint(value, 0, 64)
			if err != nil {
				return makeParseError(err)
			}
			fieldValue.SetUint(u)
		case reflect.Bool:
			b, err := strconv.ParseBool(value)
			if err != nil {
				return makeParseError(err)
			}
			fieldValue.SetBool(b)
		case reflect.String:
			fieldValue.SetString(value)
		case reflect.Map:
			// When dealing with maps, we first have to obtain values for map key and map value
			mapKey := reflect.ValueOf(strings.ToLower(path[1]))
			mapValue := reflect.New(field.Type.Elem())

			// If map value is nil, it's simple: we will use mapValue that we've created above
			// and assign values to its fields (we assume that map items are always structs, because
			// nothing else makes sense for configuration).
			// However, if the map value exists, we have to clone the existing struct (the value) into
			// mapValue and reassign mapValue to mapKey (modifying map value will cause panic)
			if !fieldValue.IsNil() {
				existing := fieldValue.MapIndex(mapKey)
				if err := cloneStruct(&mapValue, &existing); err != nil {
					return err
				}
			}

			// Since all maps in Config have structs as values, we use recursion to set the value
			// of a correct field of the struct
			if err := deepSetByPath(&mapValue, path[2:], value); err != nil {
				return err
			}
			// SetMapIndex below needs a concrete type, not a pointer
			if mapValue.Kind() == reflect.Pointer {
				mapValue = mapValue.Elem()
			}
			// If the map is nil, we have to initialize it (just like `make(map[t1]t2)` does)
			if fieldValue.IsNil() {
				// MakeMap initializes a map. reflect.MapOf gives us the map type.
				newMap := reflect.MakeMap(reflect.MapOf(mapKey.Type(), mapValue.Type()))
				fieldValue.Set(newMap)
			}
			// We set the key to the correct value
			fieldValue.SetMapIndex(mapKey, mapValue)
		case reflect.Struct, reflect.Interface:
			if len(path) > 1 {
				return deepSetByPath(&fieldValue, path[1:], value)
			}
			return fmt.Errorf("expected value, but got struct for %v", path)
		default:
			return fmt.Errorf("unsupported type for %v", path)
		}
	}
	return nil
}

// cloneStruct sets fields of destination to the same values as found in source.
// The two structs have to be of the same type.
func cloneStruct(destination *reflect.Value, source *reflect.Value) error {
	v := *source
	// Follow pointers
	if v.Kind() == reflect.Pointer {
		v = v.Elem()
	}

	fieldCount := v.Type().NumField()
	for i := 0; i < fieldCount; i++ {
		fieldType := v.Type().Field(i)
		fieldValue := v.Field(i)
		if fieldValue.IsZero() {
			continue
		}

		if kind := destination.Kind(); kind != reflect.Pointer && kind != reflect.Interface {
			return fmt.Errorf("expected pointer to %s", fieldType.Name)
		}
		destValue := destination.Elem()
		if destValue.Kind() != reflect.Struct {
			return fmt.Errorf("expected %s to be a struct", fieldType.Name)
		}
		destField := destValue.FieldByName(fieldType.Name)
		if !destField.CanSet() {
			return fmt.Errorf("cannot set %s", fieldType.Name)
		}

		destField.Set(fieldValue)
	}
	return nil
}
