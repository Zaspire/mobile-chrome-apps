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

#include "bootstrap.h"

QByteArray kMagicString("document.readyState = 'loading';");

Bootstrap::Bootstrap(Cordova *cordova): CPlugin(cordova) {
}

void Bootstrap::onAppLoaded() {
    m_cordova->execQML(QString("root.overrideScheme('chrome-extension', '%1')").arg(Bootstrap::fullID()));
}

QVariant Bootstrap::handleUri(const QString &uri) {
    QUrl url(uri);
    qCritical() << uri << ":)";
    qCritical() << url.path();
    if (url.path() == "/chrome-content-loaded") {
        return kMagicString;
    }

    assert(url.path()[0] == '/');

    QString fileName = QDir(m_cordova->get_app_dir()).filePath(url.path().mid(1));
    QFile f1(fileName);

    if (f1.open(QIODevice::ReadOnly)) {
        QByteArray t = f1.readAll();
        return t;
    }

    return QByteArray("");
}

