#include "backend.h"

#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>


BackEnd::BackEnd(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket(this);
    m_serverAddress = "192.168.10.1";
    QHostAddress addr(m_serverAddress);
    m_serverPort = 4210;
    m_leftDialValue = 0;
    m_rightDialValue = 0;

    udpSocket->bind(addr, m_serverPort);

    connect(this, SIGNAL(leftDialValueChanged()), this, SLOT(sendLeftSpeed()));
    connect(this, SIGNAL(rightDialValueChanged()), this, SLOT(sendRightSpeed()));
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

QString BackEnd::serverAddress() {
    return m_serverAddress;
}

qint16 BackEnd::serverPort() {
    return m_serverPort;
}

qint16 BackEnd::udpPort() {
    return m_udpPort;
}

qreal BackEnd::leftDialValue() {
    return m_leftDialValue;
}

qreal BackEnd::rightDialValue() {
    return m_rightDialValue;
}

void BackEnd::setServerAddress(const QString &addr) {
    if (addr == m_serverAddress)
        return;
    m_serverAddress = addr;
    emit serverAddressChanged();
}

void BackEnd::setServerPort(const qint16 &port) {
    if (port == m_serverPort)
        return;
    m_serverPort = port;
    emit serverPortChanged();
}

void BackEnd::setUdpPort(const qint16 &port) {
    if (port == m_udpPort)
        return;
    m_udpPort = port;
    emit udpPortChanged();
}

void BackEnd::setLeftDialValue(const qreal &value) {
    m_leftDialValue = (int)value;
    emit leftDialValueChanged();
}

void BackEnd::setRightDialValue(const qreal &value) {
    m_rightDialValue = (int)value;
    emit rightDialValueChanged();
}

void BackEnd::sendPacket(packetType type,
                         QString param,
                         QString serverAddress,
                         qint16 serverPort) {
    QHostAddress addr(serverAddress);
    QString text;
    switch (type) {
    case setSpeed:
        text = "setspeed " + param;
        break;
    case isAvailiable:
        text = "isavailiable";
        break;
    default:
        return;
    }
    QByteArray datagram = text.toLocal8Bit();
    udpSocket->writeDatagram(datagram, addr, serverPort);
}

void BackEnd::sendLeftSpeed() {
    char tmp[32];
    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "left %i", (int)m_leftDialValue);
    QString param = tmp;
    sendPacket(setSpeed, param, m_serverAddress, m_serverPort);
    return;
}

void BackEnd::sendRightSpeed() {
    char tmp[32];
    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "right %i", (int)m_rightDialValue);
    QString param = tmp;
    sendPacket(setSpeed, param, m_serverAddress, m_serverPort);
    return;
}

void BackEnd::readPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        processDatagram(datagram);
    }
}

void BackEnd::processDatagram(QNetworkDatagram datagram) {

}

