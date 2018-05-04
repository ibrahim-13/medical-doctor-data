#include "httpresponse.h"

BlockchainServer::HttpResponse::HttpResponse(QObject *parent) : QObject(parent)
{
    headers = new QHash<QString, QString>();
    data = new QString();
    user_data = new QString();

    http_version = new QString();
    http_version->append(HVALUE_HTTP_1_1);
    status_code = new QString();
    status_code->append(HVALUE_STATUS_200_OK);

    headers->insert(HEADER_CONNECTION, HVALUE_CONNECTION);
    headers->insert(HEADER_CONTENT_TYPE, HVALUE_CONTENT_TYPE_HTML);
    headers->insert(HEADER_PRAGMA, HVALUE_PRAGMA);
}

BlockchainServer::HttpResponse::~HttpResponse()
{
    delete headers;
    delete data;
    delete user_data;
    delete http_version;
    delete status_code;
}

void BlockchainServer::HttpResponse::setHttpVersion(QString version)
{
    http_version->clear();
    http_version->append(version);
}

void BlockchainServer::HttpResponse::setHttpStatus(QString status)
{
    status_code->clear();
    status_code->append(status);
}

void BlockchainServer::HttpResponse::setHeaderValueByName(QString header_name, QString header_value)
{
    headers->insert(header_name, header_value);
}

void BlockchainServer::HttpResponse::appendData(QString _data)
{
    user_data->append(_data);
}

void BlockchainServer::HttpResponse::appendData(char *in, int len)
{
    for(int i = 0; i < len; i++)
    {
        user_data->append(QChar(*(in + i)));
    }
}

void BlockchainServer::HttpResponse::clearData()
{
    user_data->clear();
}

QString* BlockchainServer::HttpResponse::getResponseData()
{
    data->clear();
    data->append(*http_version + " " + *status_code + "\r\n");

    QHash<QString, QString>::iterator it;
    for(it = headers->begin(); it != headers->end(); it++)
    {
        data->append(it.key() + ": " + it.value() + "\r\n");
    }

    data->append("\r\n");
    data->append(*user_data);

    return data;
}

const QString BlockchainServer::HttpResponse::HEADER_ACCESS_CONTROL_ALLOW_ORIGIN = "Access-Control-Allow-Origin";
const QString BlockchainServer::HttpResponse::HEADER_CONNECTION = "Connection";
const QString BlockchainServer::HttpResponse::HEADER_CONTENT_TYPE = "Content-Type";
const QString BlockchainServer::HttpResponse::HEADER_PRAGMA = "Pragma";

const QString BlockchainServer::HttpResponse::HVALUE_ACCESS_CONTROL_ALLOW_ORIGIN = "*";
const QString BlockchainServer::HttpResponse::HVALUE_CONNECTION = "close";
const QString BlockchainServer::HttpResponse::HVALUE_CONTENT_TYPE_HTML = "text/html";
const QString BlockchainServer::HttpResponse::HVALUE_CONTENT_TYPE_JSON = "application/json";
const QString BlockchainServer::HttpResponse::HVALUE_HTTP_1_1 = "HTTP/1.1";
const QString BlockchainServer::HttpResponse::HVALUE_PRAGMA = "no-cache";
const QString BlockchainServer::HttpResponse::HVALUE_STATUS_200_OK = "200 OK";
