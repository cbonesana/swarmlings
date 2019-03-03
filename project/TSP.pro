TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += \
    ../source/utilities.cpp \
    ../source/opt.cpp \
    ../source/nearest_neighbour.cpp \
    ../source/main.cpp \
    ../source/antcolony.cpp \
    ../source/ant.cpp

HEADERS += \
    ../source/types.h \
    ../source/utilities.h \
    ../source/opt.h \
    ../source/antcolony.h \
    ../source/ant.h \
    ../source/nearest_neighbour.h

OTHER_FILES +=
