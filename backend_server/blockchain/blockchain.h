#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <blockchain/config.h>
#include <blockchain/datastore/blockchainstore.h>
#include <QByteArray>
#include <QThreadPool>
#include <QMutex>
#include <QVector>

namespace Blockchain {
    class App : public QObject
    {
    public:
        ~App();
        int getIndexLimit();
        bool addDoctorData(QString* name, QString* reg, QString* med_deg, QString* reg_date, QString* practice);
        bool getDoctorData(QString* name, QString* reg, QString* med_deg, QString* reg_date, QString* practice);
        void getHashOfIndex(int index, char hash[HASH_OUT_CHAR_LEN]);
        void getPrevHashOfIndex(int index, char hash[HASH_OUT_CHAR_LEN]);
        static Blockchain::App* getDefaultInstance();

    public slots:
        void receiveHashResult(QString output, QByteArray nonce, int index);
    private:
        typedef struct {
            QString name_str;
            int name;
            int reg;
            int med_deg;
            int reg_date;
            int practice;
        } DOCTOR_DATA;
        App();
        static Blockchain::App *defaultInstance;
        Blockchain::StorageManager *storageMgr;
        QThreadPool *threadPool;
        QMutex *lockStorageMgr;
        QVector<DOCTOR_DATA*> *doctors;
        int blockCount;
        int addNew(QByteArray data);
        bool varify(int index, QByteArray data);
    };
}

#endif // BLOCKCHAIN_H
