#include <QApplication>
#include <QTime>
#include <sstream>
#include <iostream>
#include <boost/asio.hpp>
#include "license-session.h"
#include "mainwindow.h"

using io_service = boost::asio::io_service;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
