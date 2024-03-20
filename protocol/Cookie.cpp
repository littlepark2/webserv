#include "../include/Cookie.hpp"

Cookie::Cookie()
{

}

Cookie::~Cookie()
{

}

Cookie::Cookie(const Cookie &copy)
{
    this->_reqCookieHeader = copy._reqCookieHeader;
    this->_resCookieHeader = copy._resCookieHeader;
    this->_queryString = copy._queryString;
    this->_queryStringExistance = copy._queryStringExistance;
    this->_resCookieHeaderString = copy._resCookieHeaderString;
    this->_resBody = copy._resBody;
}

Cookie Cookie::operator=(const Cookie &copy)
{
    this->_reqCookieHeader = copy._reqCookieHeader;
    this->_resCookieHeader = copy._resCookieHeader;
    this->_queryString = copy._queryString;
    this->_queryStringExistance = copy._queryStringExistance;
    this->_resCookieHeaderString = copy._resCookieHeaderString;
    this->_resBody = copy._resBody;
    return (*this);
}

time_t Cookie::getCookieTime(int day, int hour, int min)
{
    time_t  rawtime;
    time(&rawtime);

    rawtime = rawtime + (day * 24 * 60 * 60) + (hour * 60 * 60) + (min * 60);
    return (rawtime);
}

std::string Cookie::convertIntoRealTime(time_t rawtime)
{
    struct tm   *timeinfo = gmtime(&rawtime);

    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    std::string cookieTime(buffer);
    return (cookieTime);
}

void Cookie::controlCookies(std::map<std::string, std::string> header, std::string URI)
{
    std::map<std::string, std::string>::iterator itrHeader = header.find("Cookie");
    
    parseURI(URI);
    if (_queryStringExistance == 1) //쿼리스트링이 존재함
    {
        _resCookieHeaderString.clear();
        _resCookieHeaderString = "color=" + _queryString["color"] + "; \r\n";
        _resCookieHeaderString += "Set-Cookie: size=" + _queryString["size"] + "; ";
        // _resCookieHeaderString += "Expires=" + convertIntoRealTime(getCookieTime(0, 1, 0)) + ";";
        makeBody(_queryString["color"], _queryString["size"]);
        std::cout << "query string exists, color is:" << _queryString["color"] << ", and size is:" << _queryString["size"] << std::endl;
    }
    else if (itrHeader == header.end()) //쿼리스트링과 쿠키 다 없음
    {
        _resCookieHeaderString.clear();
        _resCookieHeaderString = "color=000000; \r\nSet-Cookie: size=15;";
        makeBody("000000", "15");
        std::cout << "query string and cookie does not exist, color is:" << "000000" << ", and size is:" << "15" << std::endl;
    }
    else //쿼리스트링은 없지만 쿠키헤더값은 있는 경우
    {
        _resCookieHeaderString.clear();
        setCookieHeader(itrHeader->second);
        // if (_reqCookieHeader.find("size") == _reqCookieHeader.end())
        //     _reqCookieHeader.insert(std::make_pair("size", "15"));
        _resCookieHeaderString = "color=" + _reqCookieHeader["color"] + "; \r\nSet-Cookie: size=" + _reqCookieHeader["size"] + "; ";
        makeBody(_reqCookieHeader["color"], _reqCookieHeader["size"]);
        std::cout << "query string does not, but cookie header exists, color is:" << _reqCookieHeader["color"] << ", and size is:" << _reqCookieHeader["size"] << std::endl;
    }
}

void    Cookie::setCookieHeader(std::string rawCookie)
{
    size_t index = 0;
    size_t pos = 0;
    std::string tmp;

    while (1)
    {
        index = rawCookie.find(";", pos);
        tmp = rawCookie.substr(pos, index - pos);
        std::string key = splitBefore(tmp, "=");
        std::string value = splitAfter(tmp, "=");
        this->_reqCookieHeader[key] = value;
        if (index == std::string::npos)
            break;
        pos = index + 2;
    }
}

void    Cookie::parseURI(std::string URI)
{
    if (URI.find("cookie?color=") == std::string::npos && URI.find("cookie.html?color=") == std::string::npos)
        _queryStringExistance = 0;
    else
    {
        _queryStringExistance = 1;
        std::string query = splitAfter(URI, "color=%23");
        std::string colorString = splitBefore(query, "&size=");
        std::string sizeString = splitAfter(query, "&size=");
        _queryString["color"] = colorString;
        _queryString["size"] = sizeString;
    }
}

void    Cookie::makeBody(std::string color, std::string size)
{
    int fd = open("document/html/cookie.html", O_RDONLY);
    std::string cookieHTML;
    char buf[BUFFER_SIZE + 1];
    
    memset(buf, 0, BUFFER_SIZE + 1);
    while (read(fd, buf, BUFFER_SIZE) > 0)
    {
        std::string tmp(buf);
        cookieHTML += tmp;
        memset(buf, 0, BUFFER_SIZE + 1);
    }
    std::vector<std::string> rgbValue = rgbToDecimal(color);
    _resBody = splitBefore(cookieHTML, ".cookie-text") + "  .cookie-text";
    _resBody += " {\n    color: rgb(" + rgbValue[0] + ", ";
    _resBody += rgbValue[1] + ", " + rgbValue[2] + ");";
    _resBody += "\n    font-size: " + size + "px;\n  }";
    _resBody += "\n</style>\n</head>\n<body>\n\n" + splitAfter(cookieHTML, "</style>\n</head>\n<body>");
}

std::string Cookie::getresBody() const
{
    return (_resBody);
}


std::string Cookie::getresCookieHeaderString() const
{
    if (_resCookieHeaderString.size() == 0)
        return ("");
    else
        return (_resCookieHeaderString + "\r\n");
}