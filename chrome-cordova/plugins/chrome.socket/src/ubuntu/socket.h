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

#ifndef _SOCKET_H_HKJMBNCVA67867
#define _SOCKET_H_HKJMBNCVA67867

#include <QtCore>
#include <QtNetwork>
#include <random>

#include <cplugin.h>
#include <cordova.h>

class Socket: public CPlugin {
    Q_OBJECT
public:
    explicit Socket(Cordova *cordova);

    virtual const QString fullName() override {
        return Socket::fullID();
    }

    virtual const QString shortName() override {
        return Socket::fullID();
    }

    static const QString fullID() {
        return "ChromeSocket";
    }

public slots:
    void create(int scId, int, const QString &type); // type-> "tcp", "udp"
    void destroy(int, int, int id);
    void connect(int scId, int ecId, int id, const QString &host, int port);
    void bind(int scId, int ecId, int id, const QString &host, int port);
    void disconnect(int, int, int id);
    void read(int scId, int ecId, int id, int size);
    void write(int scId, int ecId, int id, const QByteArray &data);

private:
    QMap<int, QSharedPointer<QTcpSocket>> _id2tcpSocket;
    std::uniform_int_distribution<int> _dist;
    std::random_device _rd;
    std::default_random_engine _gen;
};

#endif
