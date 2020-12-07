#ifndef OPENSTREAMMAINWINDOW_H
#define OPENSTREAMMAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QSharedMemory>
#include <QProcess>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QInputDialog>

#include "auth_listener_worker.h"
#include "auth_pin_handler.h"


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

public slots:
    /**
     * @brief on_event_loop_started
     * Automatically starts the host for usability with
     * Auto start functionality when systems boot.
     */
    void on_event_loop_started();

protected:
    void closeEvent(QCloseEvent *) override;

signals:
    /**
     * @brief auth_finished is emitted when the authentication pin dialog
     * has been closed.
     * It doesn't matter if the pairing was succesful or not. After the dialog
     * has been closed, the named pipe will be waiting again.
     */
    void auth_finished();

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

    /**
     * @brief authListenerWorkerRegeneration is intended to be
     * executed after destruction of auth_listener_thread.
     * It will regenerate the IPC thread after a new client
     * has tried to be paired.
     */
    void authListenerWorkerRegeneration();

    /**
     * @brief showAuthMessagePopUp activates a pop-up message
     * when IPC communication is received from the streamming host
     */
    void showAuthMessagePopUp();

    /**
     * @brief inputAuthPinCapture opens a dialog for capture of
     * the authentication pin challenge
     */
    void inputAuthPinCapture();

    void onStartButtonClicked();

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

    /*Tray app related*/
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    /*Auth pin related*/
    AuthListenerWorker *auth_listener_worker;
    QThread *auth_listener_thread;
    void allocate_auth_listener();
    AuthPinHandler *auth_pin_handler;

};

#endif // OPENSTREAMMAINWINDOW_H
