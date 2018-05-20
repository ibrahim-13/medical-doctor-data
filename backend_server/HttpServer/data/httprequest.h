/**
Copyright (C) 2017 MD. Ibrahim Khan

Project Name: 
Author: MD. Ibrahim Khan
Author's Email: ib.arshad777@gmail.com

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of the contributors may
   be used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTIONS) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
**/

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
