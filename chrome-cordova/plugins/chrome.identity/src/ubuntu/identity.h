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

#ifndef _IDENTITY_H_HJKHJDASB6769821
#define _IDENTITY_H_HJKHJDASB6769821

#include <QtCore>
#include <QtQuick>

#include <cplugin.h>
#include <cordova.h>

class Identity: public CPlugin {
    Q_OBJECT
public:
    explicit Identity(Cordova *cordova);

    virtual const QString fullName() override {
        return Identity::fullID();
    }

    virtual const QString shortName() override {
        return Identity::fullID();
    }

    static const QString fullID() {
        return "ChromeIdentity";
    }
public slots:
    void getAuthToken(int scId, int ecId, bool interactive, const QVariantMap &oauth2);
    void removeCachedAuthToken(int scId, int ecId, const QString &token);
private:
    void login(int scId, int ecId, const QVariantMap &oauth2);
    bool hasAccount();
};

#endif
