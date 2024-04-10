package output

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Remove rpc.MetaData and use types.MetaData throughout

type FieldType int

var comma = ","

const (
	FieldArray FieldType = iota
	FieldObject
)

type position int

const (
	positionEmpty position = iota
	positionInRoot
	positionInArray
	positionInObject
	positionRootClosed
)

type state = struct {
	position
	children int
}

type DefaultField struct {
	Key string
	FieldType
}

// JsonWriter can write JSON object in portions.
type JsonWriter struct {
	// current state of the JSON object (are we inside of
	// an array or object? are there any children?)
	state state
	// previous state is helpful when closing an object or
	// array, so we will keep it as a stack
	previousStates []state
	// the writer that we will output to
	outputWriter io.Writer
	indentLevel  int
	indentString string
	// errors to output
	errs []string
	// function to get meta data
	GetMeta func() (*types.MetaData, error)
	// flag indicating if we should output `meta` object as
	// well
	ShouldWriteMeta bool
	// Should writer write newline after `Close`
	ShouldWriteNewline bool
	DefaultField
}

// NewJsonWriter creates JsonWriter with some useful defaults
func NewJsonWriter(w io.Writer) *JsonWriter {
	return &JsonWriter{
		outputWriter: w,
		indentString: "  ",
		GetMeta: func() (*types.MetaData, error) {
			return &types.MetaData{}, nil
		},
	}
}

func NewDefaultJsonWriter(w io.Writer, shouldWriteNewline bool) *JsonWriter {
	jw := NewJsonWriter(w)
	jw.DefaultField = DefaultField{
		Key:       "data",
		FieldType: FieldArray,
	}
	jw.ShouldWriteNewline = shouldWriteNewline
	return jw
}

// popState removes the last item from state stack and returns it
func (w *JsonWriter) popState() (prevState state) {
	lastIndex := len(w.previousStates) - 1
	prevState = w.previousStates[lastIndex]
	w.previousStates = w.previousStates[:lastIndex]
	return
}

// writeRaw writes directly to `outputWriter` without performing any
// additional operations. It is a foundation for all other Write*
// functions
func (w *JsonWriter) writeRaw(toWrite []byte) (n int, err error) {
	return w.outputWriter.Write(toWrite)
}

func (w *JsonWriter) writeNewline() (n int, err error) {
	return w.writeRaw([]byte("\n"))
}

// writeErrors writes `errors` array
func (w *JsonWriter) writeErrors() (n int, err error) {
	return w.WriteCompoundItem("errors", w.errs)
}

// openRoot prints the beginning "{"
func (w *JsonWriter) openRoot() (n int, err error) {
	// You can only open root once
	if w.state.position != positionEmpty {
		err = errors.New("root object is already opened")
		return
	}

	n, err = w.writeRaw([]byte("{"))
	w.indentLevel = 1
	w.previousStates = append(w.previousStates, w.state)
	w.state.position = positionInRoot
	w.state.children = 0

	if w.DefaultField.Key != "" {
		dn, err := w.OpenField(w.DefaultField.Key, w.DefaultField.FieldType)
		n += dn
		if err != nil {
			return n, err
		}
	}

	return
}

// GetPrefixForLevel returns indent string for given intent level
func (w *JsonWriter) GetPrefixForLevel(level int) (prefix string) {
	if level == 0 {
		return
	}
	return strings.Repeat(w.indentString, level)
}

// GetCurrentPrefix returns indent string for current indent level.
// The returned string can be passed to json.MarshalIndent as `prefix`
// to match this writer's indentation.
func (w *JsonWriter) GetCurrentPrefix() (prefix string) {
	return w.GetPrefixForLevel(w.indentLevel)
}

// Indent writes indentation string
func (w *JsonWriter) Indent() {
	prefix := w.GetCurrentPrefix()
	_, _ = w.writeRaw([]byte(prefix))
}

// Write writes bytes p, adding indentation and comma before if needed.
// In most cases, you should use `WriteItem` instead.
func (w *JsonWriter) Write(p []byte) (n int, err error) {
	if w.state.position == positionEmpty {
		n, err = w.openRoot()
	}
	if err != nil {
		return
	}
	if shouldInsertComma(w.state) {
		bw, cerr := w.writeRaw([]byte(comma))
		n += bw
		err = cerr
		if err != nil {
			return
		}
	}
	bw, err := w.writeNewline()
	n += bw
	if err != nil {
		return
	}
	w.Indent()
	if w.state.position == positionInArray || w.state.position == positionInObject || w.state.position == positionInRoot {
		w.state.children++
	}
	return w.writeRaw(p)
}

// WriteItem writes `value` under the key `key`
func (w *JsonWriter) WriteItem(key string, value string) (n int, err error) {
	if key == "" {
		return w.Write([]byte(value))
	}
	return w.Write([]byte(fmt.Sprintf(`"%s": %s`, key, value)))
}

// WriteError saves error to be written when the writer is `Close`d
func (w *JsonWriter) WriteError(err error) {
	w.errs = append(w.errs, err.Error())
}

// WriteCompoundItem makes it easier to write an object or array.
func (w *JsonWriter) WriteCompoundItem(key string, obj any) (n int, err error) {
	if w.state.position == positionEmpty {
		_, _ = w.openRoot()
	}
	prefix := w.GetPrefixForLevel(w.indentLevel)
	marshalled, err := json.MarshalIndent(obj, prefix, w.indentString)
	if err != nil {
		return
	}
	return w.WriteItem(key, string(marshalled))
}

// Close writes errors and meta data (if requested) and then the ending "}"
func (w *JsonWriter) Close() error {
	defer func() {
		if !w.ShouldWriteNewline {
			return
		}
		_, _ = w.writeNewline()
	}()
	// If we didn't write anything, but there is default key, we need
	// to write it
	if w.state.position == positionEmpty && w.DefaultField.Key != "" {
		_, _ = w.openRoot()
	}
	// CloseField if in field
	if w.state.position == positionInArray {
		_, _ = w.CloseField(FieldArray)
	}
	if w.state.position == positionInObject {
		_, _ = w.CloseField(FieldObject)
	}
	// Print meta, if any
	if w.ShouldWriteMeta {
		meta, err := w.GetMeta()
		if err != nil {
			w.WriteError(err)
		}
		_, _ = w.WriteCompoundItem("meta", meta)
	}
	// Print errors, if any
	if len(w.errs) > 0 {
		_, _ = w.writeErrors()
	}
	// Print closing bracket
	if w.state.position == positionEmpty && w.DefaultField.Key == "" {
		_, err := w.writeRaw([]byte("{}"))
		return err
	}
	_, _ = w.CloseField(FieldObject)
	return nil
}

// OpenField writes opening "[" or "{", depending on `state.position`
func (w *JsonWriter) OpenField(key string, fieldType FieldType) (n int, err error) {
	if w.state.position == positionEmpty {
		n, err = w.openRoot()
	}
	if err != nil {
		return
	}

	if shouldInsertComma(w.state) {
		bw, err := w.writeRaw([]byte(comma))
		n += bw
		if err != nil {
			return n, err
		}
	}

	_, _ = w.writeNewline()
	w.Indent()

	bracket := "["
	newPosition := positionInArray
	if fieldType == FieldObject {
		bracket = "{"
		newPosition = positionInObject
	}

	if key == "" {
		n, err = w.writeRaw([]byte(bracket))
	} else {
		n, err = w.writeRaw([]byte(fmt.Sprintf(`"%s": %s`, key, bracket)))
	}
	if err != nil {
		return
	}

	w.indentLevel++
	w.state.children++
	w.previousStates = append(w.previousStates, w.state)
	w.state = state{
		position: newPosition,
		children: 0,
	}
	return
}

// CloseField writes closing "]" or "}", depending on `state.position`
func (w *JsonWriter) CloseField(fieldType FieldType) (n int, err error) {
	indentMod := -1
	bracket := "]"
	if fieldType == FieldObject {
		bracket = "}"
	}
	w.indentLevel = w.indentLevel + indentMod
	if w.state.children > 0 {
		_, _ = w.writeNewline()
		w.Indent()
	}
	n, err = w.writeRaw([]byte(bracket))
	if err != nil {
		return
	}
	previousState := w.popState()
	w.state = previousState

	return
}

func (w *JsonWriter) GetOutputWriter() *io.Writer {
	return &w.outputWriter
}

// Helpers

func shouldInsertComma(s state) bool {
	return s.children > 0
}
