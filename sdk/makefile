all:
	@cd ~/Development/trueblocks-core/build && make sdk
	@cd typescript && make
	@cd python && make

publish:
	@cd typescript && yarn publish
	@cd python && make
