#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T08:30:47
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DICOM_Parser
TEMPLATE = app

# Setup defines variable
DEFINES += HAVE_CONFIG_H

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/xmlItem.cpp \
        src/xmlAttribute.cpp

HEADERS  += include/mainwindow.h \
    include/xmlItem.h \
    include/xmlAttribute.h

# Setup common and libraries paths
win32 {
    COMMONBASE = $$(USERPROFILE)/Libraries
#    LIBRARIESBASE = C:/libs-vs11-x64
    LIBRARIESBASE = C:/Libraries
}

# Setup include paths
INCLUDEPATH += include/
INCLUDEPATH += $$quote($$LIBRARIESBASE/include)
INCLUDEPATH += $$quote($$LIBRARIESBASE/include/dcmtk)

win32 {
    win32-msvc2010 {
        # Setup dcmtk libraries for win32-msvc2010
        LIBS += -L$$quote($$LIBRARIESBASE/lib) -ldcmimage
        LIBS += -L$$quote($$LIBRARIESBASE/lib) -ldcmimgle
        LIBS += -L$$quote($$LIBRARIESBASE/lib) -ldcmdata
        LIBS += -L$$quote($$LIBRARIESBASE/lib) -loflog
        LIBS += -L$$quote($$LIBRARIESBASE/lib) -lofstd

        # Setup common library path
        LIBS += -L$$quote($$LIBRARIESBASE/lib)
        LIBS += -lbotan
        LIBS += -llibcurl
        LIBS += -llibssh2
        LIBS += -lssleay32
        LIBS += -llibeay32
        LIBS += -lnetapi32
        LIBS += -lws2_32
        LIBS += -lwldap32
        LIBS += -ladvapi32
        LIBS += -luser32
        LIBS += -lwsock32
    }
}

#FORMS    += mainwindow.ui
