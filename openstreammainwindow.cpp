#include "openstreammainwindow.h"
#include "ui_openstreammainwindow.h"

OpenstreamMainWindow::OpenstreamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenstreamMainWindow)
{
    ui->setupUi(this);
    ui->main_area_console_output->setReadOnly(true);

    allocateSharedMemoryFootprint();
    createMinimalActions();
    createTrayIcon();
    allocateNewProcess();

    /*Auth pin handling related*/
    auth_pin_handler = new AuthPinHandler();
    allocate_auth_listener();

    /*Connections*/
    connect(ui->start_button,
            &QAbstractButton::clicked,
            this,
            &OpenstreamMainWindow::startSunshine);
    connect(ui->stop_button,
            &QAbstractButton::clicked,
            this,
            &OpenstreamMainWindow::stopSunshine);
    connect(trayIcon,
            &QSystemTrayIcon::activated,
            this,
            &OpenstreamMainWindow::trayIconActivated);
    connect(this,
            &OpenstreamMainWindow::auth_finished,
            this,
            &OpenstreamMainWindow::authListenerWorkerRegeneration);
    //This restores de QTrayApp when the notification is clicked
    connect(trayIcon,
            &QSystemTrayIcon::messageClicked,
            this,
            &QWidget::showNormal);
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
   connect(quitAction, &QAction::triggered, this, &OpenstreamMainWindow::stopHostBeforeClose);
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

void OpenstreamMainWindow::allocateNewProcess() {
    proc = new QProcess;
}

void OpenstreamMainWindow::startSunshine() {
    switch(proc->state()) {
        case QProcess::NotRunning:
            appStart();
            break;
        case QProcess::Starting:
            appStarting();
            break;
        case QProcess::Running:
            appRunning();
            break;
        default:
            qDebug() << "Unknown host state" << Qt::endl;
    }
}

void OpenstreamMainWindow::stopSunshine() {
    qDebug() << "Stoping sunshine process...";
    qint64 pid = proc->processId();
    if(pid > 0) {
        //TODO: research use of kill vs terminate
        proc->kill();
        //Allocate new process
        delete proc;
        allocateNewProcess();
        qDebug() << "Process host stopped " << pid << Qt::endl;
    }
    else {
        qDebug() << "Process currently stopped.";
    }
}

void OpenstreamMainWindow::appStart() {
    qDebug() << "Start application" << Qt::endl;
    QString app_dir = QCoreApplication::applicationDirPath();
    connect(proc, &QProcess::readyReadStandardOutput, this, &OpenstreamMainWindow::updateAppConsole);
    connect(proc, &QProcess::readyRead, this, &OpenstreamMainWindow::updateAppConsole);
    connect(proc, &QProcess::readyReadStandardError, this, &OpenstreamMainWindow::updateAppConsoleError);
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &OpenstreamMainWindow::appStoppedWatch);
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &OpenstreamMainWindow::set_off_host_state_indicator);
    proc->open(QProcess::Unbuffered);
    proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << app_dir + *SUNSHINE_CONF);
    this->set_on_host_state_indicator();
    qDebug() << "Application started" << Qt::endl;
}

void OpenstreamMainWindow::appStarting() {
    qDebug() << "A sunshine host is currently starting" << Qt::endl;
    QMessageBox::information(this, "Sunshine Launcher",
                             tr("A sunshine host is currently starting"));
}

void OpenstreamMainWindow::appRunning() {
    qDebug() << "A sunshine host was already started" << Qt::endl;
    QMessageBox::information(this, "Sunshine Launcher",
                             tr("A sunshine host was already started"));
}

void OpenstreamMainWindow::set_on_host_state_indicator() {
    ui->main_area_status_label->setText(STATE_RUNNING_MSG);
    trayIcon->setIcon(*icon_on);
    trayIcon->setToolTip(STATE_RUNNING_MSG);
}

void OpenstreamMainWindow::set_off_host_state_indicator() {
    ui->main_area_status_label->setText(STATE_STOPPED_MSG);
    trayIcon->setIcon(*icon_off);
    trayIcon->setToolTip(STATE_STOPPED_MSG);
}

void OpenstreamMainWindow::updateAppConsole() {
    QByteArray strData = proc->readAllStandardOutput();
    ui->main_area_console_output->moveCursor (QTextCursor::End);
    ui->main_area_console_output->insertPlainText(strData);
}

void OpenstreamMainWindow::updateAppConsoleError()
{
    QByteArray strData = proc->readAllStandardError();
    ui->main_area_console_output->moveCursor (QTextCursor::End);
    ui->main_area_console_output->append(strData);
}

void OpenstreamMainWindow::appStoppedWatch() {
    qDebug() << "Streamming host was stopped" << Qt::endl;
}

void OpenstreamMainWindow::stopHostBeforeClose() {
    this->stopSunshine();
}

void OpenstreamMainWindow::on_event_loop_started() {
    appStart();
}


void OpenstreamMainWindow::allocate_auth_listener() {
    auth_listener_thread = new QThread();
    auth_listener_worker = new AuthListenerWorker();
    auth_listener_worker->moveToThread(auth_listener_thread);
    connect(auth_listener_worker,
            &AuthListenerWorker::auth_attempt,
            this,
            &OpenstreamMainWindow::showAuthMessagePopUp);
    connect(auth_listener_worker,
            &AuthListenerWorker::auth_attempt,
            this,
            &OpenstreamMainWindow::inputAuthPinCapture);
    connect(auth_listener_thread,
            &QThread::started,
            auth_listener_worker,
            &AuthListenerWorker::start_listening);
    connect(auth_listener_worker,
            &AuthListenerWorker::finished,
            auth_listener_thread,
            &QThread::quit);
    connect(auth_listener_worker,
            &AuthListenerWorker::finished,
            auth_listener_worker,
            &QObject::deleteLater);
    connect(auth_listener_thread,
            &QThread::finished,
            auth_listener_thread,
            &QThread::deleteLater);
    auth_listener_thread->start();
}

void OpenstreamMainWindow::authListenerWorkerRegeneration() {
    allocate_auth_listener();
}

void OpenstreamMainWindow::inputAuthPinCapture() {
    bool ok;
    QString pin = QInputDialog::getText(this, tr("Introduce PIN"),
                                        tr("PIN:"), QLineEdit::Normal,
                                        "", &ok);
    if(ok) {
        qDebug() << "PIN captured!: " << pin << Qt::endl;
        auth_pin_handler->send_host_pin(pin);
    }
    else {
        qDebug() << "NO PIN captured! :( " << pin << Qt::endl;
    }

    emit auth_finished(); /*Restart named pipe listening*/
}

void OpenstreamMainWindow::showAuthMessagePopUp() {
    QString title = QString(tr("Client is trying to connect"));
    QString body = QString(tr("A client wants to use openstream.\n"
                              "Click this message for introduce your pin"));

    trayIcon->showMessage(title, body, trayIcon->icon(), 60 * 100000);
}

