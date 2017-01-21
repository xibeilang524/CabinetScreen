#ifndef WID_SENSOR_H
#define WID_SENSOR_H

#include <QWidget>

#include "wid_outputbit.h"

class WID_Sensor : public QWidget
{
    Q_OBJECT
public:
    explicit WID_Sensor(QWidget *parent = 0);

    void updateData(NPM_sAlarm &data);

protected:
    void initWidget(void);
    void addItemContent(int , int , QString &);
    void addRowContent(QStringList &list);
    void initTable(void);

    void updateWidget(void);
signals:
    
public slots:
    void timeoutDone(void);

private:
    QTableWidget *m_tableWidget;
    QGridLayout *layout;
    QVector<QTableWidgetItem *> m_itemVec;

    NPM_sAlarm m_sensor;
};

#endif // WID_SENSOR_H
