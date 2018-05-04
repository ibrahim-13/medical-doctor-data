#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "blockchain/blockchain.h"
#include "HttpServer/data/handler.h"

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QIODevice>
#include <QHash>
#include <QDebug>
#include <QThreadPool>
#include <QMutex>

namespace BlockchainServer {
    class HttpServer : public QTcpServer
    {
        Q_OBJECT
    public:
        explicit HttpServer(QObject *parent = nullptr);
        ~HttpServer();
        void addHandler(QString location, BlockchainServer::HandlerCallBack handler);

    signals:

    public slots:

    private:
        QHash<QString, BlockchainServer::HandlerCallBack> *handlers;
        QThreadPool *threadPool;
        Blockchain::App *globalBlockchainApp;
        void incomingConnection(qintptr handle);
    };
}

#endif // HTTPSERVER_H
