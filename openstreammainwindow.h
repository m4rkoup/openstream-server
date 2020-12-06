#ifndef OPENSTREAMMAINWINDOW_H
#define OPENSTREAMMAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QSharedMemory>

namespace Ui {
class OpenstreamMainWindow;
}

class OpenstreamMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenstreamMainWindow(QWidget *parent = nullptr);
    void setVisible(bool visible) override;
    ~OpenstreamMainWindow();

protected:
    void closeEvent(QCloseEvent *) override;

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);


private:
    Ui::OpenstreamMainWindow *ui;

    /*Shared Memory Footprint functionality*/
    /*/**
     * is used to keep only
     * one instance of the program in the computer.
     */
    QString sharedMemoryFootprintKey = "OPEN STREAM";
    QSharedMemory *sharedMemoryFootprint;
    void allocateSharedMemoryFootprint();
    void sharedMemoryFootprintErrorMessage();

    /*TrayIcon functionality*/
    void createMainGroupBox();
    void createMinimalActions();
    void createTrayIcon();

    /*State indicator*/
    QString STATE_RUNNING_MSG_TRAY = "Openstream - Running";
    QString STATE_STOPPED_MSG_TRAY = "Openstream - Stopped";
    QIcon *icon_off;
    QIcon *icon_on;
    /*State indicator*/

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;


    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // OPENSTREAMMAINWINDOW_H
