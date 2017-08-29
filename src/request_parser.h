#pragma once
#include <tuple>

#include "header.h"
#include "request.h"

namespace http {
  namespace server {
    struct request;

    // Result of parse.
    enum class ParseResult { good, bad, indeterminate };

    // Parse states.
    enum class ParseState {
      method_start,
      method,
      uri,
      http_version_h,
      http_version_t_1,
      http_version_t_2,
      http_version_p,
      http_version_slash,
      http_version_major_start,
      http_version_major,
      http_version_minor_start,
      http_version_minor,
      expecting_newline_1,
      header_line_start,
      header_lws,
      header_name,
      space_before_header_value,
      header_value,
      expecting_newline_2,
      expecting_newline_3
    };

    // Request parser.
    class RequestParser {
    public:
      // Construct ready to parse the request method.
      RequestParser();

      // Reset to initial parser state.
      void reset();

      // Parse some data. The enum return value is good when a complete request has
      // been parsed, bad if the data is invalid, indeterminate when more data is
      // required. The InputIterator return value indicates how much of the input
      // has been consumed.
      // template<typename InputIterator>
      std::tuple<ParseResult, char*> parse(Request& request,
																					 char* begin,
																					 char* end);

    private:
      // Handle the next charater input.
      ParseResult consume(Request& request, char input);

      // Check if a byte is an HTTP character.
      static bool is_char(int c);

      // Check if a byty is an HTTP control character.
      static bool is_ctl(int c);

      // Check if a byte is defined as an HTTP tspecial character.
      static bool is_tspecial(int c);

      // Check if a byte is a digit.
      static bool is_digit(int c);

      // Current state of the parser.
      ParseState state_;
    };

  } // namespace server
} // namespace http
