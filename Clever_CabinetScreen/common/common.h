#ifndef COMMON_H
#define COMMON_H
#include <QtCore>
#include <QWidget>

#define ARM_LINUX  1

/* 窗口背景色 */
#define     WIDGET_BACK_COLOR   232,249,255
#define     BUTTON_BACK_COLOR  232,238,250

#define     DILAG_BACK_COLOR      85,152,215
#define     DILAG_BUTTON_COLOR  255,255,255

/* 获得焦点BTN背景颜色 */
#define    BTN_FOCUS_BACK_COLOR    "QPushButton:focus{background-color: rgb(96,238,250);}"

/* 设置窗口背景色函数 */
extern void com_setBackColour(const QString &,QWidget *target);

#endif // COMMON_H
