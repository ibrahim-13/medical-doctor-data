#include "httprequest.h"

#include <string>
#include <QString>
#include <QVector>
#include <QStringRef>
#include <QUrl>

BlockchainServer::HttpRequest::HttpRequest(QObject *parent, char *data, int len) : QObject(parent)
{
   method = new REQUEST_METHOD;
   url = new QByteArray();
   request_url = new QString();
   params = new QHash<QString, QString>();
   headers = new QHash<QString, QString>();
   parseStatus = new bool;

   // Set default values. Values will be changed when parse is successful
   *method = METHOD_INVALID;
   *parseStatus = false;

   // Stop parseing if the header length does not meet the minimum requirement
   if(len < HTTP_HEADER_MIN_LEN)
   {
       return;
   }

   QString *requestRawData = new QString(data);
   QVector<QStringRef> splitRawData = requestRawData->splitRef("\r\n");
   QString *tmp = new QString();

   // Stop parsing if the request line is not present
   if(splitRawData.size() < 1)
   {
       return;
   }

   QVector<QStringRef> splitRequestData = splitRawData[0].split(" ");

   // Stop parsing if request in the header is malformed
   if(splitRequestData.size() != 3)
   {
       return;
   }

   // Separate REQUEST METHOD
   tmp->clear();
   splitRequestData[0].appendTo(tmp);
   QString tmp2 = tmp->trimmed();
   if(QString::compare(tmp2, "GET") == 0)
   {
       *method = METHOD_HTTP_GET;
   }
   else if(QString::compare(tmp2, "POST") == 0)
   {
       *method = METHOD_HTTP_POST;
   }
   else
   {
       return;
   }

   // Separate URL
   tmp->clear();
   QVector<QStringRef> splitURL = splitRequestData[1].split("?");
   if(splitURL.size() > 0)
   {
       splitURL[0].appendTo(request_url);
   }

   for(int i = 1; i < splitURL.size(); i++)
   {
       QVector<QStringRef> splitParam = splitURL[i].split("=");
       if(splitParam.size() == 2)
       {
           QString paramKey;
           QString paramValue;
           splitParam[0].appendTo(&paramKey);
           splitParam[1].appendTo(&paramValue);
           paramKey = paramKey.trimmed();
           paramKey.squeeze();
           paramValue = paramValue.trimmed();
           paramValue.squeeze();
           params->insert(paramKey, paramValue);
       }
   }

   // Separate headers
   for(int i = 1; i < splitRawData.size(); i++)
   {
       QVector<QStringRef> nestedSplit = splitRawData[i].split(":");
       if(nestedSplit.size() > 1)
       {
           QString headerName;
           QString headerValue;
           nestedSplit[0].appendTo(&headerName);
           nestedSplit[1].appendTo(&headerValue);
           headerName = headerName.trimmed();
           headerName.squeeze();
           for(int j = 2; j < nestedSplit.size(); j++)
           {
               headerValue.append(":");
               nestedSplit[j].appendTo(&headerValue);
           }
           headerValue = headerValue.trimmed();
           headerValue.squeeze();
           headers->insert(headerName, headerValue);
       }
   }

   // Parse URL from header
   int url_start;
   if(*method == METHOD_HTTP_GET)
       url_start = 4;
   else
       url_start = 5;

   int url_len = getURLLength(data, len);
   for(int i = url_start; i <= url_len; i++)
       url->append(*(data + i));

   *parseStatus = true;

   // Cleanup
   delete requestRawData;
   delete tmp;
}

BlockchainServer::HttpRequest::~HttpRequest()
{
    delete method;
    delete parseStatus;
    delete url;
    delete request_url;
    delete params;
    delete headers;
}

bool BlockchainServer::HttpRequest::isParseSuccessful() const
{
    return parseStatus;
}

BlockchainServer::HttpRequest::REQUEST_METHOD BlockchainServer::HttpRequest::getRequestMethod() const
{
    return *method;
}

QString BlockchainServer::HttpRequest::getHeaderValue(QString headerKey) const
{
    return headers->value(headerKey);
}

void BlockchainServer::HttpRequest::getParamByKey(QString paramKey, QString* output) const
{
    if(params->contains(paramKey))
        output->append(QByteArray::fromBase64(QUrl::fromPercentEncoding(params->value(paramKey).toUtf8()).toUtf8()));
}

QString* BlockchainServer::HttpRequest::getURL() const
{
    return request_url;
}

bool BlockchainServer::HttpRequest::matchesURL(const QString *url_str) const
{
    if(url_str->length() > url->length())
        return false;

    QString *tmp = new QString(*url);
    int comp = tmp->left(url_str->length()).compare(*url_str);
    delete tmp;
    return comp == 0 ? true : false;
}

int BlockchainServer::HttpRequest::getURLLength(char *data, int len)
{
    for(int i = 0; i < (len - 5); i++)
    {
        if(*(data + i) == 'H')
            if(*(data + i + 1) == 'T')
                if(*(data + i + 2) == 'T')
                    if(*(data + i + 3) == 'P')
                        if(*(data + i + 4) == '/')
                            return (i - 2);
    }
    return -1;
}

const QString BlockchainServer::HttpRequest::HEADER_ACCEPT_ENCODING = "Accept-Encoding";
const QString BlockchainServer::HttpRequest::HEADER_CONNECTION = "Connection";
const QString BlockchainServer::HttpRequest::HEADER_ACCEPT = "Accept";
const QString BlockchainServer::HttpRequest::HEADER_ACCEPT_LANGUAGE = "Accept-Language";
const QString BlockchainServer::HttpRequest::HEADER_USER_AGENT = "User-Agent";
const QString BlockchainServer::HttpRequest::HEADER_DEFAULT_VALUE = "default_value";
const QString BlockchainServer::HttpRequest::HEADER_HOST = "Host";
const QString BlockchainServer::HttpRequest::HEADER_ORIGIN = "Origin";
const QString BlockchainServer::HttpRequest::HEADER_REFERER = "Referer";
const QString BlockchainServer::HttpRequest::HEADER_VALUE_DEFAULT = HTTP_HEADER_DEFAULT_VAL;
const QString BlockchainServer::HttpRequest::PARAM_VALUE_DEFAULT = HTTP_PARAM_DEFAULT_VAL;
