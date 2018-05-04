#ifndef HANDLER_H
#define HANDLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include <QObject>

namespace BlockchainServer {
    typedef void (* HandlerCallBack) (BlockchainServer::HttpRequest* request, BlockchainServer::HttpResponse* response);
}


#endif // HANDLER_H
