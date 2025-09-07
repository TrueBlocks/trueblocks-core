package base

import (
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/bykof/gostradamus"
)

type DateTime struct {
	time.Time
}

func NewDateTime(fields ...int) DateTime {
	if len(fields) > 6 {
		logger.ShouldNotHappen("NewDateTime accepts at most 6 arguments")
	}

	now := time.Now()
	year, month, day := now.Year(), now.Month(), now.Day()
	hour, minute, second := now.Hour(), now.Minute(), now.Second()

	switch len(fields) {
	case 6:
		second = fields[5]
		fallthrough
	case 5:
		minute = fields[4]
		fallthrough
	case 4:
		hour = fields[3]
		fallthrough
	case 3:
		day = fields[2]
		fallthrough
	case 2:
		month = time.Month(fields[1])
		fallthrough
	case 1:
		year = fields[0]
	}

	return DateTime{time.Date(year, month, day, hour, minute, second, 0, time.UTC)}
}

func NewDateTimeTs(ts Timestamp) DateTime {
	return DateTime{time.Unix(int64(ts), 0).UTC()}
}

func (dt *DateTime) Before(test *DateTime) bool {
	return dt.String() < test.String()
}

func (dt *DateTime) After(test *DateTime) bool {
	return dt.String() > test.String()
}

func (dt *DateTime) String() string {
	ret := dt.Time.Format("2006-01-02T15:04:05")
	ret = strings.Replace(ret, "T", " ", -1)
	ret = strings.Replace(ret, "+0000", "", -1)
	return ret + " UTC"
}

func (dt *DateTime) MarshalCSV() (string, error) {
	return dt.String(), nil
}

func (dt *DateTime) UnmarshalCSV(csv string) (err error) {
	csv = strings.Replace(csv, " UTC", "", -1)
	csv = strings.Replace(csv, " ", "T", -1)
	fmt := strings.Replace(time.RFC3339, "Z07:00", "", -1)
	dt.Time, err = time.Parse(fmt, csv)
	return err
}

func (dt *DateTime) EndOfMonth() DateTime {
	g := gostradamus.DateTimeFromTime(dt.Time).CeilMonth()
	t, _ := time.Parse("2006-01-02T15:04:05", g.Format("2006-01-02T15:04:05"))
	return DateTime{t}
}

func (dt *DateTime) EndOfYear() DateTime {
	g := gostradamus.DateTimeFromTime(dt.Time).CeilYear()
	t, _ := time.Parse("2006-01-02T15:04:05", g.Format("2006-01-02T15:04:05"))
	return DateTime{t}
}

func IsValidPeriod(period string) bool {
	switch period {
	case "current":
		fallthrough
	case "annually":
		fallthrough
	case "quarterly":
		fallthrough
	case "monthly":
		fallthrough
	case "weekly":
		fallthrough
	case "daily":
		fallthrough
	case "hourly":
		fallthrough
	case "minutely":
		fallthrough
	case "secondly":
		fallthrough
	case "dayofweek":
		fallthrough
	case "blockly":
		return true
	default:
		return false
	}
}

func GetDateKey(period string, date DateTime) string {
	switch period {
	case "current":
		return ""
	case "annually":
		return fmt.Sprintf("%04d", date.Year())
	case "quarterly":
		quarter := (date.Month()-1)/3 + 1
		return fmt.Sprintf("%04d Q%d", date.Year(), quarter)
	case "monthly":
		return fmt.Sprintf("%04d-%02d", date.Year(), date.Month())
	case "weekly":
		y, w := date.ISOWeek()
		return fmt.Sprintf("%04d-%02d", y, w)
	case "daily":
		return fmt.Sprintf("%04d-%02d-%02d", date.Year(), date.Month(), date.Day())
	case "hourly":
		return fmt.Sprintf("%04d-%02d-%02d %02d", date.Year(), date.Month(), date.Day(), date.Hour())
	case "minutely":
		return fmt.Sprintf("%04d-%02d-%02d %02d:%02d", date.Year(), date.Month(), date.Day(), date.Hour(), date.Minute())
	case "secondly":
		return fmt.Sprintf("%04d-%02d-%02d %02d:%02d:%02d", date.Year(), date.Month(), date.Day(), date.Hour(), date.Minute(), date.Second())
	case "dayofweek":
		dayNames := []string{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}
		return dayNames[date.Weekday()]
	default:
		return fmt.Sprintf("%04d-%02d-%02d", date.Year(), date.Month(), date.Day())
	}
	// return ""
}
