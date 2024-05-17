package base

import (
	"strconv"
)

func (v *Value) UnmarshalJSON(data []byte) error {
	result, _ := strconv.ParseUint(string(data), 0, 64)
	*v = Value(result)
	return nil
}

func (t *Timestamp) UnmarshalJSON(data []byte) error {
	result, _ := strconv.ParseUint(string(data), 0, 64)
	*t = Timestamp(result)
	return nil
}

func (f *Float) UnmarshalJSON(data []byte) error {
	result, _ := strconv.ParseFloat(string(data), 64)
	*f = Float(result)
	return nil
}
