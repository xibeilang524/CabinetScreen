#ifndef ENV_SENSETDLG_H
#define ENV_SENSETDLG_H

#include <QDialog>
#include "env_tablewidget.h"

namespace Ui {
    class ENV_SenSetDlg;
}

class ENV_SenSetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ENV_SenSetDlg(QWidget *parent = 0);
    ~ENV_SenSetDlg();

    void changeData(int, short min, short max);
    void setData(void);

public slots:
    void timeoutDone(void);
    void setSlot(int);

private slots:
    void on_refreshBtn_clicked();

    void on_saveBtn_clicked();

    void on_quitBtn_clicked();

    void on_tempBtn_clicked();

    void on_humBtn_clicked();

private:
    Ui::ENV_SenSetDlg *ui;

    QTimer *timer;
    ENV_TableWidget *m_envTable;

    QVector <short> tempMin; // 温度下限
    QVector <short> humMin; //  湿度下限
    QVector <short> tempMax; // 温度上限
    QVector <short> humMax; // 湿度上限

};

#endif // ENV_SENSETDLG_H
