#ifndef OPENSTREAMMAINWINDOW_H
#define OPENSTREAMMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class OpenstreamMainWindow;
}

class OpenstreamMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenstreamMainWindow(QWidget *parent = nullptr);

    ~OpenstreamMainWindow();

private:
    Ui::OpenstreamMainWindow *ui;
};

#endif // OPENSTREAMMAINWINDOW_H
