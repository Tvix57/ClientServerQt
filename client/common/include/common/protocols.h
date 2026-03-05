#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

namespace Common {

/**
 * @brief Типы сообщений в протоколе
 */
enum class MessageType {
    Handshake,
    Acknowledgment,
    Command,
    Configuration,
    NetworkMetrics,
    DeviceStatus,
    Log,
    Unknown
};

/**
 * @brief Структура для метрик сети
 */
struct NetworkMetrics {
    QString clientId;
    qint64 timestamp;
    double bandwidth;
    double latency;
    double packetLoss;
    double jitter;
    QString connectionType;
    int signalStrength;
    
    QJsonObject ToJson() const;
    static NetworkMetrics FromJson(const QJsonObject &json);
};

/**
 * @brief Структура для статуса устройства
 */
struct DeviceStatus {
    QString clientId;
    qint64 timestamp;
    int uptime;
    double cpuUsage;
    double memoryUsage;
    double temperature;
    QString ipAddress;
    bool hasWarning;
    QString warningMessage;
    
    QJsonObject ToJson() const;
    static DeviceStatus FromJson(const QJsonObject &json);
};

/**
 * @brief Структура для лога
 */
struct LogMessage {
    QString clientId;
    qint64 timestamp;
    QString severity;
    QString message;
    int messageId;
    
    QJsonObject ToJson() const;
    static LogMessage FromJson(const QJsonObject &json);
};

/**
 * @brief Вспомогательные функции для протокола
 */
class ProtocolHelper {
public:
    static MessageType GetMessageType(const QJsonObject &json);
    static QString MessageTypeToString(MessageType type);
    static QByteArray CreateHandshake(const QString &clientType, const QString &version);
    static QByteArray CreateAcknowledgment(const QString &status, const QString &message);
    static QByteArray CreateCommand(const QString &command);
    static QByteArray CreateConfiguration(const QJsonObject &config);
};

} // namespace Common

#endif // PROTOCOLS_H