#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <signal.h>

#include <boost/asio.hpp>

#include "globals.h"
#include "connection.h"
#include "connection_manager.h"
#include "request_handler.h"

namespace http {
  namespace server {

    // The top-level class of the HTTP server.
    class Server {
    public:
      Server(const Server&) = delete;
      Server& operator=(const Server&) = delete;

      // Construct the server to listen on the specified TCP address and
      // port, and serve up files from the given directory.
      explicit Server(const std::string& address,
											const uint16& port,
											const std::string& doc_root);

      // Run the server's io_service loop.
      void run();

    private:
      // Perdorm an asynchronous accep operation.
      void do_accept();

      // Wait for a request to stop the server.
      void do_await_stop();

      // The io_service used to perform aynchronous operations.
      boost::asio::io_service io_service_;

      // The signal_set is used to register for process termination notifications.
      boost::asio::signal_set signals_;

      // Acceptor used to listen for incoming connections.
      boost::asio::ip::tcp::acceptor acceptor_;

      // The connection manager which owns all live connections.
      ConnectionManager connection_manager_;

      // The next socket to be accepted.
      boost::asio::ip::tcp::socket socket_;

      // The handler for all incoming requests.
      RequestHandler request_handler_;
    };

  } // namespace server
} // namespace http
