#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "config.h"
#include <QHash>
#include <QObject>


namespace BlockchainServer {
    class HttpRequest : public QObject
    {
        Q_OBJECT
    enum REQUEST_METHOD { METHOD_HTTP_GET, METHOD_HTTP_POST, METHOD_INVALID };
    const static QString HEADER_ACCEPT_ENCODING;
    const static QString HEADER_CONNECTION;
    const static QString HEADER_ACCEPT;
    const static QString HEADER_ACCEPT_LANGUAGE;
    const static QString HEADER_USER_AGENT;
    const static QString HEADER_DEFAULT_VALUE;
    const static QString HEADER_HOST;
    const static QString HEADER_ORIGIN;
    const static QString HEADER_REFERER;
    const static QString HEADER_VALUE_DEFAULT;
    const static QString PARAM_VALUE_DEFAULT;

    public:
        explicit HttpRequest(QObject *parent = nullptr, char *data = nullptr, int len = 0);
        ~HttpRequest();
        bool isParseSuccessful() const;
        REQUEST_METHOD getRequestMethod() const;
        QString getHeaderValue(QString headerKey) const;
        void getParamByKey(QString paramKey, QString* output) const;
        QString* getURL() const;
        bool matchesURL(const QString *url_str) const;
        void getParamData(QByteArray &out);
        int getIndex();

    private:
        REQUEST_METHOD *method;
        QString *request_url;
        QHash<QString, QString> *params;
        QByteArray *url;
        QHash<QString, QString> *headers;
        bool *parseStatus;
        int getURLLength(char *data, int len);

    signals:

    public slots:
    };
}

#endif // HTTPREQUEST_H
