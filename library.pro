TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC

SOURCES += \
    library.cpp \
    main.cpp

HEADERS += \
    library.hh
