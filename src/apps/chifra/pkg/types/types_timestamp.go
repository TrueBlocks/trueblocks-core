package types

// EXISTING_CODE
import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawTimestamp interface {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTimestamp struct {
	BlockNumber uint64         `json:"blockNumber"`
	Timestamp   base.Timestamp `json:"timestamp"`
	Diff        base.Timestamp `json:"diff"`
	raw         *RawTimestamp  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleTimestamp) Raw() *RawTimestamp {
	return s.raw
}

func (s *SimpleTimestamp) SetRaw(raw *RawTimestamp) {
	s.raw = raw
}

func (s *SimpleTimestamp) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleTimestamp) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleTimestamp) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
