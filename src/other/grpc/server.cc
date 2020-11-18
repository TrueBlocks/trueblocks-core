/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>

#ifdef BAZEL_BUILD
#include "examples/protos/eth.grpc.pb.h"
#else
#include "eth.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using rpcdaemon::BlockReply;
using rpcdaemon::BlockRequest;
using rpcdaemon::EthService;

extern const char* STR_FAKE_BLOCK;

//--------------------------------------------------------
class ServerImpl final {
public:
  ~ServerImpl() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  }

  // There is no shutdown handling in this code.
  void Run() {
    std::string server_address("0.0.0.0:50051");

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&service_);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    cq_ = builder.AddCompletionQueue();
    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    // Proceed to the server's main loop.
    HandleRpcs();
  }

private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData {
  public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(EthService::AsyncService* service, ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
      // Invoke the serving logic right away.
      Proceed();
    }

    void Proceed() {
      if (status_ == CREATE) {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;

        // As part of the initial CREATE state, we *request* that the system
        // start processing GetBlock requests. In this request, "this" acts
        // are the tag uniquely identifying the request (so that different
        // CallData instances can serve different requests concurrently), in
        // this case the memory address of this CallData instance.
        service_->RequestGetBlock(&ctx_, &request_, &responder_, cq_, cq_, this);
      } else if (status_ == PROCESS) {
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        new CallData(service_, cq_);

        // The actual processing.
        static int cnt = 0;
        std::ostringstream os;
        reply_.set_message(STR_FAKE_BLOCK);
        std::cerr << "\e[1;34mI AM HERE\e[0m: " << (++cnt) << std::endl;

        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.
        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);

      } else {
        GPR_ASSERT(status_ == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }

  private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    EthService::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    // What we get from the client.
    BlockRequest request_;
    // What we send back to the client.
    BlockReply reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<BlockReply> responder_;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
  };

  // This can be run in multiple threads if needed.
  void HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, cq_.get());
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      GPR_ASSERT(cq_->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->Proceed();
    }
  }

  std::unique_ptr<ServerCompletionQueue> cq_;
  EthService::AsyncService service_;
  std::unique_ptr<Server> server_;
};

//--------------------------------------------------------
int main(int argc, char** argv) {
  ServerImpl server;
  server.Run();

  return 0;
}

const char* STR_FAKE_BLOCK =
    "{\"author\": \"0x0193d941b50d91be6567c7ee1c0fe7af498b4137\",\n"
    "\"difficulty\": \"0x3ffffa000\",\n"
    "\"extraData\": \"0x476574682f76312e302e302f6c696e75782f676f312e342e32\",\n"
    "\"gasLimit\": \"0x1388\",\n"
    "\"gasUsed\": \"0x0\",\n"
    "\"hash\": \"0xc63f666315fa1eae17e354fab532aeeecf549be93e358737d0648f50d57083a0\",\n"
    "\"logsBloom\": "
    "\"0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000\",\n"
    "\"miner\": \"0x0193d941b50d91be6567c7ee1c0fe7af498b4137\",\n"
    "\"mixHash\": \"0xbe4ba21fe1ecb061e44f178428c772d2a0f59a7aafb5ed4e198eba4df3656e52\",\n"
    "\"nonce\": \"0x5f6a5cc5c36e6627\",\n"
    "\"number\": \"0xc\",\n"
    "\"parentHash\": \"0x3f5e756c3efcb93099361b7ddd0dabfeaa592439437c1c836e443ccb81e93242\",\n"
    "\"receiptsRoot\": \"0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421\",\n"
    "\"sealFields\": [\n"
    "  \"0xa0be4ba21fe1ecb061e44f178428c772d2a0f59a7aafb5ed4e198eba4df3656e52\",\n"
    "  \"0x885f6a5cc5c36e6627\"\n"
    "],\n"
    "\"sha3Uncles\": \"0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347\",\n"
    "\"size\": \"0x219\",\n"
    "\"stateRoot\": \"0x821c41f30a2fd9580605363784a8a2a6575b255ec37cacf87fe52715b8828d8e\",\n"
    "\"timestamp\": \"0x55ba42c0\",\n"
    "\"totalDifficulty\": \"0x33f2ffe033\",\n"
    "\"transactions\": [],\n"
    "\"transactionsRoot\": \"0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421\",\n"
    "\"uncles\": []}";
