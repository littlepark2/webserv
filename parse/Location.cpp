#include "../include/Location.hpp"

Location::Location(void)
    : _autoindex(false), _clientBodyMax(1024 * 1024),
      _clientHeaderMax(32 * 1024) {}

Location::~Location(void) {}

Location::Location(const Location &object) { *this = object; }

Location &Location::operator=(const Location &object) {
  if (this == &object)
    return (*this);
  ConfigBase::operator=(object);
  _locationFunctions = object._locationFunctions;
  _parameters = object._parameters;
  _parameterCount = object._parameterCount;
  _autoindex = object._autoindex;
  _clientBodyMax = object._clientBodyMax;
  _clientHeaderMax = object._clientHeaderMax;
  _rootDirectory = object._rootDirectory;
  _return = object._return;
  _indexes = object._indexes;
  _errorPages = object._errorPages;
  _acceptedMethods = object._acceptedMethods;
  return (*this);
}

Location::Location(const std::string &locationText, const Location &location)
    : ConfigBase(locationText, _getDirectiveSet()) {
  _autoindex = location._autoindex;
  _clientBodyMax = location._clientBodyMax;
  _clientHeaderMax = location._clientHeaderMax;
  _rootDirectory = location._rootDirectory;
  _return = location._return;
  _indexes = location._indexes;
  _errorPages = location._errorPages;
  _setFunctionPTRMap();
  _setConfigData();
}

Location::Location(const std::string &configText,
                   const string_set &directiveSet)
    : ConfigBase(configText, directiveSet), _autoindex(false),
      _clientBodyMax(1024 * 1024), _clientHeaderMax(32 * 1024) {
  _setFunctionPTRMap();
}

const std::string &Location::getRootDirectory(void) const {
  return (_rootDirectory);
}

const std::string &Location::getReturn(void) const { return (_return); }

const size_t &Location::getClientBodyMax(void) const {
  return (_clientBodyMax);
}

const size_t &Location::getClientHeaderMax(void) const {
  return (_clientHeaderMax);
}

const bool &Location::getAutoIndex(void) const { return (_autoindex); }

const std::vector<std::string> &Location::getIndexes(void) const {
  return (_indexes);
}

bool Location::isIndexExist(const std::string &path) const {
  if (std::find(_indexes.begin(), _indexes.end(), path) != _indexes.end())
    return (true);
  else
    return (false);
}

const error_page_map &Location::getErrorPageMap(void) const {
  return (_errorPages);
}

std::string Location::getErrorPage(const int &errorPageNumber) const {
  std::map<int, std::string>::const_iterator iterator;

  iterator = _errorPages.find(errorPageNumber);
  if (iterator == _errorPages.end())
    return ("");
  else
    return (iterator->second);
}

bool Location::isAcceptedHTTPMethod(const std::string &HTTPMethod) {
  string_vector::iterator start = _acceptedMethods.begin();
  string_vector::iterator end = _acceptedMethods.end();

  if (_acceptedMethods.size() == 0)
    return (true);
  if (std::find(start, end, HTTPMethod) != end)
    return (true);
  else
    return (false);
}

const string_vector &Location::getAcceptedMethods(void) const {
  return (_acceptedMethods);
}

std::set<std::string> Location::_getDirectiveSet(void) const {
  std::set<std::string> directiveSet;

  directiveSet.insert(ROOT_DIRECTIVE);
  directiveSet.insert(INDEX_DIRECTIVE);
  directiveSet.insert(AUTOINDEX_DIRECTIVE);
  directiveSet.insert(ERROR_PAGE_DIRECTIVE);
  directiveSet.insert(REDIRECTION_DIRECTIVE);
  directiveSet.insert(CLIENT_MAX_DIRECTIVE);
  directiveSet.insert(HEADER_MAX_DIRECTIVE);
  directiveSet.insert(LIMIT_EXCEPT_DIRECTIVE);
  return (directiveSet);
}

void Location::_setFunctionPTRMap(void) {
  _locationFunctions[ROOT_DIRECTIVE] = &Location::_setRootDirectory;
  _locationFunctions[INDEX_DIRECTIVE] = &Location::_setIndexes;
  _locationFunctions[AUTOINDEX_DIRECTIVE] = &Location::_setAutoindex;
  _locationFunctions[ERROR_PAGE_DIRECTIVE] = &Location::_setErrorPages;
  _locationFunctions[REDIRECTION_DIRECTIVE] = &Location::_setReturn;
  _locationFunctions[CLIENT_MAX_DIRECTIVE] = &Location::_setClientBodyMax;
  _locationFunctions[HEADER_MAX_DIRECTIVE] = &Location::_setClientHeaderMax;
  _locationFunctions[LIMIT_EXCEPT_DIRECTIVE] = &Location::_setAcceptedMethods;
}

void Location::_setDirectiveData(void) {
  _setParameters();
  (this->*_locationFunctions[_configParser.getDirective()])();
}

void Location::_setParameters(void) {
  _parameters = _configParser.getParameters();
  _parameterCount = _parameters.size();
}

void Location::_setRootDirectory(void) { _rootDirectory = _parameters[0]; }

void Location::_setIndexes(void) {
  size_t i;

  if (_indexes.empty() == false)
    _indexes.clear();
  for (i = 0; i < _parameterCount; i++)
    _indexes.push_back(_parameters[i]);
}

void Location::_setAutoindex(void) {
  if (_parameters[0] == "on")
    _autoindex = true;
  else
    _autoindex = false;
}

void Location::_setErrorPages(void) {
  _errorPages[std::atoi(_parameters[0].c_str())] = _parameters[1];
}

void Location::_setReturn(void) { _return = _parameters[0]; }

void Location::_setClientBodyMax(void) {
  std::string sizeString = _parameters[0];
  size_t number = ConfigUtil::convertToSizeT(sizeString);
  size_t unit = ConfigUtil::convertByteUnit(sizeString[sizeString.size() - 1]);

  _clientBodyMax = number * unit;
}

void Location::_setClientHeaderMax(void) {
  std::string sizeString = _parameters[0];
  size_t number = ConfigUtil::convertToSizeT(sizeString);
  size_t unit = ConfigUtil::convertByteUnit(sizeString[sizeString.size() - 1]);

  _clientHeaderMax = number * unit;
}

void Location::_setAcceptedMethods(void) { _acceptedMethods = _parameters; }