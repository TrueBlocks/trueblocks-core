// Package tslib provides conversions between four data types: block numbers, dates, Linux
// timestamps, and "special" named blocks. The function names are self-explanitory.
//
// Note: Because the relationship between block numbers and timestamps is a per-chain value,
// some of these functions require a `chain` name. It is assumed the chain is properly configured.
package tslib
