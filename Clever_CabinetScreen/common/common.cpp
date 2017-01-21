#include "common.h"
#include <QIcon>

void com_setBackColour(const QString &str,QWidget *target)
{
    target->setWindowTitle(str);
    //target->setWindowFlags(Qt::FramelessWindowHint);
    target->setAutoFillBackground(true) ;
    QPalette paletteTool = target->palette() ;
    QBrush brushToolPic(QColor(WIDGET_BACK_COLOR)) ;
    QBrush brushToolwhite(QColor(BUTTON_BACK_COLOR)) ;
    paletteTool.setBrush(QPalette::Window,brushToolPic) ;
    paletteTool.setBrush(QPalette::Base,brushToolPic) ;
    paletteTool.setBrush(QPalette::Button,brushToolwhite) ;
    target->setPalette(paletteTool) ;

    target->setStyleSheet(BTN_FOCUS_BACK_COLOR);
    target->setWindowIcon(QIcon(":/image/logo.jpg"));
}
