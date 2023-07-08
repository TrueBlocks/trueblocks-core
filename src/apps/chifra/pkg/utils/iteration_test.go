package utils

import (
	"context"
	"fmt"
	"sync"
	"testing"
	"time"
)

func TestIterateOverMap(t *testing.T) {
	type testStruct struct {
		value bool
	}
	itemCount := 100
	target := make(map[int]testStruct, itemCount)
	result := make([]struct {
		key   int
		value testStruct
	}, 0, itemCount)
	for i := 0; i < itemCount; i++ {
		target[i] = testStruct{value: false}
	}
	var mu sync.Mutex
	errorChan := make(chan error)
	IterateOverMap(context.Background(), errorChan, target, func(key int, value testStruct) error {
		mu.Lock()
		defer mu.Unlock()
		item := target[key]
		item.value = true
		result = append(result, struct {
			key   int
			value testStruct
		}{key: key, value: item})
		// target[key] = item
		return nil
	})

	if err := <-errorChan; err != nil {
		t.Fatal(err)
	}

	fmt.Printf("%+v\n", target)
	// for key, value := range target {
	for _, v := range result {
		if !v.value.value {
			t.Fatal("wrong value (false) for key", v.key)
		}
	}
}

var benchmarkItemCount = 13000

func BenchmarkSync(b *testing.B) {
	target := make(map[int]bool, benchmarkItemCount)
	for j := 0; j < benchmarkItemCount; j++ {
		target[j] = true
	}

	for i := 0; i < b.N; i++ {
		for _, v := range target {
			time.Sleep(5 * time.Millisecond)
			_ = v
		}
	}
}

func BenchmarkIterateOverMap(b *testing.B) {
	target := make(map[int]bool, benchmarkItemCount)
	for j := 0; j < benchmarkItemCount; j++ {
		target[j] = true
	}

	ctx := context.Background()
	errorChan := make(chan error)
	for i := 0; i < b.N; i++ {
		IterateOverMap(ctx, errorChan, target, func(key int, value bool) error {
			time.Sleep(5 * time.Millisecond)
			return nil
		})

		if err := <-errorChan; err != nil {
			b.Fatal(err)
		}
	}
}
