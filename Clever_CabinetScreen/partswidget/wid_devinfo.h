#ifndef WID_DEVINFO_H
#define WID_DEVINFO_H

#include <QWidget>
#include "mdb_corethread.h"
#include "wid_total.h"

class WID_DevInfo : public QWidget
{
    Q_OBJECT
public:
    explicit WID_DevInfo(QWidget *parent = 0);
    
    void updateData(NPM_sDevInfo &data);

protected:
    void createControl(void);
    void layoutControl(void);

    void updateWidget(void);
signals:
    
public slots:
    void timeoutDone(void);

private:
    QLabel *lab;
    QLabel *nameLab, *name;
    QLabel *typeLab, *type;
    QLabel *stateLab, *state;
    QLabel *ipLab, *ip;

    NPM_sDevInfo m_devInfo;
};

#endif // WID_DEVINFO_H
