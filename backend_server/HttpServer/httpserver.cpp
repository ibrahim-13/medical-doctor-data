#include "httpserver.h"
#include "jobs/conncectionhandler.h"

BlockchainServer::HttpServer::HttpServer(QObject *parent) : QTcpServer(parent)
{
    threadPool = new QThreadPool(this);
    handlers = new QHash<QString, BlockchainServer::HandlerCallBack>();
    //globalBlockchainApp = new Blockchain::App;
    globalBlockchainApp = Blockchain::App::getDefaultInstance();
    threadPool->setMaxThreadCount(10);
    if(listen(QHostAddress::Any, 8080))
        qDebug() << "Server running on port 8080";
    else
        qDebug() << "Could not start server on port 8080";
}

BlockchainServer::HttpServer::~HttpServer()
{
    threadPool->deleteLater();
    globalBlockchainApp->deleteLater();
    delete handlers;
}

void BlockchainServer::HttpServer::addHandler(QString location, BlockchainServer::HandlerCallBack handler)
{
    handlers->insert(location, handler);
}

void BlockchainServer::HttpServer::incomingConnection(qintptr sockHandle)
{
    BlockchainServer::ConncectionHandler *connHandler = new BlockchainServer::ConncectionHandler(sockHandle, handlers);
    threadPool->start(connHandler);
}
