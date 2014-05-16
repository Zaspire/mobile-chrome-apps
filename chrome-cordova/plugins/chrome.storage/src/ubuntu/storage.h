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
#ifndef _STORAGE_H_NMBILHKBFMAS78453
#define _STORAGE_H_NMBILHKBFMAS78453

#include <QtCore>

#include <cplugin.h>
#include <cordova.h>

class Storage: public CPlugin {
    Q_OBJECT
public:
    explicit Storage(Cordova *cordova);

    virtual const QString fullName() override {
        return Storage::fullID();
    }

    virtual const QString shortName() override {
        return Storage::fullID();
    }

    static const QString fullID() {
        return "ChromeStorage";
    }
public slots:
    void remove(int scId, int ecId, const QString& ns, const QString &val);
    void set(int scId, int ecId, const QString& ns, const QVariantMap &);
    void clear(int scId, int ecId, const QString& ns, const QString &val);
    void get(int scId, int ecId, const QString& ns, const QString &names);
    void getBytesInUse(int scId, int ecId, const QString& ns, const QString &names);
private:
    bool readDB();
    bool writeDB();

    QVariantMap _db;
};

#endif
