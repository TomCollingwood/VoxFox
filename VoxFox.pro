TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cpp \
    src/RootNode.cpp \
    src/PrimaryNode.cpp \
    src/SecondaryNode.cpp \
    src/LeafNode.cpp \
    src/PolyToVoxel.cpp

HEADERS += \
    include/RootNode.h \
    include/PrimaryNode.h \
    include/SecondaryNode.h \
    include/LeafNode.h \
    include/PolyToVoxel.h

OTHER_FILES += lib/glm/glm/*
