package types

import "testing"

func Test_serialize(t *testing.T) {
	type args struct {
		input any
	}
	tests := []struct {
		name string
		args args
		want string
	}{
		{
			name: "simple slice",
			args: args{
				input: []string{
					"0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c",
					"0x0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc",
				},
			},
			want: "[0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c|0x0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc]",
		},
		{
			name: "simple map",
			args: args{
				input: map[string]any{
					"string": "0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c",
					"number": 123,
				},
			},
			want: "{number:123|string:0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c}",
		},
		{
			name: "simple map with name key",
			args: args{
				input: map[string]any{
					"string": "0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c",
					"number": 123,
					"name":   "_delegate",
				},
			},
			want: "{name:_delegate|number:123|string:0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c}",
		},
		{
			name: "simple struct",
			args: args{
				input: struct {
					Key1       string `json:"key1"`
					Key2       int    `json:"key2"`
					privateKey int
				}{
					Key1:       "string key",
					Key2:       400,
					privateKey: 42,
				},
			},
			want: "{key1:string key|key2:400}",
		},
		{
			name: "simple struct with omit tag",
			args: args{
				input: struct {
					Key1       string `json:"key1"`
					Key2       int    `json:"key2,omitempty"`
					privateKey int
				}{
					Key1:       "string key",
					privateKey: 42,
				},
			},
			want: "{key1:string key}",
		},
		{
			name: "simple struct with explicit omit",
			args: args{
				input: struct {
					Key1       string `json:"key1"`
					Key2       int    `json:"-"`
					privateKey int
				}{
					Key1:       "string key",
					privateKey: 42,
				},
			},
			want: "{key1:string key}",
		},
		{
			name: "simple struct with name key",
			args: args{
				input: struct {
					Key1       string `json:"key1"`
					Key2       int    `json:"key2"`
					privateKey int
					Name       string `json:"name"`
				}{
					Key1:       "string key",
					Key2:       400,
					privateKey: 42,
					Name:       "someName",
				},
			},
			want: "{name:someName|key1:string key|key2:400}",
		},
		// A bit more complex tests
		{
			name: "nested slice",
			args: args{
				input: [][]string{
					{
						"0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c",
					},
					{
						"0x0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc",
					},
				},
			},
			want: "[[0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c]|[0x0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc]]",
		},
		{
			name: "slice with nested struct",
			args: args{
				input: []struct {
					Name  string `json:"name"`
					Value int    `json:"value"`
				}{
					{
						Name: "first", Value: 1,
					},
					{
						Name: "second", Value: 2,
					},
				},
			},
			want: "[{name:first|value:1}|{name:second|value:2}]",
		},
		{
			name: "nested map",
			args: args{
				input: map[string]any{
					"key1": "firstKey",
					"key2": map[string]any{
						"map2key1": "secondMapSecondKey",
						"name":     "someName",
					},
				},
			},
			want: "{key1:firstKey|key2:{name:someName|map2key1:secondMapSecondKey}}",
		},
		{
			name: "map with nested object",
			args: args{
				input: map[string]struct {
					Name  string `json:"name"`
					Value int    `json:"value"`
				}{
					"key1": {
						Name: "first", Value: 1,
					},
					"key2": {
						Name: "second", Value: 2,
					},
				},
			},
			want: "{key1:{name:first|value:1}|key2:{name:second|value:2}}",
		},
		{
			name: "nested structs",
			args: args{
				input: struct {
					Key1 string `json:"key1"`
					Key2 struct {
						Name  string `json:"name"`
						Value int    `json:"value"`
					} `json:"key2"`
				}{
					Key1: "string key",
					Key2: struct {
						Name  string `json:"name"`
						Value int    `json:"value"`
					}{
						Name: "first", Value: 1,
					},
				},
			},
			want: "{key1:string key|key2:{name:first|value:1}}",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := serialize(tt.args.input, 100); got != tt.want {
				t.Errorf("serialize() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestSerializeTooDeep(t *testing.T) {
	input := [][][]string{
		{
			{
				"some string",
			},
		},
	}
	got := serialize(input, 2)

	if got != "[["+tooDeepMessage+"]]" {
		t.Fatal("wrong value:", got)
	}
}
