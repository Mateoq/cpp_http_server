#include "request_handler.h"

namespace http {
  namespace server {

    RequestHandler::RequestHandler(const std::string& doc_root)
      : doc_root_(doc_root) {

    }

    void RequestHandler::handle_request(const Request& req, Response res) {
      // Decode url to path.
      std::string request_path;
      if (!url_decode(req.uri, request_path)) {
				res = Response::stock_response(StatusCode::bad_request);
				return;
      }

      // Request path must be absolute and not cotain "..".
      if (request_path.empty() || request_path[0] != '/' ||
					request_path.find("..") != std::string::npos) {
				res = Response::stock_response(StatusCode::bad_request);
				return;
      }

      // If path ends in slash (i.e. is a directory) then add "index.html".
      if (request_path[request_path.size() - 1] == '/') {
				request_path += "index.html";
      }

      // Determine the file extension.
      std::size_t last_slash_pos = request_path.find_last_of("/");
      std::size_t last_dot_pos = request_path.find_last_of(".");
      std::string extension;
      if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
				extension = request_path.substr(last_dot_pos + 1);
      }

      // Open the file to send back.
      std::string full_path = doc_root_ + request_path;
      std::ifstream stream(full_path.c_str(), std::ios::in | std::ios::binary);
      if (!stream) {
				res = Response::stock_response(StatusCode::not_found);
				return;
      }

      // Fill out the response to be sent to the client.
      res.status = StatusCode::ok;
      char buffer[512];
      while (stream.read(buffer, sizeof(buffer)).gcount() > 0) {
				res.content.append(buffer, stream.gcount());
      }
      res.headers.resize(2);
      res.headers[0].name = "Content-Length";
      res.headers[0].value = std::to_string(res.content.size());
      res.headers[1].name = "Content-Type";
      res.headers[1].value = mime_types::extension_to_type(extension);
    }

    bool RequestHandler::url_decode(const std::string& in, std::string& out) {
      out.clear();
      out.reserve(in.size());
      for (std::size_t i = 0; i < in.size(); ++i) {
				if (in[i] == '%') {
					if (i + 3 <= in.size()) {
						int value = 0;
						std::istringstream is(in.substr(i + 1, 2));

						if (is >> std::hex >> value) {
							out += static_cast<char>(value);
							i +=2;
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else if (in[i] == '+') {
					out += ' ';
				} else {
					out += in[i];
				}
      }

      return true;
    }

  } // namespace server
} // namespace http
