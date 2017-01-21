#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T14:34:03
#
#-------------------------------------------------

QT       += core gui  network  sql

TARGET = Clever_CabinetScreen
TEMPLATE = app

INCLUDEPATH +=  serialport \
                                    modbus \
                                    total \
                                    output \
                                    fieldvalues \
                                    sensor \
                                    sql \
                                    log \
                                    alarm \
                                    datetime \
                                    common \
                                    partswidget \

SOURCES += main.cpp\
        mainwindow.cpp \
    serialport/serial_portset.cpp \
    serialport/serial_trans.cpp \
    modbus/mdb_readthread.cpp \
    serialport/serial_test.cpp \
    modbus/mdb_sentcmd.cpp \
    modbus/mdb_recvcmd.cpp \
    partswidget/wid_devinfo.cpp \
    partswidget/wid_outputbit.cpp \
    partswidget/wid_tabwidget.cpp \
    partswidget/wid_sensor.cpp \
    partswidget/wid_total.cpp \
    modbus/mdb_corethread.cpp \
    modbus/mdb_writethread.cpp \
    modbus/mdb_writecmd.cpp \
    total/total_maindlg.cpp \
    total/total_thresholdset.cpp \
    output/op_outputdlg.cpp \
    output/op_tablewidget.cpp \
    fieldvalues/fv_thresholddlg.cpp \
    fieldvalues/fv_tablewidget.cpp \
    fieldvalues/fv_setdlg.cpp \
    fieldvalues/fv_waitdlg.cpp \
    sensor/env_sensordlg.cpp \
    sensor/env_sensetdlg.cpp \
    sensor/env_tablewidget.cpp \
    sensor/env_setdlg.cpp \
    sensor/env_waitdlg.cpp \
    total/total_thresholddlg.cpp \
    total/total_waitdlg.cpp \
    sql/db_sqlquery.cpp \
    sql/db_sqltablemodel.cpp \
    log/log_tableview.cpp \
    log/log_showdlg.cpp \
    alarm/alarm_thread.cpp \
    datetime/timesettingdlg.cpp \
    common/common.cpp \
    fieldvalues/fv_maindlg.cpp

HEADERS  += mainwindow.h \
    serialport/Serial_PortData.h \
    serialport/serial_portset.h \
    serialport/serial_trans.h \
    modbus/mdb_readthread.h \
    serialport/serial_test.h \
    modbus/mdb_npmvdef.h \
    modbus/mdb_npmvtype.h \
    modbus/mdb_sentcmd.h \
    modbus/mdb_recvcmd.h \
    partswidget/wid_devinfo.h \
    partswidget/wid_outputbit.h \
    partswidget/wid_tabwidget.h \
    partswidget/wid_sensor.h \
    partswidget/wid_total.h \
    modbus/mdb_corethread.h \
    modbus/mdb_writethread.h \
    modbus/mdb_writecmd.h \
    total/total_maindlg.h \
    total/total_thresholdset.h \
    output/op_outputdlg.h \
    output/op_tablewidget.h \
    fieldvalues/fv_thresholddlg.h \
    fieldvalues/fv_tablewidget.h \
    fieldvalues/fv_setdlg.h \
    fieldvalues/fv_waitdlg.h \
    sensor/env_sensordlg.h \
    sensor/env_sensetdlg.h \
    sensor/env_tablewidget.h \
    sensor/env_setdlg.h \
    sensor/env_waitdlg.h \
    total/total_thresholddlg.h \
    total/total_waitdlg.h \
    sql/db_sqlquery.h \
    sql/db_sqltablemodel.h \
    log/log_tableview.h \
    log/log_showdlg.h \
    alarm/alarm_thread.h \
    datetime/timesettingdlg.h \
    common/common.h \
    fieldvalues/fv_maindlg.h

FORMS    += mainwindow.ui \
    total/total_maindlg.ui \
    total/total_thresholdset.ui \
    output/op_outputdlg.ui \
    fieldvalues/fv_thresholddlg.ui \
    fieldvalues/fv_setdlg.ui \
    fieldvalues/fv_waitdlg.ui \
    sensor/env_sensordlg.ui \
    sensor/env_sensetdlg.ui \
    sensor/env_setdlg.ui \
    sensor/env_waitdlg.ui \
    total/total_thresholddlg.ui \
    total/total_waitdlg.ui \
    log/log_showdlg.ui \
    datetime/timesettingdlg.ui \
    fieldvalues/fv_maindlg.ui

RESOURCES += \
    image/image.qrc








































































