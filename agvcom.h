#ifndef AGVCOM_H
#define AGVCOM_H

#include <QObject>
#include "agvparastruct.h"
#include "spcom.h"
#include "agvrx.h"
#include "agvtx.h"
class AGVCom : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setportName NOTIFY portNameChanged)
    Q_PROPERTY(int baudRate READ baudRate WRITE setbaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(bool isOpen READ isOpen)
    Q_PROPERTY(float agvfwdspd READ agvfwdspd WRITE setAgvfwdspd NOTIFY agvfwdspdChanged)
    /**************前进*******************/
//     Q_PROPERTY(int fwdSpd READ fwdSpd WRITE setfwdSpd NOTIFY fwdSpdChanged)
//    Q_PROPERTY(float fwdSpd READ fwdSpd WRITE setfwdSpd NOTIFY fwdSpdChanged)
//    /**************转向*******************/
//    Q_PROPERTY(float rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
//    /**************目标位置*******************/
//    Q_PROPERTY(float Aimx READ Aimx WRITE setAimx NOTIFY AimxChanged)
//    Q_PROPERTY(float Aimy READ Aimy WRITE setAimy NOTIFY AimyChanged)
//    Q_PROPERTY(float Aima READ Aima WRITE setAima NOTIFY AimaChanged)
//    Q_PROPERTY(float Aimw READ Aimw WRITE setAimw NOTIFY AimwChanged)
//    /**************起始位置*******************/
//    Q_PROPERTY(float Startx READ Startx WRITE setStartx NOTIFY StartxChanged)
//    Q_PROPERTY(float Starty READ Starty WRITE setStarty NOTIFY StartyChanged)
//    Q_PROPERTY(float Starta READ Starta WRITE setStarta NOTIFY StartaChanged)
//    Q_PROPERTY(float Startw READ Startw WRITE setStartw NOTIFY StartwChanged)
    /**************当前机器人位置*******************/
    Q_PROPERTY(float CurX READ CurX   )
    Q_PROPERTY(float CurY READ CurY   )
    Q_PROPERTY(float CurA READ CurA   )
    Q_PROPERTY(float CurW READ CurW   )
    /**************左前超生*******************/
    Q_PROPERTY(ushort leftFwdUltrasonic READ leftFwdUltrasonic  NOTIFY leftFwdUltrasonicChanged)
    /**************右前超生*******************/
    Q_PROPERTY(ushort rightFwdUltrasonic READ rightFwdUltrasonic NOTIFY rightFwdUltrasonicChanged)
    /**************左后超生*******************/
    Q_PROPERTY(ushort leftBwdUltrasonic READ leftBwdUltrasonic  NOTIFY leftBwdUltrasonicChanged)
    /**************右后超生*******************/
    Q_PROPERTY(ushort rightBwdUltrasonic READ rightBwdUltrasonic  NOTIFY rightBwdUltrasonicChanged)
    /**************左碰撞*********************/
    Q_PROPERTY(ushort leftCollision READ leftCollision NOTIFY leftCollisionChanged)
    /**************右碰撞*********************/
    Q_PROPERTY(ushort rightCollision READ rightCollision NOTIFY rightCollisionChanged)
    /**************左轮抬起*********************/
    Q_PROPERTY(ushort leftWheelNoContact READ leftWheelNoContact  NOTIFY leftWheelNoContactChanged)
    /**************右轮抬起*********************/
    Q_PROPERTY(ushort rightWheelNoContact READ rightWheelNoContact NOTIFY rightWheelNoContactChanged)
    /***************机器人返回状态***************/
    Q_PROPERTY(ushort agvState READ agvState  NOTIFY agvStateChanged)
public:
    explicit AGVCom(QObject *parent = 0);

    bool isOpen() const;

    float agvfwdspd() const;
    void setAgvfwdspd(float agvfwdspd);
    /**************机器人当前位置****************/
    float CurX() const;
    float CurY() const;
    float CurA() const;
    float CurW() const;
    //左前超生
    ushort leftFwdUltrasonic() const;
    //右前超生
    ushort rightFwdUltrasonic() const;
    //左后超生
    ushort leftBwdUltrasonic() const;
    //右后超生
    ushort rightBwdUltrasonic() const;
    //左碰撞
    ushort leftCollision() const;
    //右碰撞
    ushort rightCollision() const;
    //左轮抬起
    ushort leftWheelNoContact() const;
    //右轮抬起
    ushort rightWheelNoContact() const;
    //机器人状态
    ushort agvState() const;


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
    Q_INVOKABLE void setSpdRotation(float spd,float rotation);
    Q_INVOKABLE bool agvComState() const;
protected:
    SPcom * m_Agvspcom;
    agvRx * m_AgvRx;\
    AGVTx * m_AgvTx;
    float m_curagvfwdspd;
public slots:
    void recSendDataSlot(QByteArray buffer,int cnt);
    Q_INVOKABLE void agvRec_CtrData_Slot(QByteArray qSendData);

signals:
    void portNameChanged();
    void baudRateChanged();
    void fwdSpdChanged();
    void agvfwdspdChanged();
    void leftFwdUltrasonicChanged();    //左前超生
    void rightFwdUltrasonicChanged();    //右前超生
    void leftBwdUltrasonicChanged();    //左后超生
    void rightBwdUltrasonicChanged();    //右后超生
    void leftCollisionChanged();    //左碰撞
    void rightCollisionChanged();         //右碰撞
    void leftWheelNoContactChanged();     //左轮抬起
    void rightWheelNoContactChanged();    //右轮抬起
    void agvStateChanged();               //机器人状态
    void agvCurPositionChanged(Tag_position tagposition);              //当前位置改变
    void recOverSig(QByteArray buffer,int cnt);
    void writeDataSig(QByteArray buffer,ushort cnt);

};

#endif // AGVCOM_H
