#pragma once

#include "Config.hpp"
#include "ErrorResponse.hpp"
#include "Request.hpp"

class Response {
public:
  Response();
  ~Response();

  std::string setResponse(Request _request);
  void checkValidity();
  void executeMethod();

  void setEnvp(char **envp);
  void setConfig(Config conf);
  char **getEnvp() const;
  std::string getPath(char **envp, std::string cmd);

  void GET_HEAD();
  void POST();
  void DELETE();
  void PUT();
  void OPTIONS();
  void TRACE();

  void setResponseHeader(std::string key, std::string value);
  void setResponseBody(std::string body);
  void setResponse(std::string response);
  int getResponseFile() const;
  std::map<std::string, std::string> getResponseHeader() const;
  std::string getResponseBody() const;
  std::string getResponse() const;

private:
  Response(const Response &copy);
  Response operator=(const Response &copy);

  char **_envp;
  Config _config;
  int _responseFile;
  Request _request;
  std::map<std::string, std::string> _responseHeader;
  std::string _responseBody;
  std::string _response;
  errorResponse _errorResponse;
};
