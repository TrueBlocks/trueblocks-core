package blockRange

import (
	"testing"
)

func TestParseBlockNums(t *testing.T) {
	out, err := Parse("10-1000:25")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Block != 1000 {
		t.Error("Mismatched end block:", out.Points[1].Block)
	}

	if out.Modifier.Step != 25 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseBlockNumsLeading0(t *testing.T) {
	out, err := Parse("000000-020:10")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 0 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Block != 20 {
		t.Error("Mismatched end block:", out.Points[1].Block)
	}
}

func TestParseBlockNumsNoEnd(t *testing.T) {
	out, err := Parse("10:25")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if len(out.Points) > 1 {
		t.Error("End present in parsed output")
	}

	if out.Modifier.Step != 25 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseBlockNumsNoStep(t *testing.T) {
	out, err := Parse("10-1000")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Block != 1000 {
		t.Error("Mismatched end block:", out.Points[1].Block)
	}

	if out.Modifier != nil {
		t.Error("Modifier present in parsed output")
	}
}

func TestParseBlockNumsNoEndNoStep(t *testing.T) {
	out, err := Parse("10")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if len(out.Points) > 1 {
		t.Error("End present in parsed output")
	}

	if out.Modifier != nil {
		t.Error("Modifier present in parsed output")
	}
}

func TestParseBlockNumsPeriod(t *testing.T) {
	out, err := Parse("10-1000:weekly")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Block != 1000 {
		t.Error("Mismatched end block:", out.Points[1].Block)
	}

	if out.Modifier.Period != "weekly" {
		t.Error("Mismatched period:", out.Modifier.Period)
	}
}

func TestParseBlockNumsPeriodNoEnd(t *testing.T) {
	out, err := Parse("10:weekly")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if len(out.Points) > 1 {
		t.Error("End present in parsed output")
	}

	if out.Modifier.Period != "weekly" {
		t.Error("Mismatched period:", out.Modifier.Period)
	}
}

func TestParseBlockNumsHexStart(t *testing.T) {
	out, err := Parse("0xfff-1000:25")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 4095 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Block != 1000 {
		t.Error("Mismatched end block:", out.Points[1].Block)
	}

	if out.Modifier.Step != 25 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseSpecialStart(t *testing.T) {
	out, err := Parse("london-1000:10")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Special != "london" {
		t.Error("Mismatched start special:", out.Points[0].Special)
	}

	if out.Points[1].Block != 1000 {
		t.Error("Mismatched end block:", out.Points[1].Block)
	}

	if out.Modifier.Step != 10 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseSpecialEnd(t *testing.T) {
	out, err := Parse("0-devcon1:10")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 0 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Special != "devcon1" {
		t.Error("Mismatched end special:", out.Points[1].Special)
	}

	if out.Modifier.Step != 10 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseSpecialNoEndNoModifier(t *testing.T) {
	out, err := Parse("london")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Special != "london" {
		t.Error("Mismatched start special:", out.Points[0].Special)
	}

	if len(out.Points) > 1 {
		t.Error("End present in parsed output")
	}

	if out.Modifier != nil {
		t.Error("Modifier present in parsed output")
	}
}

func TestParseSpecialNoEnd(t *testing.T) {
	out, err := Parse("devcon1:10")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Special != "devcon1" {
		t.Error("Mismatched start block:", out.Points[0].Special)
	}

	if len(out.Points) > 1 {
		t.Error("End present in parsed output")
	}

	if out.Modifier.Step != 10 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseDate(t *testing.T) {
	out, err := Parse("10-2021-10-03T10:20:30:25")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Date != "2021-10-03T10:20:30" {
		t.Error("Mismatched end date:", out.Points[1].Date)
	}

	if out.Modifier.Step != 25 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseDateUTC(t *testing.T) {
	out, err := Parse("10-2021-10-03T10:20:30UTC:25")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Date != "2021-10-03T10:20:30UTC" {
		t.Error("Mismatched end date:", out.Points[1].Date)
	}

	if out.Modifier.Step != 25 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

// func TestParseDateYearMonth(t *testing.T) {
// 	out, err := Parse("10-2021-10:25")

// 	if err != nil {
// 		t.Error(err)
// 	}

// 	if out.Points[0].Block != 10 {
// 		t.Error("Mismatched start block:", out.Points[0].Block)
// 	}

// 	if out.Points[1].Date != "2021-10" {
// 		t.Error("Mismatched end date:", out.Points[1].Date)
// 	}

// 	if out.Modifier.Step != 25 {
// 		t.Error("Mismatched step:", out.Modifier.Step)
// 	}
// }

func TestParseDateShort(t *testing.T) {
	out, err := Parse("10-2021-10-03:25")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Date != "2021-10-03" {
		t.Error("Mismatched end date:", out.Points[1].Date)
	}

	if out.Modifier.Step != 25 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseDateOnlyHour(t *testing.T) {
	out, err := Parse("10-2021-10-03T05")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Date != "2021-10-03T05" {
		t.Error("Mismatched end date:", out.Points[1].Date)
	}

	if out.Modifier != nil {
		t.Error("Modifier present in parsed output")
	}
}

func TestParseDateOnlyHourAndMinutes(t *testing.T) {
	out, err := Parse("10-2021-10-03T05:59")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Date != "2021-10-03T05:59" {
		t.Error("Mismatched end date:", out.Points[1].Date)
	}

	if out.Modifier != nil {
		t.Error("Modifier present in parsed output")
	}
}

func TestParseDateNoModifier(t *testing.T) {
	out, err := Parse("10-2021-10-03T05:59:29")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Block != 10 {
		t.Error("Mismatched start block:", out.Points[0].Block)
	}

	if out.Points[1].Date != "2021-10-03T05:59:29" {
		t.Error("Mismatched end date:", out.Points[1].Date)
	}

	if out.Modifier != nil {
		t.Error("Modifier present in parsed output")
	}
}

func TestParseDateNoEndNoModifier(t *testing.T) {
	out, err := Parse("2017-10-03T10:20:30")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Date != "2017-10-03T10:20:30" {
		t.Error("Mismatched start block:", out.Points[0].Date)
	}

	if len(out.Points) > 1 {
		t.Error("End present in parsed output")
	}

	if out.Modifier != nil {
		t.Error("Modifier present in parsed output")
	}
}

func TestParseShortDateNoEndNoModifier(t *testing.T) {
	out, err := Parse("2017-10-03")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Date != "2017-10-03" {
		t.Error("Mismatched start block:", out.Points[0].Date)
	}

	if len(out.Points) > 1 {
		t.Error("End present in parsed output")
	}

	if out.Modifier != nil {
		t.Error("Modifier present in parsed output")
	}
}

func TestParseDateStart(t *testing.T) {
	out, err := Parse("2017-10-03T10:20:30-1000:25")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Date != "2017-10-03T10:20:30" {
		t.Error("Mismatched start block:", out.Points[0].Date)
	}

	if out.Points[1].Block != 1000 {
		t.Error("Mismatched end date:", out.Points[1].Block)
	}

	if out.Modifier.Step != 25 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseDateStartShort(t *testing.T) {
	out, err := Parse("2017-10-03-1000:25")

	if err != nil {
		t.Error(err)
	}

	if out.Points[0].Date != "2017-10-03" {
		t.Error("Mismatched start block:", out.Points[0].Date)
	}

	if out.Points[1].Block != 1000 {
		t.Error("Mismatched end date:", out.Points[1].Block)
	}

	if out.Modifier.Step != 25 {
		t.Error("Mismatched step:", out.Modifier.Step)
	}
}

func TestParseInvalidInput(t *testing.T) {
	_, err := Parse("-100")

	if err == nil {
		t.Error("Passes for -100")
	}
}

func TestParseInvalidPeriod(t *testing.T) {
	_, err := Parse("10-100:biweekly")

	if err == nil {
		t.Error(err)
		t.Error("Passes for invalid period")
	}
}
