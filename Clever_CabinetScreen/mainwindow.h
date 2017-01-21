#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wid_tabwidget.h"
#include "alarm_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
          
    void initWindow(void);
    void connectStatus(void);

signals:

public slots:
    void timeoutDone(void);

private slots:

    void on_thresholdBtn_clicked();

    void on_envBtn_clicked();

    void on_totalBtn_clicked();

    void on_outputBtn_clicked();

    void on_logBtn_clicked();

    void on_timeBtn_clicked();

private:
    Ui::MainWindow *ui;
     QTimer *timer;

    MDB_CoreThread *mdb;
    ALARM_Thread *m_alarm;
};

#endif // MAINWINDOW_H
