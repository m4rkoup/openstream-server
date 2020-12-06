#include "openstreammainwindow.h"
#include "ui_openstreammainwindow.h"

#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>

OpenstreamMainWindow::OpenstreamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenstreamMainWindow)
{
    ui->setupUi(this);
    allocateSharedMemoryFootprint();
    createMinimalActions();
    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &OpenstreamMainWindow::trayIconActivated);

    //This restores de QTrayApp when the notification is clicked
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &QWidget::showNormal);
    setWindowTitle(tr("Open Stream"));

    icon_off = new QIcon(":/images/joystick.png");
    icon_on = new QIcon(":/images/joystick_on.png");
    trayIcon->setIcon(*icon_off);
    trayIcon->show();
}

OpenstreamMainWindow::~OpenstreamMainWindow()
{
    delete ui;
}

/**
 * @brief OpenstreamMainWindow::createMinimalActions
 * creates the minimal actions for the Tray application popup menu.
 */
void OpenstreamMainWindow::createMinimalActions() {
   this->minimizeAction = new QAction(tr("Mi&nimize"), this);
   connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

   maximizeAction = new QAction(tr("Ma&ximize"), this);
   connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

   restoreAction = new QAction(tr("&Restore"), this);
   connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

   quitAction = new QAction(tr("&Quit"), this);
   //TODO: Adapt this line to the new functionality
   //connect(quitAction, &QAction::triggered, this, &Launcher::stopHostBeforeClose);
   connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

/**
 * @brief OpenstreamMainWindow::createTrayIcon
 * creates the Tray icon object for tray functionality.
 * Should be called after createMinimalActions.
 */
void OpenstreamMainWindow::createTrayIcon() {
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIconMenu->setStyleSheet(
                "QMenu"
                "{"
                "background-color: #979dac;"
                "font: \"Open Sans Semibold\";"
                "}"
                );
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(STATE_STOPPED_MSG_TRAY);
}

/**
 * @brief OpenstreamMainWindow::trayIconActivated
 * deals with user mouse interactions
 * @param reason
 */
void OpenstreamMainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
        case QSystemTrayIcon::DoubleClick:
            this->showNormal();
            break;
        default:
            //Empty by now. Default behaviour for all clicks.
        ;
    }
}

/**
 * @brief OpenstreamMainWindow::setVisible
 * overriden setVisible for tray application.
 * @param visible
 */
void OpenstreamMainWindow::setVisible(bool visible) {
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QWidget::setVisible(visible);

}

/**
 * @brief OpenstreamMainWindow::closeEvent
 * overriden closeEvent for tray application.
 * @param event
 */
void OpenstreamMainWindow::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

/**
 * @brief OpenstreamMainWindow::allocateSharedMemoryFootprint
 * Checks if another application is running with the same memory footprint.
 */
void OpenstreamMainWindow::allocateSharedMemoryFootprint() {
    sharedMemoryFootprint = new QSharedMemory(sharedMemoryFootprintKey, this);
    bool semaphore = sharedMemoryFootprint->create(4);
    if(semaphore) {
        qDebug() << "Starting new application. Not previous footprint found." << endl;
    }
    else {
        switch(sharedMemoryFootprint->error())
        {
            case QSharedMemory::InvalidSize:
                qDebug() << "Invalid footprint size" << endl;
                break;
           case QSharedMemory::QSharedMemory::KeyError:
                qDebug() << "Invalid footprint key" << endl;
                break;
           case QSharedMemory::AlreadyExists:
                qDebug() << "An instance of Sunshine Launcher is already running."
                         << "Closing current instance." << endl;
                sharedMemoryFootprintErrorMessage();
                break;
           default:
                qDebug() << "Unknown error found creating shared memory footprint" << endl;
        }

    }
}

void OpenstreamMainWindow::sharedMemoryFootprintErrorMessage() {
    if(QMessageBox::Ok == QMessageBox::critical(this,
                                                tr("Application Error"),
                                                tr("Sunshine launcher is already executing.")))
    {
        exit(EXIT_FAILURE);
    }
}
