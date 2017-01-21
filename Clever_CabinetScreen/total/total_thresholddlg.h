#ifndef TOTAL_THRESHOLDDLG_H
#define TOTAL_THRESHOLDDLG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include "total_thresholdset.h"

namespace Ui {
    class TOTAL_ThresholdDlg;
}

class TOTAL_ThresholdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TOTAL_ThresholdDlg(QWidget *parent = 0);
    ~TOTAL_ThresholdDlg();

protected:
    void addItemContent(int row, int column, QString &content);
    void addRowContent(QStringList &list);
    void initTableWidget(void);
    void updateTable(void);
    void checkUpdateTable(void);

    void setCurValue(void);
    void slaveRecord(void);

    void checkAlarm(int line, QTableWidgetItem *item);
private slots:
    void InitFunction(void);
    void timeoutDone(void);
    void on_setThresholdBtn_clicked();

    void on_saveBtn_clicked();

    void on_refreshBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::TOTAL_ThresholdDlg *ui;

    bool m_initFlag;
    QTimer *timer;
    NPM_sTotalData m_totalData;
    QVector<QTableWidgetItem *> m_itemVec;

    QVector <short> m_tcurMin; // 总电流下限
    QVector <short> m_tcurMax; // 总电流上限
};
extern void db_record_save(QString &type, QString &msg);

#endif // TOTAL_THRESHOLDDLG_H
