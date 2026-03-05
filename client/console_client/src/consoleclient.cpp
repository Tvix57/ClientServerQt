#include "console_client/consoleclient.h"
#include "common/constants.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QThread>

using namespace Common;

ConsoleClient::ConsoleClient(QObject *parent)
    : QObject(parent)
    , m_networkManager(std::make_unique<NetworkManager>())
    , m_dataGenerator(std::make_unique<DataGenerator>())
    , m_isRunning(false)
    , m_isReady(false)
    , m_isSending(false)
{
    // Генерация уникального ID клиента
    m_clientId = QString("DEVICE_%1").arg(
        QRandomGenerator::global()->bounded(1000, 9999));

    // Подключение сигналов от NetworkManager
    connect(m_networkManager.get(), &NetworkManager::Connected,
            this, &ConsoleClient::onConnected);
    connect(m_networkManager.get(), &NetworkManager::Disconnected,
            this, &ConsoleClient::onDisconnected);
    connect(m_networkManager.get(), &NetworkManager::Error,
            this, &ConsoleClient::onError);
    connect(m_networkManager.get(), &NetworkManager::DataReceived,
            this, &ConsoleClient::onDataReceived);
    connect(m_networkManager.get(), &NetworkManager::AcknowledgmentReceived,
            this, &ConsoleClient::onAcknowledgmentReceived);

    // Настройка таймера переподключения
    m_reconnectTimer.setInterval(Constants::RECONNECT_INTERVAL_MS);
    m_reconnectTimer.setSingleShot(false);
    connect(&m_reconnectTimer, &QTimer::timeout, this, &ConsoleClient::attemptConnection);

    // Настройка таймера отправки данных
    m_dataSendTimer.setSingleShot(true);
    connect(&m_dataSendTimer, &QTimer::timeout, this, &ConsoleClient::onSendData);
}

ConsoleClient::~ConsoleClient()
{
    Stop();
}

void ConsoleClient::Start()
{
    if (m_isRunning) {
        emit LogMessage("Client is already running");
        return;
    }

    m_isRunning = true;
    m_isReady = false;
    m_isSending = false;
    emit LogMessage(QString("Starting client [%1]...").arg(m_clientId));
    
    attemptConnection();
    m_reconnectTimer.start();
}

void ConsoleClient::Stop()
{
    if (!m_isRunning) {
        return;
    }

    m_isRunning = false;
    m_isReady = false;
    m_isSending = false;
    
    m_reconnectTimer.stop();
    m_dataSendTimer.stop();
    
    if (m_networkManager->IsConnected()) {
        m_networkManager->Disconnect();
    }
    
    emit LogMessage("Client stopped");
}

void ConsoleClient::attemptConnection()
{
    if (!m_isRunning || m_networkManager->IsConnected()) {
        return;
    }

    emit LogMessage("Attempting to connect to server at localhost:" + QString::number(Constants::DEFAULT_SERVER_PORT) + "...");
    m_networkManager->ConnectToServer("localhost", Constants::DEFAULT_SERVER_PORT);
}

void ConsoleClient::onConnected()
{
    if (!m_isRunning) return;

    emit LogMessage("Connected to server successfully!");
    m_reconnectTimer.stop();
    
    // Отправляем handshake
    // QByteArray handshake = ProtocolHelper::CreateHandshake(
    //     Constants::CLIENT_TYPE_DEVICE,
    //     "1.0.0"
    // );
    
    // if (m_networkManager->SendData(handshake)) {
    //     emit LogMessage("Sent handshake to server");
    // }
}

void ConsoleClient::onDisconnected()
{
    emit LogMessage("Disconnected from server");
    m_isReady = false;
    m_isSending = false;
    m_dataSendTimer.stop();
    
    if (m_isRunning) {
        emit LogMessage("Will attempt to reconnect in " + 
                       QString::number(Constants::RECONNECT_INTERVAL_MS / 1000) + " seconds...");
        m_reconnectTimer.start();
    }
}

void ConsoleClient::onError(const QString &errorString)
{
    emit LogMessage("Connection error: " + errorString);
}

void ConsoleClient::onDataReceived(const QByteArray &data)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        emit LogMessage("Failed to parse server data: " + parseError.errorString());
        return;
    }

    if (!doc.isObject()) {
        emit LogMessage("Invalid data format");
        return;
    }

    handleServerResponse(doc.object());
}

void ConsoleClient::onAcknowledgmentReceived(const QByteArray &data)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        emit LogMessage("Failed to parse acknowledgment: " + parseError.errorString());
        return;
    }

    if (!doc.isObject()) {
        emit LogMessage("Invalid acknowledgment format");
        return;
    }

    QJsonObject jsonObj = doc.object();
    QString status = jsonObj["status"].toString();
    QString message = jsonObj["message"].toString();
    
    emit LogMessage("Server acknowledgment: " + status + " - " + message);
    
    if (status == "connected") {
        m_isReady = true;
        emit LogMessage("Ready to send data. Waiting for start command...");
    }
}

void ConsoleClient::handleServerResponse(const QJsonObject &jsonObj)
{
    // MessageType type = ProtocolHelper::GetMessageType(jsonObj);
    
    // switch (type) {
    //     case MessageType::Command:
    //         handleCommand(jsonObj["command"].toString());
    //         break;
            
    //     case MessageType::Configuration:
    //         handleConfiguration(jsonObj["config"].toObject());
    //         break;
            
    //     default:
    //         emit LogMessage("Received unknown message type: " +
    //                       ProtocolHelper::MessageTypeToString(type));
    //         break;
    // }
}

void ConsoleClient::handleCommand(const QString &command)
{
    if (command == Constants::CMD_START) {
        if (!m_isReady) {
            emit LogMessage("Received start command before acknowledgment!");
            return;
        }
        if (!m_isSending) {
            m_isSending = true;
            emit LogMessage("Received START command. Beginning data transmission...");
            onSendData();  // Начинаем отправку
        }
    }
    else if (command == Constants::CMD_STOP) {
        if (m_isSending) {
            m_isSending = false;
            m_dataSendTimer.stop();
            emit LogMessage("Received STOP command. Stopping data transmission...");
        }
    }
    else if (command == Constants::CMD_STATUS) {
        emit LogMessage(QString("Status: running=%1, ready=%2, sending=%3, connected=%4")
                       .arg(m_isRunning).arg(m_isReady).arg(m_isSending)
                       .arg(m_networkManager->IsConnected()));
    }
}

void ConsoleClient::handleConfiguration(const QJsonObject &config)
{
    // m_dataGenerator->UpdateThresholds(config);
    emit LogMessage("Updated configuration from server");
}

void ConsoleClient::onSendData()
{
    if (!m_isRunning || !m_isReady || !m_isSending || !m_networkManager->IsConnected()) {
        return;
    }

    // Генерация случайного типа данных
    int dataType = QRandomGenerator::global()->bounded(3);
    QByteArray data;
    QString dataTypeStr;

    // switch (dataType) {
    //     case 0:
    //         data = m_dataGenerator->GenerateNetworkMetrics(m_clientId);
    //         dataTypeStr = Constants::MSG_TYPE_NETWORK_METRICS;
    //         break;
    //     case 1:
    //         data = m_dataGenerator->GenerateDeviceStatus(m_clientId);
    //         dataTypeStr = Constants::MSG_TYPE_DEVICE_STATUS;
    //         break;
    //     case 2:
    //         data = m_dataGenerator->GenerateLog(m_clientId);
    //         dataTypeStr = Constants::MSG_TYPE_LOG;
    //         break;
    // }

    if (m_networkManager->SendData(data)) {
        emit LogMessage(QString("Sent %1 data (%2 bytes)")
                       .arg(dataTypeStr).arg(data.size()));
    } else {
        emit LogMessage("Failed to send data");
    }

    // Случайный интервал до следующей отправки
    int nextInterval = QRandomGenerator::global()->bounded(
        Constants::MIN_SEND_INTERVAL_MS,
        Constants::MAX_SEND_INTERVAL_MS + 1);
    
    if (m_isRunning && m_isReady && m_isSending && m_networkManager->IsConnected()) {
        m_dataSendTimer.start(nextInterval);
    }
}
