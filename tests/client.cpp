// greeter_client.cc
#include <iostream>
#include <string>
#include <grpc/grpc.h>

// Include the generated files
#include "hello.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// Use the generated types
using hello::Greeter;
using hello::HelloRequest;
using hello::HelloReply;

class GreeterClient {
  public:
	// Client constructor takes a channel to connect to the server
	GreeterClient(std::shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {
	}

	std::string SayHello(const std::string& user) {
		// 1. Set up the request and response objects
		HelloRequest request;
		request.set_name(user);
		HelloReply reply;

		// 2. Context for the client call (e.g., deadline, metadata)
		ClientContext context;

		// 3. The actual remote procedure call
		Status status = stub_->SayHello(&context, request, &reply);

		// 4. Act upon the status
		if (status.ok()) {
			return reply.message();
		} else {
			std::cout << "RPC failed: " << status.error_code() << ": " << status.error_message() << std::endl;
			return "RPC failed";
		}
	}

  private:
	std::unique_ptr<Greeter::Stub> stub_;
};

int main() {
	// 1. Create a channel (connect to the server address)
	std::string target_str = "localhost:50051";

	// InsecureChannelCredentials is used for development/unencrypted connections.
	std::shared_ptr<Channel> channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());

	// 2. Create the client object
	GreeterClient greeter(channel);

	// 3. Call the RPC method
	std::string user  = "C++ gRPC User";
	std::string reply = greeter.SayHello(user);

	// 4. Output the result
	std::cout << "Greeter received: " << reply << std::endl;

	return 0;
}