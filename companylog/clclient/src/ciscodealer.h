#ifndef CISCODEALER_H
#define CISCODEALER_H

#include <QObject>
#include <QIODevice>
#include <QHttp>
#include <QBuffer>
#include <QSslError>
#include <QMessageBox>
#include <QSslSocket>
#include <QNetworkCookie>
#include <QEventLoop>
#include <QSslError>
#include <QUrl>
enum {
    RequestType_RegisterMe,
    RequestType_SendPassword,
    RequestType_SendDevice,

    RequestType_none
};

enum {
    CiscoRequestRv_OK=1,
    CiscoRequestRv_ConnectionFailed,
    CiscoRequestRv_AuthenticationFailed,
    CiscoRequestRv_DeviceRegisterFailed
};

class CiscoDealer : public QObject
{
    Q_OBJECT
public:
    CiscoDealer();
    ~CiscoDealer();
    int deal(QString xnumber, QString xpasswd, QString xuser,QString xmyNumber);
public slots:
    void requestFinished(int id, bool error);
    void requestStarted(int id);
    void sslErrors(const QList<QSslError> &errors);
private:
    void sendRequest(QString url, int requestType);
    void registerMe();
    void sendPassword();
    void sendDevice();
    void callDevice();
    QEventLoop loop;

    QHttp	*qhttp;
    QString baseurl;
    QString jsession;
    QString device;
    QString cookie;
    QString sess;
    QString sep;

    QString number;
    QString passwd;
    QString user;
    QString myNumber;

    bool    readyToCall;
    QBuffer *buf;
    int     actual_request_id;
    int     actual_request_type;
};

#endif // CISCODEALER_H
