#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "globals.h"
#include "server.h"

int main(int argc, char* argv[]) {
  std::cout << "Hello!" << std::endl;

  // for (int i = 0; i < argc; ++i) {
  //   std::cout << argv[i] << std::endl;
  // }

  try {
    // Check command line arguments.
    if (argc != 4) {
      std::cerr << "Usage: HttpServer <address> <port> <doc_root>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 80 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 80 . \n";
      return 1;
    }

    // Initialise the server.
    http::server::Server server(argv[1], (uint16)std::stoi(argv[2]), argv[3]);

    // Run the server until stopped.
    server.run();
  } catch (std::exception& e) {
    std::cerr << "Esception: " << e.what() << "\n";
  }

  return 0;
}
