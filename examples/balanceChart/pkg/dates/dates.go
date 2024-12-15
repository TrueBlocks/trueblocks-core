package dates

import "github.com/bykof/gostradamus"

func NewDate(year, month, day, hour, min, sec int) gostradamus.DateTime {
	return gostradamus.NewDateTime(year, month, day, hour, min, sec, 0, "UTC")
}
