
#include "identity.h"

#include <online-accounts-client/setup.h>
#include <Accounts/Manager>
#include <Accounts/AccountService>
#include <SignOn/Identity>
#include <SignOn/SessionData>

Identity::Identity(Cordova *cordova): CPlugin(cordova) {
    OnlineAccountsClient::Setup().exec();
}

void Identity::getAuthToken(int scId, int ecId, bool interactive, const QVariantMap &oauth2) {
    Accounts::Manager manager("mail"); //type from file
    Accounts::Service service; //from file

    auto list = manager.accountListEnabled();
    for (Accounts::AccountId id: list) {
        Accounts::Account *account = Accounts::Account::fromId(&manager, id);
        Accounts::AccountService *s = new Accounts::AccountService(account, service);
        auto authData = s->authData();

        SignOn::Identity *identity = SignOn::Identity::existingIdentity(authData.credentialsId());
        auto authSession = identity->createSession(authData.method());

        QVariantMap map = authData.parameters();
        map.insert("UiPolicy", SignOn::RequestPasswordPolicy);
        map.insert("ClientId", oauth2["client_id"]);
        //        map.insert("ClientSecret", "mZk50Jl_MhDhxwLGLv8eFyJW");

        map.insert("WindowId", 10);

        QStringList scopes;
        for (QVariant &v: oauth2["scopes"].toList()) {
            assert(v.toString().size());
            scopes.append(v.toString());
        }

        map.insert("Scope", scopes);
        map.insert("RedirectUri", QString("https://oficlfehfenioickohognhdhmmcpceil.chromiumapp.org/"));

        qCritical() << oauth2["scopes"];
        /*
    "client_id": "665859454684.apps.googleusercontent.com",
    "scopes": [ -->> List of strings
      "https://www.googleapis.com/auth/drive"
      ]*/
        QMetaObject::Connection handler1, handler2;
        authSession->connect(authSession.data(), &SignOn::AuthSession::response, [=] (const SignOn::SessionData &sessionData) {
                qCritical() << sessionData.toMap();
                qCritical() << "1";
                authSession->name();
                QVariantMap obj;
                obj.insert("token", sessionData.toMap()["AccessToken"]);
                cb(scId, obj);
                qCritical() << "2";
        });
        authSession->process(map, authData.mechanism());
        //    void error(const SignOn::Error &err);

        //    void response(const SignOn::SessionData &sessionData);
    //---> toMap;
    }
}

void Identity::removeCachedAuthToken(int scId, int ecId, const QString &token) {
}
