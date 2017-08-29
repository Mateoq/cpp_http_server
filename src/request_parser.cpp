#include "request_parser.h"

namespace http {
  namespace server {
    RequestParser::RequestParser() : state_(ParseState::method_start) {

    }

    // template<typename InputIterator>
    std::tuple<ParseResult, char*> RequestParser::parse(Request& request,
																												char* begin,
																												char* end) {
      while (begin != end) {
				ParseResult result = consume(request, *begin++);
				if (result == ParseResult::good || result == ParseResult::bad) {
					return std::make_tuple(result, begin);
				}
      }
      return std::make_tuple(ParseResult::indeterminate, begin);
    }

    void RequestParser::reset() {
      state_ = ParseState::method_start;
    }

    ParseResult RequestParser::consume(Request& request, char input) {
      switch (state_) {
      case ParseState::method_start:
				if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
					return ParseResult::bad;
				} else {
					state_ = ParseState::method;
					request.method.push_back(input);
					return ParseResult::indeterminate;
				}
      case ParseState::method:
				if (input == ' ') {
					state_ = ParseState::uri;
					return ParseResult::indeterminate;
				} else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
					return ParseResult::bad;
				} else {
					request.method.push_back(input);
					return ParseResult::indeterminate;
				}
      case ParseState::uri:
				if (input == ' ') {
					state_ = ParseState::http_version_h;
					return ParseResult::indeterminate;
				} else if (is_ctl(input)) {
					return ParseResult::bad;
				} else {
					request.uri.push_back(input);
					return ParseResult::indeterminate;
				}
      case ParseState::http_version_h:
				if (input == 'H') {
					state_ = ParseState::http_version_t_1;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::http_version_t_1:
				if (input == 'T') {
					state_ = ParseState::http_version_t_2;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::http_version_t_2:
				if (input == 'T') {
					state_ = ParseState::http_version_p;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::http_version_p:
				if (input == 'P') {
					state_ = ParseState::http_version_slash;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::http_version_slash:
				if (input == '/') {
					request.http_version_major = 0;
					request.http_version_minor = 0;
					state_ = ParseState::http_version_major_start;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::http_version_major_start:
				if (is_digit(input)) {
					request.http_version_major = request.http_version_major * 10 + input - '0';
					state_ = ParseState::http_version_major;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::http_version_major:
				if (input == '.') {
					state_ = ParseState::http_version_minor_start;
					return ParseResult::indeterminate;
				} else if (is_digit(input)) {
					request.http_version_major = request.http_version_major * 10 + input - '0';
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::http_version_minor_start:
				if (is_digit(input)) {
					request.http_version_minor = request.http_version_minor * 10 + input - '0';
					state_ = ParseState::http_version_minor;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::http_version_minor:
				if (input == '\r') {
					state_ = ParseState::expecting_newline_1;
					return ParseResult::indeterminate;
				} else if (is_digit(input)) {
					request.http_version_minor = request.http_version_minor * 10 + input - '0';
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::expecting_newline_1:
				if (input == '\n') {
					state_ = ParseState::header_line_start;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::header_line_start:
				if (input == '\r') {
					state_ = ParseState::expecting_newline_3;
					return ParseResult::indeterminate;
				} else if (!request.headers.empty() && (input == ' ' || input == '\t')) {
					state_ = ParseState::header_lws;
					return ParseResult::indeterminate;
				} else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
					return ParseResult::bad;
				} else {
					request.headers.push_back(Header());
					request.headers.back().name.push_back(input);
					state_ = ParseState::header_name;
					return ParseResult::indeterminate;
				}
      case ParseState::header_lws:
				if (input == '\r') {
					state_ = ParseState::expecting_newline_2;
					return ParseResult::indeterminate;
				} else if (input == ' ' || input == '\t') {
					return ParseResult::indeterminate;
				} else if (is_ctl(input)) {
					return ParseResult::bad;
				} else {
					request.headers.back().value.push_back(input);
					state_ = ParseState::header_value;
					return ParseResult::indeterminate;
				}
      case ParseState::header_name:
				if (input == ':') {
					state_ = ParseState::space_before_header_value;
					return ParseResult::indeterminate;
				} else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
					return ParseResult::bad;
				} else {
					request.headers.back().name.push_back(input);
					return ParseResult::indeterminate;
				}
      case ParseState::space_before_header_value:
				if (input == ' ') {
					state_ = ParseState::header_value;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::header_value:
				if (input == '\r') {
					state_ = ParseState::expecting_newline_2;
					return ParseResult::indeterminate;
				} else if (is_ctl(input)) {
					return ParseResult::bad;
				} else {
					request.headers.back().value.push_back(input);
					return ParseResult::indeterminate;
				}
      case ParseState::expecting_newline_2:
				if (input == '\n') {
					state_ = ParseState::header_line_start;
					return ParseResult::indeterminate;
				} else {
					return ParseResult::bad;
				}
      case ParseState::expecting_newline_3:
				return (input == '\n') ? ParseResult::good : ParseResult::bad;
      default:
				return ParseResult::bad;
      }
    }

    bool RequestParser::is_char(int c) {
      return (c >= 0 && c <= 127);
    }

    bool RequestParser::is_ctl(int c) {
      return ((c >= 0 && c <= 31) || (c == 127));
    }

    bool RequestParser::is_tspecial(int c) {
      switch(c) {
      case '(':
      case ')':
      case '<':
      case '>':
      case '@':
      case ',':
      case ';':
      case ':':
      case '\\':
      case '"':
      case '/':
      case '[':
      case ']':
      case '?':
      case '=':
      case '{':
      case '}':
      case ' ':
      case '\t':
				return true;
      default:
				return false;
      }
    }

    bool RequestParser::is_digit(int c) {
      return (c >= '0' && c <= '9');
    }
  } // namespace server
} // namespace http
