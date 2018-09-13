
TARGET   = dmg_wd_manager
CONFIG += console c++98
CONFIG -= app_bundle
CONFIG -= qt
PKGCONFIG+= fuse

DEFINES  += _FILE_OFFSET_BITS=64 FUSE_USE_VERSION=26

TEMPLATE  = app
SOURCES += main.cpp \
    fuse_operations_interface.cpp \
    tef_fuse_operations.cpp


HEADERS += \
    fuse_operations_interface.h \
    tef_fuse_operations.h

cortexa7 {
  target.path = /tmp
  target.files = dmg_wd_manager

  INSTALLS += target
}
unix:!macx: LIBS += -lpthread -lfuse


 QMAKE_CXXFLAGS_WARN_ON += -Wall -Wno-unused-parameter
