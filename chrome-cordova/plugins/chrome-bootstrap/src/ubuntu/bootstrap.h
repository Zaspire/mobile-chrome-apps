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

#ifndef _BOOTSTRAP_H_FASFVCZXV6437385
#define _BOOTSTRAP_H_FASFVCZXV6437385

#include <QtCore>

#include <cplugin.h>
#include <cordova.h>

class Bootstrap: public CPlugin {
    Q_OBJECT
public:
    explicit Bootstrap(Cordova *cordova);

    void onAppLoaded();

    virtual const QString fullName() override {
        return Bootstrap::fullID();
    }

    virtual const QString shortName() override {
        return Bootstrap::fullID();
    }

    static const QString fullID() {
        return "ChromeBootstrap";
    }
public slots:
    QVariant handleUri(const QString &uri);
    void doesNeedLaunch(int scId, int) {
        cb(scId, true);
    }
};

#endif
