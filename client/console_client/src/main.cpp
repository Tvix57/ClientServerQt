#include <QCoreApplication>
#include <QTextStream>
#include <QTimer>
#include <QTime>
#include <memory>
#include <csignal>
#include "console_client/consoleclient.h"

static std::unique_ptr<ConsoleClient> g_client;
static QTextStream g_out(stdout);

void signalHandler(int signal) {
    g_out << "\nReceived signal " << signal << ", shutting down...\n";
    
    if (g_client) {
        g_client->Stop();
    }
    
    QCoreApplication::quit();
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    QCoreApplication::setApplicationName("Console Client");
    QCoreApplication::setApplicationVersion("1.0.0");
    
    g_out << "====================================\n";
    g_out << "  Console Client v" << QCoreApplication::applicationVersion() << "\n";
    g_out << "====================================\n\n";
    
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    g_client = std::make_unique<ConsoleClient>();
    
    QObject::connect(g_client.get(), &ConsoleClient::LogMessage,
        [](const QString &message) {
            g_out << "[" << QTime::currentTime().toString("hh:mm:ss.zzz") << "] "
                  << message << "\n";
            g_out.flush();
        });
    
    g_client->Start();
    
    g_out << "Client started. Press Ctrl+C to stop.\n\n";
    
    int result = app.exec();
    
    g_client.reset();
    g_out << "Client terminated.\n";
    
    return result;
}
