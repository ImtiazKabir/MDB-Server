- client connects => store in `name_to_socket_map<string, socket>`
- client sends `LoginInfo` JSON => parse and authenticate
- server sends `SessionInfo` JSON
- client sends `TransactionInfo` JSON => parse and create CreateInfo
- server sends `CreateInfo` JSON
- client sends `CreateInfo` JSON => add movie to Database