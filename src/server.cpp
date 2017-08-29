#include "server.h"

namespace http {
  namespace server {

    Server::Server(const std::string& address,
									 const uint16& port,
									 const std::string& doc_root)
      : io_service_(),
				signals_(io_service_),
				acceptor_(io_service_),
				connection_manager_(),
				socket_(io_service_),
				request_handler_(doc_root) {
      // Register to handle the signals that indicate when the server should exit.
      // It is safe to register for the same signal multiple times in a program,
      // provided all registration for the specified signal is made through Asio.
      signals_.add(SIGINT);
      signals_.add(SIGTERM);

#if defined(SIGQUIT)
      signals_.add(SIGQUIT);
#endif	// defined(SIGQUIT);

      do_await_stop();

      // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
      // boost::asio::ip::tcp::resolver resolver(io_service_);
      boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

      acceptor_.open(endpoint.protocol());
      acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
      acceptor_.bind(endpoint);
      acceptor_.listen();

			do_accept();
    }

    void Server::run() {
      // The io_service::run() call will block until all asynchronous operations
      // have finished. While the server is running, there is always at least one
      // asynchronous operation outstanding: the asynchronous accept call waiting
      // for new incoming connections.
			std::cout << "RUN" << std::endl;
      io_service_.run();
    }

    void Server::do_accept() {
      auto accept_handler = [this](boost::system::error_code err) {
				// Check wether the server was stopped by a signal before this
				// completion handler had a chance to run.
				if(!acceptor_.is_open()) {
					return;
				}

				if(!err) {
					connection_manager_.start(std::make_shared<Connection>(std::move(socket_),
																																 connection_manager_,
																																 request_handler_));
				}

				do_accept();
      };

      acceptor_.async_accept(socket_, accept_handler);
    }

    void Server::do_await_stop() {
      auto await_handler = [this](boost::system::error_code /*ec*/, int /*signo*/) {
				// The server is stopped by cancelling all outstanding asynchronous
        // operations. Once all operations have finished the io_service::run()
        // call will exit.
				acceptor_.close();
				connection_manager_.stop_all();
      };

      signals_.async_wait(await_handler);
    }

  } // namespace server
} // namespace http
