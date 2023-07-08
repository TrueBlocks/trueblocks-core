package utils

import (
	"context"
	"fmt"
	"runtime"
	"sync"

	"github.com/panjf2000/ants/v2"
)

// stepFunc represents single iteration step
type stepFunc[Key, Value any] func(key Key, value Value) error

// IterateOverMap distributes batches of `target` items to a pool of goroutines which execute
// `step` for every `target` item. Use benchmarks to make sure that concurrent iteration
// is faster than synchronous one.
func IterateOverMap[Key comparable, Value any](ctx context.Context, errorChan chan error, target map[Key]Value, step stepFunc[Key, Value]) {
	var wg sync.WaitGroup
	defer close(errorChan)

	itemsPerPool := len(target) / runtime.GOMAXPROCS(0)
	if itemsPerPool < 1 {
		// We don't really want this to happen, if target is small then synchronous iteration
		// will be faster.
		itemsPerPool = 1
	}

	type stepArguments struct {
		key   Key
		value Value
	}

	pool, err := ants.NewPoolWithFunc(runtime.GOMAXPROCS(0), func(i interface{}) {
		defer wg.Done()
		select {
		case <-ctx.Done():
			return
		default:
			args, ok := i.([]stepArguments)
			if !ok {
				errorChan <- fmt.Errorf("invalid worker argument: %v", i)
				return
			}
			for _, arg := range args {
				if err := step(arg.key, arg.value); err != nil {
					errorChan <- err
					return
				}
			}
		}
	})
	if err != nil {
		errorChan <- err
		return
	}
	defer pool.Release()

	// payload is a batch of map items that we distribute to the pool
	payload := make([]stepArguments, 0, itemsPerPool)
	count := 0

	// distribute batches of map items
	for key, value := range target {
		count++
		payload = append(payload, stepArguments{key, value})

		if count%itemsPerPool == 0 {
			wg.Add(1)
			err = pool.Invoke(payload)
			if err != nil {
				errorChan <- err
				return
			}
			// new batch
			payload = make([]stepArguments, 0, itemsPerPool)
		}
	}

	// don't forget about the remainder...
	if len(payload) > 0 {
		wg.Add(1)
		err = pool.Invoke(payload)
		if err != nil {
			errorChan <- err
			return
		}
	}

	wg.Wait()
}
