package progress

type Event uint

const (
	BeforeStart Event = iota
	Start
	Update
	Done
	Error
	AllDone
)

type Progress struct {
	Event   Event
	Message string
	Payload interface{}
}

func MakeChan() chan *Progress {
	return make(chan *Progress, 100)
}
