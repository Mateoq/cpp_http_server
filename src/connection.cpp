#include "connection.h"

#include "connection_manager.h"
#include "request_handler.h"

namespace http {
  namespace server {

    Connection::Connection(boost::asio::ip::tcp::socket socket,
													 ConnectionManager& manager,
													 RequestHandler& handler)
      : socket_(std::move(socket)),
				connection_manager_(manager),
				request_handler_(handler) {

    }

    void Connection::start() {
      do_read();
    }

    void Connection::stop() {
      socket_.close();
    }

    void Connection::do_read() {
      auto self(shared_from_this());
      socket_.async_read_some(boost::asio::buffer(buffer_),
															[this, self](boost::system::error_code err, std::size_t bytes_transferred) {
																if (!err) {
																	ParseResult result;
																	std::tie(result, std::ignore) = self->request_parser_.parse(self->request_,
																																															self->buffer_.data(),
																																															self->buffer_.data() + bytes_transferred);

																	if (result == ParseResult::good) {
																		self->request_handler_.handle_request(self->request_, self->response_);
																		self->do_write();
																	} else if (result == ParseResult::bad) {
																		self->response_ = Response::stock_response(StatusCode::bad_request);
																		self->do_write();
																	} else {
																		self->do_read();
																	}
																} else if (err != boost::asio::error::operation_aborted) {
																	self->connection_manager_.stop(shared_from_this());
																}
															});
    }

    void Connection::do_write() {
      auto self(shared_from_this());
			boost::asio::async_write(socket_, response_.to_buffers(),
															 [this, self](boost::system::error_code err, std::size_t) {
																 if (!err) {
																	 // Initiate graceful connection closure.
																	 boost::system::error_code ignored_ec;
																	 self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
																										ignored_ec);
																 }

																 if (err != boost::asio::error::operation_aborted) {
																	 self->connection_manager_.stop(shared_from_this());
																 }
															 });
    }

  } // namespace server
} // namespace http
