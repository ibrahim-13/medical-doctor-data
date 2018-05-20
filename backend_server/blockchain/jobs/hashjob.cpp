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
