#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include "json.hpp"
#include "license-session.h"
#include "mdi-child.h"

namespace Ui {
class MainWindow;
}
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    Ui::MainWindow *ui;
    boost::asio::io_service m_ioService;
    int timerId;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::vector<MdiChild*> m_childs; ///< depre
    std::vector<boost::weak_ptr<LicenseSession>> m_sessions; ///< depre

    void timerEvent(QTimerEvent *event)
    {
        m_ioService.poll();
    }

    void start_accept();
    void handle_accept(typename LicenseSession::pointer new_session,
                       const boost::system::error_code& error);

private slots:

};

#endif // MAINWINDOW_H
