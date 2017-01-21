#ifndef OP_OUTPUTDLG_H
#define OP_OUTPUTDLG_H

#include <QDialog>
#include "op_tablewidget.h"

namespace Ui {
    class OP_OutputDlg;
}

class OP_OutputDlg : public QDialog
{
    Q_OBJECT

public:
    explicit OP_OutputDlg(QWidget *parent = 0);
    ~OP_OutputDlg();

protected slots:
    void InitFunction(void);
    void timeoutDone(void);

private:
    Ui::OP_OutputDlg *ui;
     QTimer *timer;
    OP_TableWidget *m_table_1, *m_table_2;
};

#endif // OP_OUTPUTDLG_H
