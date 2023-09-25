all:
	@go build && ./toml-test
	@echo "Diffing output..."
	@diff -u testdata/expected.toml testdata/output.toml
	@diff -u testdata/expected.toml testdata/output1.toml

