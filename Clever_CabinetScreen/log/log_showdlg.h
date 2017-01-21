#ifndef LOG_SHOWDLG_H
#define LOG_SHOWDLG_H

#include <QDialog>
#include "log_tableview.h"

namespace Ui {
    class LOG_ShowDlg;
}

class LOG_ShowDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LOG_ShowDlg(QWidget *parent = 0);
    ~LOG_ShowDlg();

    void initRecordTable(void);
private:
    Ui::LOG_ShowDlg *ui;

    LOG_TableView *recordTable;
};

#endif // LOG_SHOWDLG_H
