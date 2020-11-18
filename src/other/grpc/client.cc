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
#include <string>

#ifdef BAZEL_BUILD
#include "examples/protos/eth.grpc.pb.h"
#else
#include "eth.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using rpcdaemon::BlockReply;
using rpcdaemon::BlockRequest;
using rpcdaemon::EthService;

//----------------------------------------------------------------------------------------------
class EthServiceClient {
public:
  explicit EthServiceClient(std::shared_ptr<Channel> channel);
  std::string GetBlock(uint64_t f, uint64_t l = UINT_MAX, uint64_t s = 1);

private:
  std::unique_ptr<EthService::Stub> stub_;
};

//----------------------------------------------------------------------------------------------
EthServiceClient::EthServiceClient(std::shared_ptr<Channel> channel) : stub_(EthService::NewStub(channel)) {
}

//----------------------------------------------------------------------------------------------
std::string EthServiceClient::GetBlock(uint64_t f, uint64_t l, uint64_t s) {
  BlockRequest request;
  request.set_first(f);
  request.set_last(l == UINT_MAX ? f : l);
  request.set_step(s);

  ClientContext context;
  CompletionQueue cq;
  BlockReply reply;
  Status status;

  std::unique_ptr<ClientAsyncResponseReader<BlockReply> > rpc(stub_->PrepareAsyncGetBlock(&context, request, &cq));

  rpc->StartCall();
  rpc->Finish(&reply, &status, (void*)1);

  void* got_tag;
  bool ok = false;
  GPR_ASSERT(cq.Next(&got_tag, &ok));
  GPR_ASSERT(got_tag == (void*)1);
  GPR_ASSERT(ok);

  return status.ok() ? reply.message() : "RPC failed";
}

//----------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
  auto credentials = grpc::InsecureChannelCredentials();
  auto channel = grpc::CreateChannel("localhost:50051", credentials);

  EthServiceClient EthService(channel);
  std::cout << "\n\t" << EthService.GetBlock(12) << std::endl;

  return 0;
}
