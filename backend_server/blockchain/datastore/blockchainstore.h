#ifndef BLOCKCHAINSTORE_H
#define BLOCKCHAINSTORE_H

#include "blockchain/config.h"
#include <QList>
#include <QtCore>

namespace Blockchain {
    class StorageManager : public QObject
    {
        Q_OBJECT
    public:
        explicit StorageManager(QObject *parent = 0);
        ~StorageManager();
        int getIndexLimit() const;
        void getHashOfIndex(int i, char output[HASH_OUT_CHAR_LEN]) const;
        void getPrevHashOfIndex(int i, char output[HASH_OUT_CHAR_LEN]) const;
        void getNonceOfIndex(int i, char *dst, int size);
        void getDataOfIndex(int index, QString* data);
    //    int add(char *data, int size);
        int addData(QByteArray data);
        int addHash(QString hash, QByteArray nonce, int index);
        bool varify(int index, char *data, int size);

    private:
        typedef struct block {
            int index;
            int index_prev_hash;
            char* data;
            int len_data;
            unsigned char prev_hash[HASH_OUT_CHAR_LEN];
            unsigned char curr_hash[HASH_OUT_CHAR_LEN];
            unsigned char nonce[NONCE_LEN];
        } BLOCK_DATA;
        QList<BLOCK_DATA*> *block_list;
        void getHashArray(unsigned char source[64], unsigned char output[HASH_OUT_CHAR_LEN]);
        char hex_to_char(const char source);

    signals:

    public slots:
    };
}

#endif // BLOCKCHAINSTORE_H
