package locations

import (
	"bytes"
	"fmt"
	"io"
	"sync"
)

// Each Storer implementation is global and thread-safe to save resources
// when reading/writing large number of items
var memoryInstance *memory
var memoryInitOnce sync.Once

// Helper interface that will allow us to implement memoryReadWriteCloser.Close
type unlocker interface {
	unlock()
}

// We have to use ReadWriteCloser to conform with Storer interface
type memoryReadWriteCloser struct {
	buf bytes.Buffer
	u   unlocker
}

func (m *memoryReadWriteCloser) Write(p []byte) (n int, err error) {
	return m.buf.Write(p)
}

func (m *memoryReadWriteCloser) Read(p []byte) (n int, err error) {
	return m.buf.Read(p)
}

func (m *memoryReadWriteCloser) Close() error {
	m.u.unlock()
	return nil
}

type memory struct {
	records map[string]*memoryReadWriteCloser
	mutex   sync.Mutex
}

// unlock is being called by memoryReadWriteCloser.Close to unlock the mutex
func (l *memory) unlock() {
	l.mutex.Unlock()
}

// Memory returns an instance of memory Storer, ready to be used
func Memory() (*memory, error) {
	memoryInitOnce.Do(func() {
		memoryInstance = new(memory)
		memoryInstance.records = make(map[string]*memoryReadWriteCloser)
	})
	return memoryInstance, nil
}

// Writer returns io.WriterCloser for the item at given path
func (l *memory) Writer(path string) (io.WriteCloser, error) {
	l.mutex.Lock()

	record, ok := l.records[path]
	if !ok {
		record = &memoryReadWriteCloser{
			u: l,
		}
		l.records[path] = record
	}

	return record, nil
}

// Reader returns io.ReaderCloser for the item at given path
func (l *memory) Reader(path string) (io.ReadCloser, error) {
	l.mutex.Lock()

	record, ok := l.records[path]
	if !ok || record == nil {
		l.mutex.Unlock()
		return nil, fmt.Errorf("%s: %w", path, ErrNotFound)
	}
	return record, nil
}

// Remove removes the item at given path
func (l *memory) Remove(path string) error {
	l.mutex.Lock()
	defer l.mutex.Unlock()
	delete(l.records, path)
	return nil
}

func (l *memory) Stat(path string) (*ItemInfo, error) {
	l.mutex.Lock()
	defer l.mutex.Unlock()
	var fileSize int
	if item, ok := l.records[path]; ok {
		fileSize = item.buf.Len()
	}
	return &ItemInfo{
		fileSize,
		path,
	}, nil
}
