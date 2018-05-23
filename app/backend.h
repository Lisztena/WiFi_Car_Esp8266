#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class QUdpSocket;
class QHostAddress;
class QNetworkDatagram;

enum packetType{setSpeed = 1, isAvailiable};

class BackEnd : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(qreal leftDialValue READ leftDialValue WRITE setLeftDialValue NOTIFY leftDialValueChanged)
    Q_PROPERTY(qreal rightDialValue READ rightDialValue WRITE setRightDialValue NOTIFY rightDialValueChanged)
    Q_PROPERTY(qint16 serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
public:
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd() {
        setLeftDialValue(0);
        setRightDialValue(0);
        sendLeftSpeed();
        sendRightSpeed();
    }

    void setServerAddress(const QString &addr);
    void setServerPort(const qint16 &port);
    void setUdpPort(const qint16 &port);
    void setLeftDialValue(const qreal &value);
    void setRightDialValue(const qreal &value);

    QString serverAddress();
    qint16 serverPort();
    qint16 udpPort();
    qreal leftDialValue();
    qreal rightDialValue();

signals:
    void serverAddressChanged();
    void serverPortChanged();
    void udpPortChanged();
    void leftDialValueChanged();
    void rightDialValueChanged();

public slots:
    void sendLeftSpeed();
    void sendRightSpeed();
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;
    QString m_serverAddress;
    qint16 m_udpPort;
    qint16 m_serverPort;
    void processDatagram(QNetworkDatagram datagram);
    void sendPacket(packetType type,
                    QString param,
                    QString serverAddress,
                    qint16 serverPort);


    qreal m_leftDialValue;
    qreal m_rightDialValue;
    qreal m_leftSpeed;
    qreal m_rightSpeed;
};

#endif // BACKEND_H
