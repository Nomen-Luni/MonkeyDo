QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lKExiv2Qt6

SOURCES += \
    main.cpp \
    MainWindow/MainWindow.cpp \
    TransformEngine/TransformEngine.cpp \
    TransformProviders/TransformProvider.cpp \
    TransformProviders/TransformProvider_Case.cpp \
    TransformProviders/TransformProvider_DateTime.cpp \
    TransformProviders/TransformProvider_InsertOverwrite.cpp \
    TransformProviders/TransformProvider_Numbering.cpp \
    TransformProviders/TransformProvider_RemoveChars.cpp \
    TransformProviders/TransformProvider_SearchReplace.cpp \
    AboutDialog/AboutDialog.cpp \
    FileOperation/FileOperation.cpp \
    FileSystemOverlay/FileSystemOverlay.cpp \
    Styling/SingleClickWorkaroundProxyStyle.cpp \
    DragTableView/DragTableView.cpp
    
HEADERS += \
    version.h\
    MainWindow/MainWindow.h \
    TransformEngine/TransformEngine.h \
    TransformProviders/enum_transformScope.h \
    TransformProviders/TransformProvider.h \
    TransformProviders/TransformProvider_Case.h \
    TransformProviders/TransformProvider_DateTime.h \
    TransformProviders/TransformProvider_InsertOverwrite.h \
    TransformProviders/TransformProvider_Numbering.h \
    TransformProviders/TransformProvider_RemoveChars.h \
    TransformProviders/TransformProvider_SearchReplace.h \
    AboutDialog/AboutDialog.h \
    FileOperation/FileOperation.h \
    FileSystemOverlay/FileSystemOverlay.h\
    Styling/SingleClickWorkaroundProxyStyle.h \
    DragTableView/DragTableView.h
    
FORMS += \
    MainWindow/MainWindow.ui \
    TransformProviders/TransformProvider_Case.ui \
    TransformProviders/TransformProvider_DateTime.ui \
    TransformProviders/TransformProvider_InsertOverwrite.ui \
    TransformProviders/TransformProvider_Numbering.ui \
    TransformProviders/TransformProvider_RemoveChars.ui \
    TransformProviders/TransformProvider_SearchReplace.ui \
    AboutDialog/AboutDialog.ui

# Include to support reading image metadata
INCLUDEPATH += /usr/include/KExiv2Qt6/

TRANSLATIONS = \
    Translations/Batch_Renamer_en_GB.ts \
    Translations/Batch_Renamer_nl.ts \
    Translations/Batch_Renamer_de.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
    

RESOURCES += Resources.qrc




