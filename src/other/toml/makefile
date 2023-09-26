all:
	@go build && ./toml-test

diff:
	@make all
	@echo "Diffing output..."
	@diff testdata/expected.toml testdata/output.toml
	@diff testdata/expected.toml testdata/output1.toml

