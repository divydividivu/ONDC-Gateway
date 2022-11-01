all: client

_cgo_export.h: request.go
	go tool cgo -exportheader $@ $^

request.a: request.go
	go build -buildmode=c-archive $^

client: client.c _cgo_export.h request.a
	$(CC) -o $@ $^ -lpthread

clean:
	rm -fr _cgo_export.h request.a _obj client