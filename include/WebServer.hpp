#pragma once

#include "Config.hpp"
#include "Convert.hpp"
#include "IServer.hpp"
#include "Protocol.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <map>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

class WebServer : public IServer {

public:
  WebServer(Config conf, Protocol prot, char **envp);
  ~WebServer();

  int getListenSocket() const;
  Config getConfig() const;
  Protocol getProtocol() const;
  //   struct kevent setEvent(uintptr_t ident, int16_t filter, uint16_t flags,
  //                          uint32_t fflags, intptr_t data, void *udata);

private:
  Config _config;
  Protocol _protocol;
  char **_envp;
  int _listenSocket;
  struct sockaddr_in _serverAddress;
  std::map<int, std::string> _clientSocket;
};