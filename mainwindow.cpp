#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "json.hpp"
#include <iostream>
using nlohmann::json;
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTimer>
#include "mdi-child.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    acceptor_(m_ioService)
{
    ui->setupUi(this);
    for(int i=0; i<2; i++) {
    }

    timerId = startTimer(1);
    acceptor_.open(tcp::v4());
    int one = 1;
    setsockopt(acceptor_.native_handle(), SOL_SOCKET, SO_REUSEADDR/* | SO_REUSEPORT*/, &one, sizeof(one));
    acceptor_.bind(tcp::endpoint(tcp::v4(), 1000));
    acceptor_.listen();

    start_accept();
    // resolving
}

void MainWindow::start_accept()
{
    auto new_session = LicenseSession::create(m_ioService);
    printf("try accept\n");
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&MainWindow::handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
}
void MainWindow::handle_accept(typename LicenseSession::pointer new_session,
                               const boost::system::error_code& error)
{
    printf("SetupSession is connected\n");
    if (!error) {
        MdiChild* mc = new MdiChild();
        mc->m_session = new_session;
        m_childs.push_back(mc);
        ui->horizontalLayout->addWidget(mc);
        m_sessions.push_back(boost::weak_ptr<LicenseSession>());
        new_session->m_mdiChild = mc;
        new_session->start();

        // for(int i=0; i<4; i++) {
        //     if(!m_childs[i]->isConnected()) {
        //         m_childs[i]->m_session = new_session;
        //         m_sessions[i] = new_session;
        //         new_session->m_mdiChild = m_childs[i];
        //         // m_childs[i]->onConnection();
        //         // , // Can also use any other except DirectConnection
        //         //                           Q_ARG(QByteArray, qa)); // And some more args if needed
        //         break;
        //     }
        // }
    }
    else {
        printf("delete %x\n", new_session.get());
        std::cout << error.message() << std::endl;
    }

    start_accept();
}
MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;
}
