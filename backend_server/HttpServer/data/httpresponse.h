#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QObject>
#include <QHash>

namespace BlockchainServer {
    class HttpResponse : public QObject
    {
        Q_OBJECT

        const static QString HEADER_CONTENT_TYPE;
        const static QString HEADER_CONNECTION;
        const static QString HEADER_PRAGMA;
        const static QString HEADER_ACCESS_CONTROL_ALLOW_ORIGIN;

        const static QString HVALUE_STATUS_200_OK;
        const static QString HVALUE_HTTP_1_1;
        const static QString HVALUE_CONTENT_TYPE_JSON;
        const static QString HVALUE_CONTENT_TYPE_HTML;
        const static QString HVALUE_ACCESS_CONTROL_ALLOW_ORIGIN;
        const static QString HVALUE_PRAGMA;
        const static QString HVALUE_CONNECTION;
    public:
        HttpResponse(QObject *parent = nullptr);
        ~HttpResponse();
        void setHeaderValueByName(QString header_name, QString header_value);
        void setHttpVersion(QString version);
        void setHttpStatus(QString status);
        void appendData(QString _data);
        void appendData(char *in, int len);
        void clearData();
        QString* getResponseData();

    private:
        QHash<QString, QString> *headers;
        QString *data;
        QString *user_data;
        QString *http_version;
        QString *status_code;
    };
}

#endif // HTTPRESPONSE_H
