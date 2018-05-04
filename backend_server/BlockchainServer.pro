QT -= gui
QT += network
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    HttpServer/jobs/conncectionhandler.cpp \
    HttpServer/httpserver.cpp \
    blockchain/datastore/blockchainstore.cpp \
    blockchain/jobs/hashjob.cpp \
    blockchain/mbedtls/sha512.c \
    blockchain/blockchain.cpp \
    HttpServer/data/httprequest.cpp \
    HttpServer/data/httpresponse.cpp

HEADERS += \
    HttpServer/jobs/conncectionhandler.h \
    HttpServer/httpserver.h \
    blockchain/datastore/blockchainstore.h \
    blockchain/jobs/hashjob.h \
    blockchain/mbedtls/includes/sha512.h \
    blockchain/config.h \
    blockchain/blockchain.h \
    HttpServer/data/httprequest.h \
    config.h \
    HttpServer/data/httpresponse.h \
    HttpServer/data/handler.h
