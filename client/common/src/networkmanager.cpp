#include "common/networkmanager.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace Common {

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_socket(std::make_unique<QTcpSocket>())
    , m_isConnected(false)
{
    connect(m_socket.get(), &QTcpSocket::connected,
            this, &NetworkManager::onConnected);
    connect(m_socket.get(), &QTcpSocket::disconnected,
            this, &NetworkManager::onDisconnected);
    connect(m_socket.get(), QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &NetworkManager::onErrorOccurred);
    connect(m_socket.get(), &QTcpSocket::readyRead,
            this, &NetworkManager::onReadyRead);
}

NetworkManager::~NetworkManager()
{
    Disconnect();
}

void NetworkManager::ConnectToServer(const QString &host, quint16 port)
{
    if (m_isConnected) {
        return;
    }

    m_socket->connectToHost(host, port);
}

void NetworkManager::Disconnect()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
}

bool NetworkManager::SendData(const QByteArray &data)
{
    if (!m_isConnected) {
        return false;
    }

    // Добавляем разделитель для сообщений
    QByteArray dataToSend = data + "\n";
    qint64 bytesWritten = m_socket->write(dataToSend);
    
    if (bytesWritten == -1) {
        emit Error("Failed to write data to socket");
        return false;
    }
    
    return m_socket->flush();
}

bool NetworkManager::IsConnected() const
{
    return m_isConnected;
}

QHostAddress NetworkManager::GetLocalAddress() const
{
    return m_socket->localAddress();
}

quint16 NetworkManager::GetLocalPort() const
{
    return m_socket->localPort();
}

void NetworkManager::onConnected()
{
    m_isConnected = true;
    emit Connected();
}

void NetworkManager::onDisconnected()
{
    m_isConnected = false;
    m_buffer.clear();
    emit Disconnected();
}

void NetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    QString errorString = m_socket->errorString();
    emit Error(errorString);
}

void NetworkManager::onReadyRead()
{
    m_buffer.append(m_socket->readAll());

    int endIndex;
    while ((endIndex = m_buffer.indexOf('\n')) != -1) {
        QByteArray message = m_buffer.left(endIndex);
        m_buffer.remove(0, endIndex + 1);
        parseIncomingData(message);
    }
}

void NetworkManager::parseIncomingData(const QByteArray &data)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
        QJsonObject obj = doc.object();
        if (obj.contains("type") && obj["type"].toString() == "acknowledgment") {
            emit AcknowledgmentReceived(data);
        } else {
            emit DataReceived(data);
        }
    } else {
        emit DataReceived(data);
    }
}

} // namespace Common