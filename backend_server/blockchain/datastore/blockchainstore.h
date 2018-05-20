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
