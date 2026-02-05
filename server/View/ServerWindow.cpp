#include "ServerWindow.hpp"

#include <iostream>

ServerWindow::ServerWindow(QObject *parent)
    : QObject(parent)
{
}


void ServerWindow::addClient() {


    emit addRow();
}

void ServerWindow::startServer() {
    this->_isRunning = true;
    this->_serverStatus = "OnLine";
    std::cout << "startServer" << std::endl;
    emit statusChanged();
}

void ServerWindow::stopServer() {
    this->_isRunning = false;
    this->_serverStatus = "OffLine";
    std::cout << "stopServer" << std::endl;
    emit statusChanged();
}
