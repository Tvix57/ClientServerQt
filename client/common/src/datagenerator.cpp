#include "common/datagenerator.h"
#include "common/constants.h"
#include "common/protocols.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QDateTime>

using namespace Common;

DataGenerator::DataGenerator(QObject *parent)
    : QObject(parent)
    , m_messageCounter(0)
{
    // Инициализация шаблонов логов
    m_logTemplates = {
        "Network interface %1 %2",
        "System process %1 %2 with PID %3",
        "Memory allocation %1 for process %2",
        "Disk %1 operation completed in %2 ms",
        "Temperature sensor %1 reading: %2°C",
        "Power supply %1 voltage: %2 V",
        "Fan %1 speed: %2 RPM",
        "Connection to %1 %2 with %3%% packet loss",
        "Service %1 %2 after %3 seconds uptime",
        "Configuration %1 applied to %2"
    };

    m_severityLevels = {
        Constants::SEVERITY_DEBUG,
        Constants::SEVERITY_INFO,
        Constants::SEVERITY_WARNING,
        Constants::SEVERITY_ERROR,
        Constants::SEVERITY_CRITICAL
    };

    // Пороговые значения по умолчанию
    m_thresholds = {
        {"max_cpu_usage", 80},
        {"max_memory_usage", 90},
        {"max_latency", 100},
        {"max_packet_loss", 5},
        {"max_temperature", 70}
    };
}

QByteArray DataGenerator::GenerateNetworkMetrics(const QString &clientId)
{
    Common::NetworkMetrics metrics;
    metrics.clientId = clientId;
    metrics.timestamp = QDateTime::currentSecsSinceEpoch();
    metrics.bandwidth = QRandomGenerator::global()->generateDouble() * 200;
    metrics.latency = QRandomGenerator::global()->generateDouble() * 200;
    metrics.packetLoss = QRandomGenerator::global()->generateDouble() * 10;
    
    // Дополнительные поля для длинных сообщений
    if (determineMessageLength() > Constants::MEDIUM_MESSAGE_MAX) {
        metrics.jitter = QRandomGenerator::global()->generateDouble() * 50;
        metrics.connectionType = QString("WiFi");
        metrics.signalStrength = QRandomGenerator::global()->bounded(-90, -30);
    }
    
    return QJsonDocument(metrics.ToJson()).toJson();
}

QByteArray DataGenerator::GenerateDeviceStatus(const QString &clientId)
{
    Common::DeviceStatus status;
    status.clientId = clientId;
    status.timestamp = QDateTime::currentSecsSinceEpoch();
    status.uptime = QRandomGenerator::global()->bounded(3600, 86400 * 7);
    status.cpuUsage = QRandomGenerator::global()->generateDouble() * 100;
    status.memoryUsage = QRandomGenerator::global()->generateDouble() * 100;
    status.temperature = 30 + QRandomGenerator::global()->generateDouble() * 40;
    status.ipAddress = generateRandomIp();
    
    // Проверка порогов
    if (status.cpuUsage > m_thresholds["max_cpu_usage"].toInt() ||
        status.memoryUsage > m_thresholds["max_memory_usage"].toInt() ||
        status.temperature > m_thresholds["max_temperature"].toInt()) {
        status.hasWarning = true;
        status.warningMessage = "Resource usage exceeds threshold";
    }
    
    return QJsonDocument(status.ToJson()).toJson();
}

QByteArray DataGenerator::GenerateLog(const QString &clientId)
{
    Common::LogMessage log;
    log.clientId = clientId;
    log.timestamp = QDateTime::currentSecsSinceEpoch();
    log.severity = generateRandomSeverity();
    log.messageId = ++m_messageCounter;
    log.message = generateRandomLogMessage(determineMessageLength());
    
    return QJsonDocument(log.ToJson()).toJson();
}

void DataGenerator::UpdateThresholds(const QJsonObject &thresholds)
{
    m_thresholds = thresholds;
}

QString DataGenerator::generateRandomIp() const
{
    return QString("%1.%2.%3.%4")
        .arg(QRandomGenerator::global()->bounded(1, 255))
        .arg(QRandomGenerator::global()->bounded(0, 255))
        .arg(QRandomGenerator::global()->bounded(0, 255))
        .arg(QRandomGenerator::global()->bounded(1, 255));
}

QString DataGenerator::generateRandomLogMessage(int length) const
{
    QString template_str = m_logTemplates[
        QRandomGenerator::global()->bounded(m_logTemplates.size())];
    
    QString message = template_str
        .arg(QString("eth%1").arg(QRandomGenerator::global()->bounded(0, 5)))
        .arg(QStringList{"started", "stopped", "failed", "restarted", "initialized"}[
            QRandomGenerator::global()->bounded(5)])
        .arg(QRandomGenerator::global()->bounded(1000, 9999));
    
    // Добавляем случайные слова для увеличения длины
    QStringList words = {"system", "network", "device", "service", "process", 
                        "application", "daemon", "interface", "protocol", "packet",
                        "buffer", "socket", "connection", "authentication", "encryption"};
    
    while (message.length() < length) {
        message += " " + words[QRandomGenerator::global()->bounded(words.size())];
    }
    
    return message;
}

QString DataGenerator::generateRandomSeverity() const
{
    return m_severityLevels[
        QRandomGenerator::global()->bounded(m_severityLevels.size())];
}

int DataGenerator::determineMessageLength() const
{
    int type = QRandomGenerator::global()->bounded(3);
    switch (type) {
        case 0: // Короткое
            return QRandomGenerator::global()->bounded(20, Constants::SHORT_MESSAGE_MAX);
        case 1: // Среднее
            return QRandomGenerator::global()->bounded(
                Constants::SHORT_MESSAGE_MAX, Constants::MEDIUM_MESSAGE_MAX);
        case 2: // Длинное
            return QRandomGenerator::global()->bounded(
                Constants::MEDIUM_MESSAGE_MAX, 500);
        default:
            return Constants::SHORT_MESSAGE_MAX;
    }
}
