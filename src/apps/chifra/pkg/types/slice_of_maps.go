package types

import "sort"

type MappedType interface {
	Transaction |
		Block |
		LightBlock |
		Appearance |
		Withdrawal |
		[]Withdrawal |
		[]Result |
		[]*Log |
		[]*Trace |
		Token |
		bool
}

// TODO: Do we want this to be configurable? Maybe, maybe not
var AppMapSize int = 20

func AsSliceOfMaps[T MappedType](apps []Appearance, reversed bool) ([]map[Appearance]*T, int, error) {
	sort.Slice(apps, func(i, j int) bool {
		if reversed {
			i, j = j, i
		}
		if apps[i].BlockNumber == apps[j].BlockNumber {
			return apps[i].TransactionIndex < apps[j].TransactionIndex
		}
		return apps[i].BlockNumber < apps[j].BlockNumber
	})

	arrayOfMaps := make([]map[Appearance]*T, 0, len(apps))
	curMap := make(map[Appearance]*T)
	for i := 0; i < len(apps); i++ {
		// TODO: Do we want this to be configurable? Maybe, maybe not
		if len(curMap) == AppMapSize {
			arrayOfMaps = append(arrayOfMaps, curMap)
			curMap = make(map[Appearance]*T)
		}
		curMap[apps[i]] = nil
	}

	if len(curMap) > 0 {
		arrayOfMaps = append(arrayOfMaps, curMap)
	}

	return arrayOfMaps, len(apps), nil
}
