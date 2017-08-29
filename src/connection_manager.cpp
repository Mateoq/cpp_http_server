#include "connection_manager.h"

namespace http {
  namespace server {
    ConnectionManager::ConnectionManager() {

    }

    void ConnectionManager::start(ConnectionPtr connection) {
      connections_.insert(connection);
      connection->start();
    }

    void ConnectionManager::stop(ConnectionPtr connection) {
      connections_.erase(connection);
      connection->stop();
    }

    void ConnectionManager::stop_all() {
      for (auto connection : connections_) {
				connection->stop();
      }
      connections_.clear();
    }

  } // namespace server
} // namespace http
