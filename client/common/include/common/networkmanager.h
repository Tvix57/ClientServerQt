#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <memory>

namespace Common {

/**
 * @brief Класс для управления сетевым подключением (общий для клиентов)
 */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    /**
     * @brief Подключение к серверу
     * @param host Адрес сервера
     * @param port Порт сервера
     */
    void ConnectToServer(const QString &host, quint16 port);

    /**
     * @brief Отключение от сервера
     */
    void Disconnect();

    /**
     * @brief Отправка данных на сервер
     * @param data Данные для отправки
     * @return true если данные успешно отправлены
     */
    bool SendData(const QByteArray &data);

    /**
     * @brief Проверка состояния подключения
     * @return true если подключено
     */
    bool IsConnected() const;

    /**
     * @brief Получение локального адреса
     */
    QHostAddress GetLocalAddress() const;

    /**
     * @brief Получение локального порта
     */
    quint16 GetLocalPort() const;

signals:
    void Connected();
    void Disconnected();
    void Error(const QString &errorString);
    void DataReceived(const QByteArray &data);
    void AcknowledgmentReceived(const QByteArray &data);

private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onReadyRead();

private:
    void parseIncomingData(const QByteArray &data);

private:
    std::unique_ptr<QTcpSocket> m_socket;
    QByteArray m_buffer;
    bool m_isConnected;
};

} // namespace Common

#endif // NETWORKMANAGER_H