#ifndef OPENSTREAMMAINWINDOW_H
#define OPENSTREAMMAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QSharedMemory>
#include <QProcess>


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
    void startSunshine();
    void stopSunshine();
    void updateAppConsole();
    void updateAppConsoleError();
    void appStoppedWatch();
    void stopHostBeforeClose();
    void set_off_host_state_indicator();
    void set_on_host_state_indicator();

private:
    Ui::OpenstreamMainWindow *ui;
    QString *SUNSHINE_CONF = new QString("/assets/sunshine.conf");


    //QProces to handle the Sunshine underneath binary executable.
    QProcess *proc;
    void allocateNewProcess();

    /*Private API for start/stop*/
    void startSunshineApp();
    void appStart();
    void appStarting();
    void appRunning();

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
    QString STATE_RUNNING_MSG = "Server Up";
    QString STATE_STOPPED_MSG = "Server Down";
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
