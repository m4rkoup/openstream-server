#include "openstreammainwindow.h"
#include "ui_openstreammainwindow.h"

#include <QCloseEvent>

OpenstreamMainWindow::OpenstreamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenstreamMainWindow)
{
    ui->setupUi(this);
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

void OpenstreamMainWindow::setVisible(bool visible) {
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QWidget::setVisible(visible);

}

void OpenstreamMainWindow::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}
