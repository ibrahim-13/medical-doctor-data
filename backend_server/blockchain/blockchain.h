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
