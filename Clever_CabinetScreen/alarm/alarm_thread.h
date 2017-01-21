#ifndef ALARM_THREAD_H
#define ALARM_THREAD_H

#include <QThread>
#include "mdb_npmvtype.h"

class ALARM_Thread : public QThread
{
    Q_OBJECT
public:
    explicit ALARM_Thread(QObject *parent = 0);
    ~ALARM_Thread();

    void checkAlarm(void);
    void outputALarm(QVector <short> &);
    void totalCurAlarm(QVector <short> &);

protected:
    void run(void);

signals:

public slots:

private:
    bool m_isRun;
};
extern NPM_sPDUData *npm_get_pduData(void);
extern void db_record_save(QString &type, QString &msg);
#endif // ALARM_THREAD_H
