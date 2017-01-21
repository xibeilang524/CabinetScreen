#ifndef WID_TABWIDGET_H
#define WID_TABWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include "wid_devinfo.h"
#include "wid_sensor.h"

class WID_TabWIdget : public QWidget
{
    Q_OBJECT
public:
    explicit WID_TabWIdget(QWidget *parent = 0);
    
    void updateWidget(NPM_sPDUData &pduData);

signals:
    
public slots:

protected:
    void initLayout(void);
    void initWidget(void);

private:
    QTabWidget *m_tabWidget;

    WID_OutputBit *m_output;
    WID_Sensor *m_sensor;
};

#endif // WID_TABWIDGET_H
