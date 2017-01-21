#ifndef ENV_SENSORDLG_H
#define ENV_SENSORDLG_H

#include "env_sensetdlg.h"

namespace Ui {
class ENV_SensorDlg;
}

class ENV_SensorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ENV_SensorDlg(QWidget *parent = 0);
    ~ENV_SensorDlg();

    void updateData(void);

public slots:
    void InitFunction(void);
    void timeoutDone(void);

private slots:
    void on_setBtn_clicked();

private:
    Ui::ENV_SensorDlg *ui;

    QTimer *timer;
    NPM_sAlarm m_sensorData;
};

#endif // ENV_SENSORDLG_H
