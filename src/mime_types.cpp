#include "mime_types.h"

namespace http {
  namespace server {
    namespace mime_types {

      struct  ExtensionMapping {
				const char* extension;
				const char* mime_type;
      };

      ExtensionMapping mappings[] = {
				{"gif", "image/gif"},
				{"htm", "text/html"},
				{"html", "text/html"},
				{"jpg", "image/jpeg"},
				{"png", "image/png"}
      };

      std::string extension_to_type(const std::string& extension) {
				for (ExtensionMapping mapping : mappings) {
					if (mapping.extension == extension) {
						return mapping.mime_type;
					}
				}
				return "text/plain";
      }

    } // namespace mime_types
  } // namespace server
} // namespace http
