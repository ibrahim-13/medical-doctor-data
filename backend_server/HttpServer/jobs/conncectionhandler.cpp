#include "conncectionhandler.h"
#include "HttpServer/data/httprequest.h"
#include "HttpServer/data/httpresponse.h"
#include <QUrl>

BlockchainServer::ConncectionHandler::ConncectionHandler(qintptr socketHandle, QHash<QString, BlockchainServer::HandlerCallBack> *handlerList)
{
    this->SocketHandle = socketHandle;
    this->blockchainApp = Blockchain::App::getDefaultInstance();
    this->handlerList = handlerList;
}

BlockchainServer::ConncectionHandler::~ConncectionHandler()
{
}

void BlockchainServer::ConncectionHandler::run()
{
    QTcpSocket socket;
    socket.setSocketDescriptor(SocketHandle);
    socket.waitForReadyRead();
    char webBrowerRXData[1000];
    int sv = socket.read(webBrowerRXData,1000);

//    QString url_path_add("/api/add");
//    QString url_path_verify("/api/verify");
//    QString url_path_blockchain_index("/api/blockchain/index");
//    QString url_path_blockchain_data("/api/blockchain/data");
    BlockchainServer::HttpRequest *request = new BlockchainServer::HttpRequest(nullptr, webBrowerRXData, sv);
    BlockchainServer::HttpResponse *response = new BlockchainServer::HttpResponse();

    if(handlerList != nullptr && request->isParseSuccessful())
    {
        QHash<QString, BlockchainServer::HandlerCallBack>::iterator it;
        for(it = handlerList->begin(); it != handlerList->end(); it++)
        {
            if(request->matchesURL(&it.key()))
            {
                qDebug() << "Request URL :: " << *request->getURL();
                it.value()(request, response);
            }
        }
    }

//    if(request->isParseSuccessful())
//    {
//        qDebug(request->getURL()->toUtf8());
//        if(request->matchesURL(&url_path_add))
//        {
//            qDebug("Adding data...");
//            QByteArray out;
//            request->getParamData(out);
//            int ret = blockchainApp->addNew(out);
//            response->appendData("{\"status\":\"true\", \"index\":\"");
//            response->appendData(QString::number(ret).toUtf8());
//            response->appendData("\"}");
//        }
//        else if(request->matchesURL(&url_path_verify))
//        {
//            qDebug("Verifying data...");
//            QByteArray out;
//            request->getParamData(out);
//            int index = request->getIndex();
//            bool result = blockchainApp->varify(index, out);
//            if(result)
//                response->appendData("{\"status\": \"true\"}");
//            else
//                response->appendData("{\"status\": \"false\"}");
//        }
//        else if(request->matchesURL(&url_path_blockchain_index))
//        {
//            qDebug() << "Sending index limit...";
//            int indexLimit = blockchainApp->getIndexLimit();
//            response->appendData("{\"status\":\"true\", \"index_limit\":");
//            response->appendData(QString::number(indexLimit).toUtf8());
//            response->appendData("}");
//        }
//        else if(request->matchesURL(&url_path_blockchain_data))
//        {
//            qDebug() << "Sending blockchain data...";
//            char *curr_hash = new char[HASH_OUT_CHAR_LEN];
//            char *prev_hash = new char[HASH_OUT_CHAR_LEN];

//            int index = request->getIndex();
//            blockchainApp->getHashOfIndex(index, curr_hash);
//            blockchainApp->getPrevHashOfIndex(index, prev_hash);

//            response->appendData("{\"status\":\"true\", \"index\":");
//            response->appendData(QString::number(index).toUtf8());
//            response->appendData(", \"hash\":\"");
//            response->appendData(curr_hash, HASH_OUT_CHAR_LEN);
//            response->appendData("\", \"previous_hash\":\"");
//            response->appendData(prev_hash, HASH_OUT_CHAR_LEN);
//            response->appendData("\"}");

//            delete[] curr_hash;
//            delete[] prev_hash;
//        }
//        else
//        {
//            qDebug() << "Did not match";
//            response->appendData("{\"status\": \"false\"}");
//        }
//    }
//    else
//        response->appendData("{\"status\": \"false\"}");
    socket.write(response->getResponseData()->toUtf8());
    socket.waitForBytesWritten();
    socket.disconnectFromHost();

    delete request;
    delete response;
}
