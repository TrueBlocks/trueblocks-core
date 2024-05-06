package base

// Min calculates the minimum between two unsigned integers (golang has no such function)
func Min[T Numeral | int | float64 | uint32 | int64 | uint64](x, y T) T {
	if x < y {
		return x
	}
	return y
}

// Max calculates the max between two unsigned integers (golang has no such function)
func Max[T Numeral | int | float64 | uint32 | int64 | uint64](x, y T) T {
	if x > y {
		return x
	}
	return y
}
