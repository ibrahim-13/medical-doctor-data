#include "blockchain.h"
#include "blockchain/jobs/hashjob.h"

Blockchain::App::App()
{
    this->storageMgr = new Blockchain::StorageManager(this);
    this->doctors = new QVector<DOCTOR_DATA*>();
    this->lockStorageMgr = new QMutex();
    this->threadPool = new QThreadPool(this);
    this->threadPool->setMaxThreadCount(1);
    this->blockCount = 0;
}

Blockchain::App::~App()
{
    this->storageMgr->deleteLater();
    this->threadPool->deleteLater();
    this->defaultInstance->deleteLater();
    QVector<DOCTOR_DATA*>::iterator it;
    for(it = doctors->begin(); it != doctors->end(); it++)
    {
        delete *it;
    }
    delete doctors;
    delete this->lockStorageMgr;
}

Blockchain::App *Blockchain::App::getDefaultInstance()
{
    static QMutex instanceLock;
    if(defaultInstance == nullptr)
    {
        instanceLock.lock();
        if(defaultInstance == nullptr)
            defaultInstance = new Blockchain::App();
        instanceLock.unlock();
    }
    return defaultInstance;
}

int Blockchain::App::addNew(QByteArray data)
{
    char *prev_hash = new char[HASH_OUT_CHAR_LEN];
    int index_limit;

    lockStorageMgr->lock();
    index_limit = storageMgr->addData(data);
    storageMgr->getPrevHashOfIndex(index_limit, prev_hash);
    lockStorageMgr->unlock();

    for(int i = 0; i < HASH_OUT_CHAR_LEN; i++)
    {
        data.append(*(prev_hash + i));
    }
    Blockchain::HashJob *hashJob = new Blockchain::HashJob((unsigned char *)data.data(), data.size(), index_limit);
    QObject::connect(hashJob, Blockchain::HashJob::hashResult, this, Blockchain::App::receiveHashResult);
    threadPool->start(hashJob);

    delete[] prev_hash;
    this->blockCount++;
    return blockCount;
}

bool Blockchain::App::addDoctorData(QString *name, QString *reg, QString *med_deg, QString *reg_date, QString *practice)
{
    DOCTOR_DATA* tmp = new DOCTOR_DATA;
    tmp->name_str.append(*name);
    tmp->name = addNew(name->toUtf8());
    tmp->reg = addNew(reg->toUtf8());
    tmp->med_deg = addNew(med_deg->toUtf8());
    tmp->reg_date = addNew(reg_date->toUtf8());
    tmp->practice = addNew(practice->toUtf8());
    doctors->append(tmp);
    return true;
}

bool Blockchain::App::getDoctorData(QString *name, QString *reg, QString *med_deg, QString *reg_date, QString *practice)
{
    if(!doctors->empty())
    {
        QVector<DOCTOR_DATA*>::iterator it;
        for(it = doctors->begin(); it != doctors->end(); it++)
        {
            DOCTOR_DATA* tmp = *it;
            if(QString::compare(*name, tmp->name_str, Qt::CaseInsensitive) == 0)
            {
                lockStorageMgr->lock();
                storageMgr->getDataOfIndex(tmp->reg, reg);
                storageMgr->getDataOfIndex(tmp->med_deg, med_deg);
                storageMgr->getDataOfIndex(tmp->reg_date, reg_date);
                storageMgr->getDataOfIndex(tmp->practice, practice);
                lockStorageMgr->unlock();
                return true;
            }
        }
    }
    return false;
}

bool Blockchain::App::varify(int index, QByteArray data)
{
    lockStorageMgr->lock();
    bool result = storageMgr->varify(index, data.data(), data.size());
    lockStorageMgr->unlock();
    return result;
}

int Blockchain::App::getIndexLimit()
{
    lockStorageMgr->lock();
    int result = (storageMgr->getIndexLimit() - 1);
    lockStorageMgr->unlock();

    return result;
}

void Blockchain::App::getHashOfIndex(int index, char hash[HASH_OUT_CHAR_LEN])
{
    lockStorageMgr->lock();
    storageMgr->getHashOfIndex(index, hash);
    lockStorageMgr->unlock();
}

void Blockchain::App::getPrevHashOfIndex(int index, char hash[HASH_OUT_CHAR_LEN])
{
    lockStorageMgr->lock();
    storageMgr->getPrevHashOfIndex(index, hash);
    lockStorageMgr->unlock();
}

void Blockchain::App::receiveHashResult(QString output, QByteArray nonce, int index)
{
    lockStorageMgr->lock();
    storageMgr->addHash(output, nonce, index);
    lockStorageMgr->unlock();
}

Blockchain::App *Blockchain::App::defaultInstance = nullptr;
