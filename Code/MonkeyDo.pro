QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lKExiv2Qt6

SOURCES += \
    SettingsDialog/SettingsDialog.cpp \
    TransformOperators/TransformOperator.cpp \
    TransformOperators/TransformOperator_Case.cpp \
    TransformOperators/TransformOperator_DateTime.cpp \
    TransformOperators/TransformOperator_InsertOverwrite.cpp \
    TransformOperators/TransformOperator_Numbering.cpp \
    TransformOperators/TransformOperator_RemoveChars.cpp \
    TransformOperators/TransformOperator_SearchReplace.cpp \
    main.cpp \
    MainWindow/MainWindow.cpp \
    TransformEngine/TransformEngine.cpp \
    TransformEngine/TransformItem.cpp \
    AboutDialog/AboutDialog.cpp \
    FileOperation/FileOperation.cpp \
    FileSystemOverlay/FileSystemOverlay.cpp \
    Styling/SingleClickWorkaroundProxyStyle.cpp \
    DragTableView/DragTableView.cpp \
    FilesAndDirsDialog/FilesAndDirsDialog.cpp \
    SettingsController/SettingsController.cpp \
    SettingsController/TransformOperatorInfo.cpp
    
HEADERS += \
    SettingsDialog/SettingsDialog.h \
    TransformOperators/TransformOperator.h \
    TransformOperators/TransformOperator_Case.h \
    TransformOperators/TransformOperator_DateTime.h \
    TransformOperators/TransformOperator_InsertOverwrite.h \
    TransformOperators/TransformOperator_Numbering.h \
    TransformOperators/TransformOperator_RemoveChars.h \
    TransformOperators/TransformOperator_SearchReplace.h \
    TransformOperators/enum_transformScope.h \
    version.h\
    MainWindow/MainWindow.h \
    TransformEngine/TransformEngine.h \
    TransformEngine/TransformItem.h \
    AboutDialog/AboutDialog.h \
    FileOperation/FileOperation.h \
    FileSystemOverlay/FileSystemOverlay.h\
    Styling/SingleClickWorkaroundProxyStyle.h \
    DragTableView/DragTableView.h \
    FilesAndDirsDialog/FilesAndDirsDialog.h \
    SettingsController/SettingsController.h \
    SettingsController/TransformOperatorInfo.h
    
FORMS += \
    MainWindow/MainWindow.ui \
    SettingsDialog/SettingsDialog.ui \
    TransformOperators/TransformOperator_Case.ui \
    TransformOperators/TransformOperator_DateTime.ui \
    TransformOperators/TransformOperator_InsertOverwrite.ui \
    TransformOperators/TransformOperator_Numbering.ui \
    TransformOperators/TransformOperator_RemoveChars.ui \
    AboutDialog/AboutDialog.ui \
    SettingsDialog/SettingsDialog.ui \
    TransformOperators/TransformOperator_SearchReplace.ui

# Include to support reading image metadata
INCLUDEPATH += /usr/include/KExiv2Qt6/

TRANSLATIONS = \
    Translations/MonkeyDo_en_GB.ts \
    Translations/MonkeyDo_nl.ts \
    Translations/MonkeyDo_de.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += Resources.qrc




