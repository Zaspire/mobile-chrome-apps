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
#include "storage.h"

namespace {
const QString GetDBPath() {
    return QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation)).filePath("chrome_storage");
}
}

Storage::Storage(Cordova *cordova): CPlugin(cordova) {
}

void Storage::remove(int scId, int ecId, const QString&, const QString &) {
    if (!readDB()) {
        cb(ecId);
        return;
    }
    qCritical() << "unimplemented chrome.storage";

    if (!writeDB()) {
        cb(ecId);
        return;
    }
}

void Storage::set(int scId, int ecId, const QString& ns, const QVariantMap &newVals) {
    if (!readDB()) {
        cb(ecId);
        return;
    }
    QVariantMap old;

    QVariantMap db = _db[ns].toMap();
    for (const QString &key: newVals.keys()) {
        if (db.find(key) != db.end()) {
            old[key] = db[key];
        }
        db[key] = newVals[key];
    }
    _db[ns] = db;

    if (!writeDB()) {
        cb(ecId);
        return;
    }
    cb(scId, old);
}

void Storage::clear(int scId, int ecId, const QString& ns, const QString &) {
    if (!readDB()) {
        cb(ecId);
        return;
    }

    QVariantMap old = _db[ns].toMap();
    _db.remove(ns);

    if (!writeDB()) {
        cb(ecId);
        return;
    }
    cb(scId, old);
}

void Storage::get(int scId, int ecId, const QString& ns, const QString &) {
    if (!readDB()) {
        cb(ecId);
        return;
    }

    cb(scId, _db[ns].toMap());
}

void Storage::getBytesInUse(int scId, int ecId, const QString&, const QString &) {
    qCritical() << "unimplemented chrome.storage";
}

bool Storage::readDB() {
    if (_db.size())
        return true;

    QFile f1(GetDBPath());
    if (!f1.exists())
        return true;
    if (!f1.open(QIODevice::ReadOnly))
        return false;

    QByteArray content = f1.readAll();

    _db = QJsonDocument::fromJson(content).object().toVariantMap();

    return true;
}

bool Storage::writeDB() {
    QFile f1(GetDBPath());

    if (!f1.open(QIODevice::WriteOnly))
        return false;

    if (f1.write(QJsonDocument(QJsonObject::fromVariantMap(_db)).toJson()) == -1)
        return false;

    f1.close();

    return true;
}
