package types

import (
	"fmt"
	"reflect"
	"sort"
	"strings"
)

const defaultMaxDeep = 10
const tooDeepMessage = "structure too deep"

// shouldOmitKey returns true if the field is a special case and we want
// to completely ignore it.
func shouldOmitKey(key string, reflectValue *reflect.Value) bool {
	reflectType := reflectValue.Type().Kind()

	if key == "stateMutability" {
		var str string
		if reflectType == reflect.String {
			str = reflectValue.String()
		}
		if reflectType == reflect.Interface || reflectType == reflect.Pointer {
			str = reflectValue.Elem().String()
		}
		return str == "nonpayable" || str == "view"
	}
	if key == "unused" {
		if reflectType == reflect.Interface || reflectType == reflect.Pointer {
			return !(reflectValue.Elem().Bool())
		}
		if reflectType == reflect.Bool {
			return !(reflectValue.Bool())
		}
		return false
	}
	if key == "components" {
		if reflectType == reflect.Interface || reflectType == reflect.Pointer {
			return reflectValue.Elem().Len() == 0
		}
		if reflectType == reflect.Slice {
			return reflectValue.Len() == 0
		}
		return false
	}
	return false
}

// serialize builds the compressed string, iterating the given input recursively until
// `maxDeep` goes to zero. If `maxDeep` is set to -1, `serialize` will use `defaultMaxDeep`
func serialize(input any, maxDeep int) string {
	if input == nil {
		return ""
	}
	// maxDeep will stop recursive calls to serialize if the code goes too deep.
	realMaxDeep := maxDeep
	if maxDeep <= -1 {
		realMaxDeep = defaultMaxDeep
	}
	if realMaxDeep == 0 {
		return tooDeepMessage
	}
	// We will pass nextMaxDeep to the next recursive call
	nextMaxDeep := realMaxDeep - 1
	inputValue := reflect.ValueOf(input)
	inputType := inputValue.Type()

	switch inputType.Kind() {
	case reflect.Slice, reflect.Array:
		size := inputValue.Len()
		items := make([]string, 0, size)
		for i := 0; i < size; i++ {
			itemValue := serialize(inputValue.Index(i).Interface(), nextMaxDeep)
			items = append(items, itemValue)
		}
		return "[" + strings.Join(items, "|") + "]"
	case reflect.Map:
		keys := inputValue.MapKeys()
		items := make([]string, 0, len(keys))
		var name string
		for i := 0; i < len(keys); i++ {
			keyString := keys[i].String()
			itemValue := inputValue.MapIndex(keys[i])
			if shouldOmitKey(keyString, &itemValue) {
				continue
			}
			if keyString == "name" {
				name = fmt.Sprint(itemValue.Interface())
				continue
			}
			itemValueString := serialize(itemValue.Interface(), nextMaxDeep)
			items = append(items, keyString+":"+itemValueString)
		}
		var content string
		sort.Strings(items)
		if name != "" {
			content = "name:" + name + "|" + strings.Join(items, "|")
		} else {
			content = strings.Join(items, "|")
		}
		return "{" + content + "}"
	case reflect.Struct:
		size := inputValue.NumField()
		items := make([]string, 0, size)
		var name string
		for i := 0; i < size; i++ {
			key := inputType.Field(i)
			if !key.IsExported() {
				continue
			}
			itemValue := inputValue.Field(i)
			var keyString string
			jsonTag, ok := inputType.Field(i).Tag.Lookup("json")
			if !ok {
				keyString = key.Name
			} else {
				parts := strings.Split(jsonTag, ",")
				keyString = parts[0]
				if keyString == "-" {
					continue
				}
				if len(parts) > 1 && parts[1] == "omitempty" && itemValue.IsZero() {
					continue
				}
			}
			if shouldOmitKey(keyString, &itemValue) {
				continue
			}
			if keyString == "name" {
				name = fmt.Sprint(itemValue.Interface())
				continue
			}
			itemValueString := serialize(itemValue.Interface(), nextMaxDeep)
			items = append(items, keyString+":"+itemValueString)
		}
		var content string
		sort.Strings(items)
		if name != "" {
			content = "name:" + name + "|" + strings.Join(items, "|")
		} else {
			content = strings.Join(items, "|")
		}
		return "{" + content + "}"
	case reflect.Pointer:
		return serialize(inputValue.Elem().Interface(), nextMaxDeep)
	}

	return fmt.Sprint(input)
}

// MakeCompressed produces compressedTx|Trace|Log field value
func MakeCompressed(input any) (result string) {
	return serialize(input, 10)
}
