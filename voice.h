#ifndef VOICE_H
#define VOICE_H

#include <QObject>
#include "spcom.h"
#include "v_modbusrx.h"
#include "v_modbutx.h"
#include "agvparastruct.h"
class voice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setportName NOTIFY portNameChanged)
    Q_PROPERTY(int baudRate READ baudRate WRITE setbaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(bool isOpen READ isOpen)
public:
    explicit voice(QObject *parent = 0);
    bool isOpen() const;

    void setportName(const QString &name);
    QString portName()const;

    void setbaudRate(int baudRate);
    int baudRate() const;

    Q_INVOKABLE virtual bool open();
    Q_INVOKABLE virtual void close();
    Q_INVOKABLE virtual bool clear();
    Q_INVOKABLE int readData(char *buffer,int count,int timeout = 1000);
    Q_INVOKABLE int writeData();
    Q_INVOKABLE int write(char ch);
    Q_INVOKABLE void testEmitCmd();

    //==============================
    //TESTTEST
    //===============================
    Q_INVOKABLE void v_VoicePlay(QString name);
signals:
    void portNameChanged();
    void baudRateChanged();
    void recOverSig(QByteArray buffer,int cnt);
    void sendVoiceWordSig(QString str);

    void v_cmdReceiveOverSig(QByteArray qReceiveCmd,QByteArray qReceivePara);
public slots:
    void recDataSlot(QByteArray buffer,int cnt);
    void recVoiceWordSlot(qVect16 qWord,ushort cnt);
    void v_RecCmdSlot(QByteArray cmd,QByteArray para);
protected:
    MODBUSActive *me;
    SPcom * m_vspcom;
    v_modbusrx *m_vmodbusRx;
    v_modbutx *m_vmodbusTx;
};
#endif // VOICE_H
