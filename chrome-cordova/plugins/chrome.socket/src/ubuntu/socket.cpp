/*
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "socket.h"

Socket::Socket(Cordova *cordova): CPlugin(cordova), _dist(1, 65535), _gen(_rd()) {
}

void Socket::create(int scId, int, const QString &type) {
    assert(type == "tcp");

    while (true) {
        int id = _dist(_gen);
        if (!_id2tcpSocket.contains(id)) {
            _id2tcpSocket[id] = QSharedPointer<QTcpSocket>::create();
            cb(scId, id);
            break;
        }
    }
}

void Socket::destroy(int, int, int id) {
    _id2tcpSocket.remove(id);
}

void Socket::connect(int scId, int ecId, int id, const QString &host, int port) {
    _id2tcpSocket[id]->connectToHost(host, port);

    QMetaObject::Connection handler1, handler2;
    handler1 = _id2tcpSocket[id]->connect(_id2tcpSocket[id].data(), &QAbstractSocket::connected, [=]() {
        // TODO: qtcpsocket can be removed
        _id2tcpSocket[id]->disconnect(handler1);
        _id2tcpSocket[id]->disconnect(handler2);
        cb(scId);
    });
    handler2 = _id2tcpSocket[id]->connect(_id2tcpSocket[id].data(), static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), [=](QAbstractSocket::SocketError) {
        // TODO: qtcpsocket can be removed
        _id2tcpSocket[id]->disconnect(handler1);
        _id2tcpSocket[id]->disconnect(handler2);
        cb(ecId);
    });
}

void Socket::bind(int scId, int ecId, int id, const QString &host, int port) {
    //TODO: checks
    if (_id2tcpSocket[id]->bind(port))
        cb(scId);
    else
        cb(ecId);
}

void Socket::disconnect(int, int, int id) {
    //TODO: checks
    _id2tcpSocket[id]->disconnectFromHost();
}

static QString toJSArrayBuffer(const QByteArray &content) {
    const QString str2array("\
    (function strToArray(str) {                 \
        var res = new Uint8Array(str.length);   \
        for (var i = 0; i < str.length; i++) {  \
            res[i] = str.charCodeAt(i);         \
        }                                       \
        return res;                             \
    })(\"%1\")");

    QString res;
    res.reserve(content.length() * 6);
    for (uchar c: content) {
        res += "\\x";
        res += QString::number(c, 16).rightJustified(2, '0').toUpper();
    }
    return str2array.arg(res);
}

void Socket::read(int scId, int ecId, int id, int size) {
    QByteArray res;
    //TODO: error handler
    if (size > 0)
        res = _id2tcpSocket[id]->read(size);
    else
        res = _id2tcpSocket[id]->readAll();

    callback(scId, toJSArrayBuffer(res));
}

void Socket::write(int scId, int ecId, int id, const QByteArray &data) {
    //TODO
    if (_id2tcpSocket[id]->write(data) == data.size())
        cb(scId, data.size());
    else
        cb(ecId);
}
