#ifndef HASHJOB_H
#define HASHJOB_H

#include "blockchain/config.h"
#include <QObject>
#include <QRunnable>
#include <QByteArray>

namespace Blockchain {
    class HashJob : public QObject, public QRunnable
    {
        Q_OBJECT

    public:
        HashJob(unsigned char *data, int length, int index);
        ~HashJob();
        void run();

    signals:
        void hashResult(QString output, QByteArray nonce, int index);

    private:
        char *STARTS_WITH;
        unsigned char *data;
        int len_data;
        int data_index;
        int increment(unsigned char *src, int offset, int length);
        void getHashArray(unsigned char source[HASH_OUT_LEN], unsigned char output[HASH_OUT_CHAR_LEN]);
        char hex_to_char(const char source);
    };
}

#endif // HASHJOB_H
