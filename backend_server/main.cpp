#include <QCoreApplication>
#include "HttpServer/httpserver.h"
#include "HttpServer/data/handler.h"
#include "HttpServer/data/httprequest.h"
#include "HttpServer/data/httpresponse.h"
#include "blockchain/blockchain.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    BlockchainServer::HttpServer server;

    server.addHandler("/api/blockchain/index", [](BlockchainServer::HttpRequest* request, BlockchainServer::HttpResponse* response)
    {
        qDebug("Sending index limit...");
        int indexLimit = Blockchain::App::getDefaultInstance()->getIndexLimit();
        response->appendData("{\"status\":\"true\", \"index_limit\":");
        response->appendData(QString::number(indexLimit).toUtf8());
        response->appendData("}");
    });

    server.addHandler("/api/add", [](BlockchainServer::HttpRequest* request, BlockchainServer::HttpResponse* response)
    {
        qDebug("Adding data...");
        QByteArray out;
        QString name, reg, med_degree, reg_date, practice;
        request->getParamByKey("name", &name);
        request->getParamByKey("reg", &reg);
        request->getParamByKey("med_deg", &med_degree);
        request->getParamByKey("reg_date", &reg_date);
        request->getParamByKey("practice", &practice);
        bool ret = Blockchain::App::getDefaultInstance()->addDoctorData(&name, &reg, &med_degree, &reg_date, &practice);
        if(ret)
            response->appendData("{\"status\":\"true\"}");
        else
            response->appendData("{\"status\":\"false\"}");
    });
    server.addHandler("/api/getData", [](BlockchainServer::HttpRequest* request, BlockchainServer::HttpResponse* response)
    {
        qDebug("Sending data...");
        QString name, reg, med_degree, reg_date, practice;
        request->getParamByKey("name", &name);
        bool ret = Blockchain::App::getDefaultInstance()->getDoctorData(&name, &reg, &med_degree, &reg_date, &practice);

        if(ret)
        {
            response->appendData("{\"status\":\"true\",");
            response->appendData("\"name\":\"");
            response->appendData(name);
            response->appendData("\",");
            response->appendData("\"reg\":\"");
            response->appendData(reg);
            response->appendData("\",");
            response->appendData("\"med_degree\":\"");
            response->appendData(med_degree);
            response->appendData("\",");
            response->appendData("\"reg_date\":\"");
            response->appendData(reg_date);
            response->appendData("\",");
            response->appendData("\"practice\":\"");
            response->appendData(practice);
            response->appendData("\"}");
        }
        else
            response->appendData("{\"status\":\"false\"}");
    });

//    server.addHandler("/api/verify", [](BlockchainServer::HttpRequest* request, BlockchainServer::HttpResponse* response)
//    {
//        qDebug("Verifying data...");
//        QByteArray out;
//        request->getParamData(out);
//        int index = request->getIndex();
//        bool result = Blockchain::App::getDefaultInstance()->varify(index, out);
//        if(result)
//            response->appendData("{\"status\": \"true\"}");
//        else
//            response->appendData("{\"status\": \"false\"}");
//    });

    server.addHandler("/api/blockchain/data", [](BlockchainServer::HttpRequest* request, BlockchainServer::HttpResponse* response)
    {
        qDebug() << "Sending blockchain data...";
        char *curr_hash = new char[HASH_OUT_CHAR_LEN];
        char *prev_hash = new char[HASH_OUT_CHAR_LEN];

        int index;
        QString index_str;
        request->getParamByKey("index", &index_str);
        index = index_str.toInt();

        Blockchain::App::getDefaultInstance()->getHashOfIndex(index, curr_hash);
        Blockchain::App::getDefaultInstance()->getPrevHashOfIndex(index, prev_hash);

        response->appendData("{\"status\":\"true\", \"index\":");
        response->appendData(QString::number(index).toUtf8());
        response->appendData(", \"hash\":\"");
        response->appendData(curr_hash, HASH_OUT_CHAR_LEN);
        response->appendData("\", \"previous_hash\":\"");
        response->appendData(prev_hash, HASH_OUT_CHAR_LEN);
        response->appendData("\"}");

        delete[] curr_hash;
        delete[] prev_hash;
    });

    return a.exec();
}
