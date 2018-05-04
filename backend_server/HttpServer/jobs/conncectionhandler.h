#ifndef CONNCECTIONHANDLER_H
#define CONNCECTIONHANDLER_H

#include "blockchain/config.h"
#include "blockchain/blockchain.h"
#include "HttpServer/data/handler.h"

#include <QObject>
#include <QHash>
#include <QRunnable>
#include <QTcpSocket>

namespace BlockchainServer {
    class ConncectionHandler : public QObject, public QRunnable
    {

    public:
        explicit ConncectionHandler(qintptr socketHandle = 0, QHash<QString, BlockchainServer::HandlerCallBack> *handlerList = nullptr);
        ~ConncectionHandler();
        void run();

    private:
        qintptr SocketHandle;
        Blockchain::App *blockchainApp;
        QHash<QString, BlockchainServer::HandlerCallBack> *handlerList;
    };
}

#endif // CONNCECTIONHANDLER_H
