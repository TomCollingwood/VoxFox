TEMPLATE=lib
# This specifies the exe name
TARGET=$$PWD/lib/VoxFox
CONFIG+=c++11
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core
# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
	cache()
	DEFINES +=QT5BUILD
}
# where to put moc auto generated files
MOC_DIR=moc
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+=   $$PWD/src/LeafNode.cpp \
            $$PWD/src/VoxFoxTree.cpp \
            $$PWD/src/SecondaryNode.cpp \
            $$PWD/src/PrimaryNode.cpp
# same for the .h files
HEADERS+=   $$PWD/include/VoxFoxTree.h \
            $$PWD/include/LeafNode.h \
            $$PWD/include/SecondaryNode.h \
            $$PWD/include/PrimaryNode.h \
            $$PWD/include/Voxel.h
# and add the include dir into the search path for Qt and make
INCLUDEPATH +=./include \
              $$PWD/lib
# note each command you add needs a ; as it will be run as a single line
# first check if we are shadow building or not easiest way is to check out against current

  INCLUDEPATH += /usr/local/include
  INCLUDEPATH += /usr/local/Cellar
  INCLUDEPATH += /usr/local/lib

macx{
    LIBS+= -framework OpenGL
    LIBS+= -framework GLUT
}
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
	message("including $HOME/NGL")
	include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
	message("Using custom NGL location")
	include($(NGLDIR)/UseNGL.pri)
}

message("Compiled VoxFox")
