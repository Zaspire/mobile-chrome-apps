
#include "identity.h"

#include <online-accounts-client/setup.h>
#include <Accounts/Manager>
#include <Accounts/Application>
#include <Accounts/AccountService>
#include <SignOn/Identity>
#include <SignOn/SessionData>

static QString appId() {
    return QCoreApplication::applicationName() + "_cordova";
}

Identity::Identity(Cordova *cordova): CPlugin(cordova) {
}

bool Identity::hasAccount() {
    Accounts::Manager manager;
    Accounts::Application app = manager.application(appId());

    auto list = manager.accountListEnabled();
    for (Accounts::AccountId id: list) {
        Accounts::Account *account = Accounts::Account::fromId(&manager, id);
        if (account->providerName() == "google")
            return true;
    }

    return false;
}

void Identity::login(int scId, int ecId, const QVariantMap &oauth2) {
    Accounts::Manager manager;
    Accounts::Application app = manager.application(appId());
    auto list = manager.accountListEnabled();
    for (Accounts::AccountId id: list) {
        Accounts::Account *account = Accounts::Account::fromId(&manager, id, this);
        if (account->providerName() != "google")
            continue;

        for (Accounts::Service &service: account->services()) {
            if (app.isValid() && app.serviceUsage(service).isEmpty())
                continue;

            Accounts::AccountService *s = new Accounts::AccountService(account, service);
            Accounts::AuthData authData = s->authData();

            SignOn::Identity *identity = SignOn::Identity::existingIdentity(authData.credentialsId(), this);
            QPointer<SignOn::AuthSession> authSession = identity->createSession(authData.method());

            QVariantMap map = authData.parameters();
            map.insert("ClientId", oauth2["client_id"]);
            map.remove("ClientSecret");
            //            map.insert("ClientSecret", QString("vLxhjmr4joC-Ghp6qxRWJ-xV"));

            map.insert("ResponseType", QString("token"));
            map.insert("AuthPath", QString("o/oauth2/auth"));

            map.insert("WindowId", m_cordova->rootObject()->window()->winId());

            QStringList scopes;
            for (QVariant &v: oauth2["scopes"].toList()) {
                assert(v.toString().size());
                scopes.append(v.toString());
            }
            map.insert("Scope", scopes);
            map.insert("RedirectUri", QString("http://localhost/"));

            authSession->connect(authSession.data(), &SignOn::AuthSession::response, [authSession, scId, this] (const SignOn::SessionData &sessionData) {
                QVariantMap obj;
                obj.insert("token", sessionData.toMap()["AccessToken"]);
                cb(scId, obj);
            });
            authSession->connect(authSession.data(), &SignOn::AuthSession::error, [authSession, ecId, this] (const SignOn::Error &) {
                cb(ecId);
            });
            authSession->process(map, "user_agent");
            return;
        }
    }
    cb(ecId, "not_authorized");
}


void Identity::getAuthToken(int scId, int ecId, bool, const QVariantMap &oauth2) {
    if (!hasAccount()) {
        OnlineAccountsClient::Setup *setup = new OnlineAccountsClient::Setup(this);

        setup->setProviderId("google");
#ifndef Q_PROCESSOR_X86
        setup->setApplicationId(appId());
#endif
        setup->connect(setup, &OnlineAccountsClient::Setup::finished, [=] () {
            login(scId, ecId, oauth2);
        });
        setup->exec();
    } else
        login(scId, ecId, oauth2);
}

void Identity::removeCachedAuthToken(int, int, const QString &) {
}
