all: chunkify create list unchunkify
	@ls -l bin

chunkify: chunkify.go
	@echo building chunkify...
	@mkdir -p bin
	go build chunkify.go
	@mv chunkify bin/chunkify
	@echo done

create: create.go
	@echo building create...
	@mkdir -p bin
	go build create.go
	@mv create bin/create
	@echo done

list: list.go
	@echo building list...
	@mkdir -p bin
	go build list.go
	@mv list bin/list
	@echo done

unchunkify: unchunkify.go
	@echo building unchunkify...
#	@mkdir -p bin
#	go build unchunkify.go
#	@mv unchunkify bin/unchunkify
#	@echo done

clean:
	@echo cleaning...
	rm -f bin/*
