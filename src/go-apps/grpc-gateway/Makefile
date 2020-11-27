GRPC_PLUGIN=`which grpc_cpp_plugin`
PROTO=echo_service.proto
PROTO_SRCS=$(PROTO:.proto=.pb.cc) $(PROTO:.proto=.grpc.pb.cc)
PROTO_OBJS=$(PROTO_SRCS:.cc=.o)

ANN_PROTO1=google/api/annotations.proto
ANN_PROTO2=google/api/http.proto
ANN_PROTOS=third_party/googleapis/$(ANN_PROTO1) third_party/googleapis/$(ANN_PROTO2)
ANN_SRCS=$(ANN_PROTO1:.proto=.pb.cc) $(ANN_PROTO2:.proto=.pb.cc)
ANN_OBJS=$(ANN_SRCS:.cc=.o)
CXXFLAGS=-std=c++11 -I. -I/usr/local/include

HTTP_SRCS=$(HTTP_PROTO:.proto=.pb.cc)
HTTP_OBJS=$(HTTP_PROTO:.cc=.o)

default: server

$(PROTO_SRCS) $(PROTO_SRCS:.cc=.h): $(PROTO)
	protoc -I/usr/local/include -Ithird_party/googleapis -I. --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_PLUGIN) $(PROTO)

$(PROTO_OBJS): $(PROTO_SRCS) $(PROTO_SRCS:.cc=.h) $(ANN_SRCS:.cc=.h)

$(ANN_SRCS) $(ANN_SRCS:.cc=.h): $(ANN_PROTOS)
	protoc -I/usr/local/include -I third_party/googleapis --cpp_out=.  $(ANN_PROTOS)

$(ANN_OBJS): $(ANN_SRCS) $(ANN_SRCS:.cc=.h)

gateway.so gateway.h: $(ANN_PROTOS) $(PROTO) main.go
	go build -o gateway.so -buildmode=c-shared github.com/trueblocks-core/src/go-apps/grpc-gateway

server_main.o: server_main.cc $(PROTO_SRCS:.cc=.h) $(ANN_SRCS:.cc=.h) gateway.h

server: server_main.o $(PROTO_OBJS) $(ANN_OBJS) gateway.so
	g++ -lgrpc++ -lprotobuf -lgflags -o server server_main.o $(PROTO_OBJS) $(ANN_OBJS) gateway.so

.PHONY: default
