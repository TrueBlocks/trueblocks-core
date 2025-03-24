package identifiers

import (
	"encoding/json"
	"testing"
)

func TestIdentifiers(t *testing.T) {
	// Sub-test for TestPointToPointTypeBlock
	t.Run("PointToPointTypeBlock", func(t *testing.T) {
		point := &Point{Number: 100}
		result := getPointType(point)
		if result != BlockNumber {
			t.Error("Bad point type returned")
		}
	})

	// Sub-test for TestPointToPointTypeSpecial
	t.Run("PointToPointTypeSpecial", func(t *testing.T) {
		point := &Point{Special: "london"}
		result := getPointType(point)
		if result != BlockSpecial {
			t.Error("Bad point type returned")
		}
	})

	// Sub-test for TestPointToPointTypeDate
	t.Run("PointToPointTypeDate", func(t *testing.T) {
		point := &Point{Date: "2021-10-03"}
		result := getPointType(point)
		if result != BlockDate {
			t.Error("Bad point type returned")
		}
	})

	// Sub-test for TestModifierToModifierTypeStep
	t.Run("ModifierToModifierTypeStep", func(t *testing.T) {
		modifier := &Modifier{Step: 15}
		result := getModifierType(modifier)
		if result != Step {
			t.Error("Bad modifier type returned")
		}
	})

	// Sub-test for TestModifierToModifierTypePeriod
	t.Run("ModifierToModifierTypePeriod", func(t *testing.T) {
		modifier := &Modifier{Period: "daily"}
		result := getModifierType(modifier)
		if result != Period {
			t.Error("Bad modifier type returned")
		}
	})

	// Sub-test for TestNewBlocks
	t.Run("NewBlocks", func(t *testing.T) {
		br, err := NewBlockRange("10-1000:10")
		if err != nil {
			t.Error(err)
		}
		if br.StartType != BlockNumber {
			t.Error("StartType is not block number")
		}
		if br.Start.Number != 10 {
			t.Errorf("Wrong start")
		}
		if br.EndType != BlockNumber {
			t.Error("EndType is not block number")
		}
		if br.End.Number != 1000 {
			t.Error("Wrong end")
		}
		if br.ModifierType != Step {
			t.Error("ModifierType is not step")
		}
		if br.Modifier.Step != 10 {
			t.Error("Wrong modifier")
		}
	})

	// Sub-test for TestNewSpecial
	t.Run("NewSpecial", func(t *testing.T) {
		br, err := NewBlockRange("london:weekly")
		if err != nil {
			t.Error(err)
		}
		if br.StartType != BlockSpecial {
			t.Error("StartType is not special")
		}
		if br.Start.Special != "london" {
			t.Errorf("Wrong start")
		}
		if br.EndType != NotDefined {
			t.Error("EndType is not notdefined")
		}
		if br.ModifierType != Period {
			t.Error("ModifierType is not period")
		}
		if br.Modifier.Period != "weekly" {
			t.Error("Wrong modifier")
		}
	})

	// Sub-test for TestHandleParserErrors
	t.Run("HandleParserErrors", func(t *testing.T) {
		_, modifierErr := NewBlockRange("10-100:biweekly")
		if me, ok := modifierErr.(*WrongModifierError); ok {
			if me.Token != "biweekly" {
				t.Errorf("Wrong token: %s", me.Token)
			}
		} else {
			t.Error("Returned error is not WrongModifier")
			t.Error(modifierErr)
		}
	})

	// Sub-test for TestBlockRange_UnmarshalJSON
	t.Run("BlockRange_UnmarshalJSON", func(t *testing.T) {
		type SomeRecord struct {
			Blocks Identifier `json:"blocks"`
		}
		var record SomeRecord
		source := []byte(`{"blocks":"000000000-10567003"}`)
		err := json.Unmarshal(source, &record)
		if err != nil {
			t.Error(err)
		}
		if record.Blocks.StartType != BlockNumber {
			t.Errorf("Wrong StartType %d", record.Blocks.StartType)
		}
		if record.Blocks.EndType != BlockNumber {
			t.Errorf("Wrong EndType %d", record.Blocks.EndType)
		}
		if record.Blocks.Start.Number != uint(0) {
			t.Error("Wrong start value")
		}
		if record.Blocks.End.Number != uint(10567003) {
			t.Errorf("Wrong end value %d", record.Blocks.End.Number)
		}
	})

	// Sub-test for TestToString
	t.Run("ToString", func(t *testing.T) {
		br, err := NewBlockRange("1234")
		if err != nil {
			t.Errorf("Could not parse block")
		}
		expected := `{
  "startType": 1,
  "start": {
    "number": 1234
  },
  "end": {},
  "modifier": {}
}`
		got := br.String()
		if got != expected {
			t.Errorf("String printer for blockRange not equal to expected:\n%s\n%s", got, expected)
		}
	})
}
