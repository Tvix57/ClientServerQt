#ifndef CONSOLECLIENT_H
#define CONSOLECLIENT_H

#include <QObject>
#include <QTimer>
#include <memory>
#include "common/networkmanager.h"
#include "common/datagenerator.h"

/**
 * @brief Главный класс консольного клиента
 */
class ConsoleClient : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleClient(QObject *parent = nullptr);
    ~ConsoleClient();

    /**
     * @brief Запуск клиента
     */
    void Start();

    /**
     * @brief Остановка клиента
     */
    void Stop();

signals:
    void LogMessage(const QString &message);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(const QString &errorString);
    void onDataReceived(const QByteArray &data);
    void onAcknowledgmentReceived(const QByteArray &data);
    void onSendData();

private:
    void attemptConnection();
    void handleServerResponse(const QJsonObject &jsonObj);
    void handleCommand(const QString &command);
    void handleConfiguration(const QJsonObject &config);

private:
    std::unique_ptr<Common::NetworkManager> m_networkManager;
    std::unique_ptr<DataGenerator> m_dataGenerator;
    QTimer m_reconnectTimer;
    QTimer m_dataSendTimer;
    QString m_clientId;
    bool m_isRunning;
    bool m_isReady;
    bool m_isSending;
};

#endif // CONSOLECLIENT_H
