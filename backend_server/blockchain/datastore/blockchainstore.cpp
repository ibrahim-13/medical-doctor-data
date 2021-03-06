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

#include "blockchainstore.h"
#include "blockchain/config.h"
#include "blockchain/mbedtls/includes/sha512.h"
#include <string>
#include <QDebug>
#include <iostream>

Blockchain::StorageManager::StorageManager(QObject *parent) : QObject(parent)
{
    block_list = new QList<BLOCK_DATA*>;
    BLOCK_DATA *tmp = new BLOCK_DATA;
    for(int i = 0; i < HASH_OUT_CHAR_LEN; i++)
    {
        tmp->curr_hash[i] = 'F';
        tmp->prev_hash[i] = 'F';
        tmp->index = 0;
    }
    block_list->append(tmp);
}

Blockchain::StorageManager::~StorageManager()
{
    for(int i = 0; i < block_list->size(); i++)
    {
        BLOCK_DATA* tmp = block_list->value(i);
        delete[] tmp->data;
        delete tmp;
    }
    block_list->clear();
    delete block_list;
}

int Blockchain::StorageManager::addData(QByteArray data)
{
    BLOCK_DATA *tmp = new BLOCK_DATA;;
    tmp->len_data = data.length();
    tmp->data = new char[tmp->len_data];
    memcpy(tmp->data, data.data(), data.length());
    block_list->append(tmp);

    return (block_list->size() - 1);
}

int Blockchain::StorageManager::addHash(QString hash, QByteArray nonce, int index)
{
    BLOCK_DATA* tmp = block_list->value(index);
    QByteArray hash_byte = hash.toUtf8();
    memcpy(tmp->curr_hash, hash_byte.data(), hash_byte.length());
    memcpy(tmp->nonce, nonce.data(), nonce.length());
    return (block_list->size() - 1);
}

bool Blockchain::StorageManager::varify(int index, char *data, int size)
{
    char *hash = new char[HASH_OUT_CHAR_LEN];
    char *curr_hash = new char[HASH_OUT_CHAR_LEN];
    char *prev_hash = new char[HASH_OUT_CHAR_LEN];
    unsigned char *output = new unsigned char[HASH_OUT_LEN];
    unsigned char *nonce = new unsigned char[NONCE_LEN];
    bool result = false;

    getHashOfIndex(index, curr_hash);
    getPrevHashOfIndex(index, prev_hash);
    getNonceOfIndex(index, (char *)nonce, NONCE_LEN);

    mbedtls_sha512_context *ctx = new mbedtls_sha512_context;
    mbedtls_sha512_init(ctx);
    mbedtls_sha512_starts(ctx, 0);
    mbedtls_sha512_update(ctx, (const unsigned char *)data, size);
    mbedtls_sha512_update(ctx, (const unsigned char *)prev_hash, HASH_OUT_CHAR_LEN);
    mbedtls_sha512_update(ctx, (const unsigned char *)nonce, NONCE_LEN);
    mbedtls_sha512_finish(ctx, output);
    getHashArray((unsigned char *)output, (unsigned char *)hash);

    if(memcmp(hash, curr_hash, HASH_OUT_CHAR_LEN) == 0)
        result = true;

    delete[] hash;
    delete[] curr_hash;
    delete[] prev_hash;
    delete[] output;
    delete[] nonce;
    delete ctx;

    return result;
}

int Blockchain::StorageManager::getIndexLimit() const
{
    return block_list->length();
}

void Blockchain::StorageManager::getHashOfIndex(
        int i, char output[HASH_OUT_CHAR_LEN]
        ) const
{
    BLOCK_DATA *tmp = block_list->value(i);
    memcpy(output, tmp->curr_hash, HASH_OUT_CHAR_LEN);
}

void Blockchain::StorageManager::getPrevHashOfIndex(
        int i, char output[HASH_OUT_CHAR_LEN]
        ) const
{
    //BLOCK_DATA tmp = block_list->value(i);
    //memcpy(output, tmp.prev_hash, 128);
    int pos = 0;
    if(i > 0)
        pos = i - 1;
    BLOCK_DATA *tmp = block_list->value(pos);
    memcpy(output, tmp->curr_hash, HASH_OUT_CHAR_LEN);
}

void Blockchain::StorageManager::getNonceOfIndex(int i, char *dst, int size)
{
    BLOCK_DATA *tmp = block_list->value(i);
    memcpy(dst, tmp->nonce, size);
}

void Blockchain::StorageManager::getDataOfIndex(int index, QString *data)
{
    BLOCK_DATA* tmp = block_list->value(index);
    for(int i = 0; i < tmp->len_data; i++)
        data->append(QChar(*(tmp->data + i)));
}

void Blockchain::StorageManager::getHashArray(
        unsigned char source[HASH_OUT_LEN], unsigned char output[HASH_OUT_CHAR_LEN]
        )
{
    int i;
    for(i = 0; i < HASH_OUT_LEN; i++)
    {
        output[i * 2] = hex_to_char(source[i] >> 4);
        output[(i * 2) + 1] = hex_to_char(source[i] & 0x0F);
    }
}

char Blockchain::StorageManager::hex_to_char(const char source)
{
    const char *hex_lookup = "0123456789ABCDEF";
    return hex_lookup[(int)source];
}
