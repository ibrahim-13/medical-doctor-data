#include "hashjob.h"
#include "blockchain/config.h"
#include "blockchain/mbedtls/includes/sha512.h"
#include <iostream>
#include <QDebug>

Blockchain::HashJob::HashJob(unsigned char *data, int length, int index)
{
    len_data = length;
    data_index = index;
    this->data = new unsigned char[len_data + NONCE_LEN];
    memcpy(this->data, data, len_data);
    for(int i = 0; i < NONCE_LEN; i++)
    {
        this->data[len_data + i] = 0x00;
    }
    STARTS_WITH = new char[LEN_STARTS_WITH];
    for(int i = 0; i < LEN_STARTS_WITH; i++)
    {
        STARTS_WITH[i] = '0';
    }
}

Blockchain::HashJob::~HashJob()
{
    delete STARTS_WITH;
    delete data;
}

void Blockchain::HashJob::run()
{
    unsigned char output[HASH_OUT_LEN];
    char output_char[HASH_OUT_CHAR_LEN];
    do
    {
        mbedtls_sha512(this->data, (len_data + NONCE_LEN), output, 0);
        getHashArray(output, (unsigned char *)output_char);
        if(strncmp(output_char, STARTS_WITH, LEN_STARTS_WITH) == 0) {
            break;
        }
    }
    while(increment(this->data, len_data, NONCE_LEN) != -1);
    QString str = QString();
    for(int i = 0; i < HASH_OUT_CHAR_LEN; i++)
    {
        str.append(QChar(output_char[i]));
    }
    QByteArray nonce = QByteArray((const char *)(this->data + len_data), NONCE_LEN);
    emit hashResult(str, nonce, data_index);
}

int Blockchain::HashJob::increment(unsigned char *src, int offset, int length)
{
    int i;
        for(i = 0; i < length; i++)
        {
            if((*(src + offset + i)) == 0xFF)
            {
                (*(src + offset + i)) = 0x00;
                continue;
            }
            else
            {
                (*(src + offset + i))++;
                return i;
            }
        }
        return -1;
}

void Blockchain::HashJob::getHashArray(unsigned char source[HASH_OUT_LEN], unsigned char output[HASH_OUT_CHAR_LEN])
{
    int i;
    for(i = 0; i < HASH_OUT_LEN; i++)
    {
        output[i * 2] = hex_to_char(source[i] >> 4);
        output[(i * 2) + 1] = hex_to_char(source[i] & 0x0F);
    }
}

char Blockchain::HashJob::hex_to_char(const char source)
{
    const char *hex_lookup = "0123456789ABCDEF";
    return hex_lookup[(int)source];
}
