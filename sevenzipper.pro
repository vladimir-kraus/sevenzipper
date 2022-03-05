CONFIG += warn_on

TARGET = sevenzipper

TEMPLATE = lib

DEFINES += \
    QT_NO_CAST_FROM_ASCII \
    QT_NO_CAST_TO_ASCII \
    QT_NO_CAST_FROM_BYTEARRAY \
    QT_NO_URL_CAST_FROM_STRING \
    QT_USE_QSTRINGBUILDER

DEFINES += MAKE_SEVENZIPPER_LIB

SOURCES += \
    src/library.cpp \
    src/guids.cpp \
    src/archiveformat.cpp \
    src/opencallback.cpp \
    src/archivereader.cpp \
    7-zip/CPP/Windows/FileIO.cpp \
    7-zip/CPP/7zip/Common/FileStreams.cpp \

HEADERS += \
    include/sevenzipper/archivereader.h \
    include/sevenzipper/library.h \
    src/guids.h \
    src/archiveformat.h \
    src/opencallback.h \

INCLUDEPATH += \
    include/sevenzipper

LIBS += -loleaut32
