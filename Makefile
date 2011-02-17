#############################################################################
# Makefile for building: QCanObserver
# Generated by qmake (2.01a) (Qt 4.6.2) on: Thu Feb 17 08:23:07 2011
# Project:  QCanObserver.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile QCanObserver.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -I/usr/include/qwt5 -Imodeltest -I. -I.
LINK          = g++
LFLAGS        = -Wl,-rpath,/usr/lib/qt4
LIBS          = $(SUBLIBS)  -L/usr/lib/qt4 -L/usr/lib/ -lqwt -lQtXml -L/usr/lib/qt4 -lQtGui -L/usr/X11R6/lib -lQtCore -lgthread-2.0 -lrt -lglib-2.0 -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		mainwindow.cpp \
		readthread.cpp \
		DevDialog.cpp \
		errordialog.cpp \
		messagebufferinterface.cpp \
		stringlistmodel.cpp \
		FilterDialog.cpp \
		filterdef.cpp \
		graphicwindow.cpp \
		processdatabase.cpp \
		observerdialog.cpp \
		aboutbox.cpp \
		sendmsgdialog.cpp \
		msgdefdialog.cpp \
		cansignal.cpp \
		cansignalcollection.cpp \
		extrect.cpp \
		configdialog.cpp \
		signalselectordialog.cpp \
		specialeventdialog.cpp \
		writethread.cpp \
		qcancostumplotcurve.cpp \
		averagefilter.cpp \
		debugterminal.cpp \
		qconsole.cpp \
		modeltest/modeltest.cpp moc_mainwindow.cpp \
		moc_readthread.cpp \
		moc_DevDialog.cpp \
		moc_errordialog.cpp \
		moc_messagebufferinterface.cpp \
		moc_stringlistmodel.cpp \
		moc_FilterDialog.cpp \
		moc_filterdef.cpp \
		moc_graphicwindow.cpp \
		moc_observerdialog.cpp \
		moc_aboutbox.cpp \
		moc_sendmsgdialog.cpp \
		moc_msgdefdialog.cpp \
		moc_configdialog.cpp \
		moc_signalselectordialog.cpp \
		moc_specialeventdialog.cpp \
		moc_writethread.cpp \
		moc_debugterminal.cpp \
		moc_qconsole.cpp \
		moc_modeltest.cpp
OBJECTS       = main.o \
		mainwindow.o \
		readthread.o \
		DevDialog.o \
		errordialog.o \
		messagebufferinterface.o \
		stringlistmodel.o \
		FilterDialog.o \
		filterdef.o \
		graphicwindow.o \
		processdatabase.o \
		observerdialog.o \
		aboutbox.o \
		sendmsgdialog.o \
		msgdefdialog.o \
		cansignal.o \
		cansignalcollection.o \
		extrect.o \
		configdialog.o \
		signalselectordialog.o \
		specialeventdialog.o \
		writethread.o \
		qcancostumplotcurve.o \
		averagefilter.o \
		debugterminal.o \
		qconsole.o \
		modeltest.o \
		moc_mainwindow.o \
		moc_readthread.o \
		moc_DevDialog.o \
		moc_errordialog.o \
		moc_messagebufferinterface.o \
		moc_stringlistmodel.o \
		moc_FilterDialog.o \
		moc_filterdef.o \
		moc_graphicwindow.o \
		moc_observerdialog.o \
		moc_aboutbox.o \
		moc_sendmsgdialog.o \
		moc_msgdefdialog.o \
		moc_configdialog.o \
		moc_signalselectordialog.o \
		moc_specialeventdialog.o \
		moc_writethread.o \
		moc_debugterminal.o \
		moc_qconsole.o \
		moc_modeltest.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		modeltest/modeltest.pri \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		QCanObserver.pro
QMAKE_TARGET  = QCanObserver
DESTDIR       = 
TARGET        = QCanObserver

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_mainwindow.h ui_DevDialog.h ui_errordialog.h ui_FilterDialog.h ui_filterdef.h ui_graphicwindow.h ui_observerdialog.h ui_aboutbox.h ui_sendmsgdialog.h ui_msgdefdialog.h ui_configdialog.h ui_signalselectordialog.h ui_specialeventdialog.h ui_debugterminal.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: QCanObserver.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		modeltest/modeltest.pri \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/qt4/libQtXml.prl \
		/usr/lib/qt4/libQtCore.prl \
		/usr/lib/qt4/libQtGui.prl
	$(QMAKE) -unix -o Makefile QCanObserver.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
modeltest/modeltest.pri:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/qt4/libQtXml.prl:
/usr/lib/qt4/libQtCore.prl:
/usr/lib/qt4/libQtGui.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile QCanObserver.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/QCanObserver1.0.0 || $(MKDIR) .tmp/QCanObserver1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/QCanObserver1.0.0/ && $(COPY_FILE) --parents mainwindow.h readthread.h DevDialog.h errordialog.h messagebufferinterface.h stringlistmodel.h FilterDialog.h filterdef.h graphicwindow.h processdatabase.h observerdialog.h aboutbox.h sendmsgdialog.h msgdefdialog.h cansignal.h cansignalcollection.h config.h obscan.h extrect.h configdialog.h signalselectordialog.h specialeventdialog.h writethread.h DeviceLib/candevice.h qcancostumplotcurve.h averagefilter.h debugterminal.h qconsole.h modeltest/modeltest.h .tmp/QCanObserver1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp readthread.cpp DevDialog.cpp errordialog.cpp messagebufferinterface.cpp stringlistmodel.cpp FilterDialog.cpp filterdef.cpp graphicwindow.cpp processdatabase.cpp observerdialog.cpp aboutbox.cpp sendmsgdialog.cpp msgdefdialog.cpp cansignal.cpp cansignalcollection.cpp extrect.cpp configdialog.cpp signalselectordialog.cpp specialeventdialog.cpp writethread.cpp qcancostumplotcurve.cpp averagefilter.cpp debugterminal.cpp qconsole.cpp modeltest/modeltest.cpp .tmp/QCanObserver1.0.0/ && $(COPY_FILE) --parents mainwindow.ui DevDialog.ui errordialog.ui FilterDialog.ui FilterDialog.ui filterdef.ui filterdef.ui graphicwindow.ui observerdialog.ui aboutbox.ui sendmsgdialog.ui msgdefdialog.ui configdialog.ui signalselectordialog.ui specialeventdialog.ui debugterminal.ui .tmp/QCanObserver1.0.0/ && (cd `dirname .tmp/QCanObserver1.0.0` && $(TAR) QCanObserver1.0.0.tar QCanObserver1.0.0 && $(COMPRESS) QCanObserver1.0.0.tar) && $(MOVE) `dirname .tmp/QCanObserver1.0.0`/QCanObserver1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/QCanObserver1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp moc_readthread.cpp moc_DevDialog.cpp moc_errordialog.cpp moc_messagebufferinterface.cpp moc_stringlistmodel.cpp moc_FilterDialog.cpp moc_filterdef.cpp moc_graphicwindow.cpp moc_observerdialog.cpp moc_aboutbox.cpp moc_sendmsgdialog.cpp moc_msgdefdialog.cpp moc_configdialog.cpp moc_signalselectordialog.cpp moc_specialeventdialog.cpp moc_writethread.cpp moc_debugterminal.cpp moc_qconsole.cpp moc_modeltest.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_readthread.cpp moc_DevDialog.cpp moc_errordialog.cpp moc_messagebufferinterface.cpp moc_stringlistmodel.cpp moc_FilterDialog.cpp moc_filterdef.cpp moc_graphicwindow.cpp moc_observerdialog.cpp moc_aboutbox.cpp moc_sendmsgdialog.cpp moc_msgdefdialog.cpp moc_configdialog.cpp moc_signalselectordialog.cpp moc_specialeventdialog.cpp moc_writethread.cpp moc_debugterminal.cpp moc_qconsole.cpp moc_modeltest.cpp
moc_mainwindow.cpp: readthread.h \
		config.h \
		errordialog.h \
		DeviceLib/candevice.h \
		obscan.h \
		messagebufferinterface.h \
		FilterDialog.h \
		filterdef.h \
		writethread.h \
		sendmsgdialog.h \
		msgdefdialog.h \
		stringlistmodel.h \
		cansignal.h \
		cansignalcollection.h \
		signalselectordialog.h \
		processdatabase.h \
		DevDialog.h \
		graphicwindow.h \
		qcancostumplotcurve.h \
		observerdialog.h \
		extrect.h \
		specialeventdialog.h \
		averagefilter.h \
		debugterminal.h \
		qconsole.h \
		mainwindow.h
	/usr/bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_readthread.cpp: config.h \
		errordialog.h \
		DeviceLib/candevice.h \
		obscan.h \
		messagebufferinterface.h \
		FilterDialog.h \
		filterdef.h \
		readthread.h
	/usr/bin/moc $(DEFINES) $(INCPATH) readthread.h -o moc_readthread.cpp

moc_DevDialog.cpp: DevDialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) DevDialog.h -o moc_DevDialog.cpp

moc_errordialog.cpp: errordialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) errordialog.h -o moc_errordialog.cpp

moc_messagebufferinterface.cpp: obscan.h \
		config.h \
		messagebufferinterface.h
	/usr/bin/moc $(DEFINES) $(INCPATH) messagebufferinterface.h -o moc_messagebufferinterface.cpp

moc_stringlistmodel.cpp: stringlistmodel.h
	/usr/bin/moc $(DEFINES) $(INCPATH) stringlistmodel.h -o moc_stringlistmodel.cpp

moc_FilterDialog.cpp: filterdef.h \
		FilterDialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) FilterDialog.h -o moc_FilterDialog.cpp

moc_filterdef.cpp: filterdef.h
	/usr/bin/moc $(DEFINES) $(INCPATH) filterdef.h -o moc_filterdef.cpp

moc_graphicwindow.cpp: messagebufferinterface.h \
		obscan.h \
		config.h \
		processdatabase.h \
		cansignalcollection.h \
		cansignal.h \
		signalselectordialog.h \
		errordialog.h \
		qcancostumplotcurve.h \
		graphicwindow.h
	/usr/bin/moc $(DEFINES) $(INCPATH) graphicwindow.h -o moc_graphicwindow.cpp

moc_observerdialog.cpp: processdatabase.h \
		cansignalcollection.h \
		cansignal.h \
		signalselectordialog.h \
		errordialog.h \
		messagebufferinterface.h \
		obscan.h \
		config.h \
		stringlistmodel.h \
		observerdialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) observerdialog.h -o moc_observerdialog.cpp

moc_aboutbox.cpp: aboutbox.h
	/usr/bin/moc $(DEFINES) $(INCPATH) aboutbox.h -o moc_aboutbox.cpp

moc_sendmsgdialog.cpp: msgdefdialog.h \
		stringlistmodel.h \
		DeviceLib/candevice.h \
		config.h \
		obscan.h \
		cansignal.h \
		cansignalcollection.h \
		signalselectordialog.h \
		processdatabase.h \
		errordialog.h \
		sendmsgdialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) sendmsgdialog.h -o moc_sendmsgdialog.cpp

moc_msgdefdialog.cpp: msgdefdialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) msgdefdialog.h -o moc_msgdefdialog.cpp

moc_configdialog.cpp: configdialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) configdialog.h -o moc_configdialog.cpp

moc_signalselectordialog.cpp: processdatabase.h \
		cansignalcollection.h \
		cansignal.h \
		errordialog.h \
		signalselectordialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) signalselectordialog.h -o moc_signalselectordialog.cpp

moc_specialeventdialog.cpp: stringlistmodel.h \
		obscan.h \
		config.h \
		specialeventdialog.h
	/usr/bin/moc $(DEFINES) $(INCPATH) specialeventdialog.h -o moc_specialeventdialog.cpp

moc_writethread.cpp: config.h \
		errordialog.h \
		DeviceLib/candevice.h \
		obscan.h \
		messagebufferinterface.h \
		FilterDialog.h \
		filterdef.h \
		sendmsgdialog.h \
		msgdefdialog.h \
		stringlistmodel.h \
		cansignal.h \
		cansignalcollection.h \
		signalselectordialog.h \
		processdatabase.h \
		writethread.h
	/usr/bin/moc $(DEFINES) $(INCPATH) writethread.h -o moc_writethread.cpp

moc_debugterminal.cpp: cansignalcollection.h \
		cansignal.h \
		signalselectordialog.h \
		processdatabase.h \
		errordialog.h \
		messagebufferinterface.h \
		obscan.h \
		config.h \
		qconsole.h \
		debugterminal.h
	/usr/bin/moc $(DEFINES) $(INCPATH) debugterminal.h -o moc_debugterminal.cpp

moc_qconsole.cpp: qconsole.h
	/usr/bin/moc $(DEFINES) $(INCPATH) qconsole.h -o moc_qconsole.cpp

moc_modeltest.cpp: modeltest/modeltest.h
	/usr/bin/moc $(DEFINES) $(INCPATH) modeltest/modeltest.h -o moc_modeltest.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h ui_DevDialog.h ui_errordialog.h ui_FilterDialog.h ui_FilterDialog.h ui_filterdef.h ui_filterdef.h ui_graphicwindow.h ui_observerdialog.h ui_aboutbox.h ui_sendmsgdialog.h ui_msgdefdialog.h ui_configdialog.h ui_signalselectordialog.h ui_specialeventdialog.h ui_debugterminal.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h ui_DevDialog.h ui_errordialog.h ui_FilterDialog.h ui_FilterDialog.h ui_filterdef.h ui_filterdef.h ui_graphicwindow.h ui_observerdialog.h ui_aboutbox.h ui_sendmsgdialog.h ui_msgdefdialog.h ui_configdialog.h ui_signalselectordialog.h ui_specialeventdialog.h ui_debugterminal.h
ui_mainwindow.h: mainwindow.ui
	/usr/bin/uic mainwindow.ui -o ui_mainwindow.h

ui_DevDialog.h: DevDialog.ui
	/usr/bin/uic DevDialog.ui -o ui_DevDialog.h

ui_errordialog.h: errordialog.ui
	/usr/bin/uic errordialog.ui -o ui_errordialog.h

ui_FilterDialog.h: FilterDialog.ui
	/usr/bin/uic FilterDialog.ui -o ui_FilterDialog.h

ui_FilterDialog.h: FilterDialog.ui
	/usr/bin/uic FilterDialog.ui -o ui_FilterDialog.h

ui_filterdef.h: filterdef.ui
	/usr/bin/uic filterdef.ui -o ui_filterdef.h

ui_filterdef.h: filterdef.ui
	/usr/bin/uic filterdef.ui -o ui_filterdef.h

ui_graphicwindow.h: graphicwindow.ui
	/usr/bin/uic graphicwindow.ui -o ui_graphicwindow.h

ui_observerdialog.h: observerdialog.ui
	/usr/bin/uic observerdialog.ui -o ui_observerdialog.h

ui_aboutbox.h: aboutbox.ui
	/usr/bin/uic aboutbox.ui -o ui_aboutbox.h

ui_sendmsgdialog.h: sendmsgdialog.ui
	/usr/bin/uic sendmsgdialog.ui -o ui_sendmsgdialog.h

ui_msgdefdialog.h: msgdefdialog.ui
	/usr/bin/uic msgdefdialog.ui -o ui_msgdefdialog.h

ui_configdialog.h: configdialog.ui
	/usr/bin/uic configdialog.ui -o ui_configdialog.h

ui_signalselectordialog.h: signalselectordialog.ui
	/usr/bin/uic signalselectordialog.ui -o ui_signalselectordialog.h

ui_specialeventdialog.h: specialeventdialog.ui
	/usr/bin/uic specialeventdialog.ui -o ui_specialeventdialog.h

ui_debugterminal.h: debugterminal.ui
	/usr/bin/uic debugterminal.ui -o ui_debugterminal.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		readthread.h \
		config.h \
		errordialog.h \
		DeviceLib/candevice.h \
		obscan.h \
		messagebufferinterface.h \
		FilterDialog.h \
		filterdef.h \
		writethread.h \
		sendmsgdialog.h \
		msgdefdialog.h \
		stringlistmodel.h \
		cansignal.h \
		cansignalcollection.h \
		signalselectordialog.h \
		processdatabase.h \
		DevDialog.h \
		graphicwindow.h \
		qcancostumplotcurve.h \
		observerdialog.h \
		extrect.h \
		specialeventdialog.h \
		averagefilter.h \
		debugterminal.h \
		qconsole.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		readthread.h \
		config.h \
		errordialog.h \
		DeviceLib/candevice.h \
		obscan.h \
		messagebufferinterface.h \
		FilterDialog.h \
		filterdef.h \
		writethread.h \
		sendmsgdialog.h \
		msgdefdialog.h \
		stringlistmodel.h \
		cansignal.h \
		cansignalcollection.h \
		signalselectordialog.h \
		processdatabase.h \
		DevDialog.h \
		graphicwindow.h \
		qcancostumplotcurve.h \
		observerdialog.h \
		extrect.h \
		specialeventdialog.h \
		averagefilter.h \
		debugterminal.h \
		qconsole.h \
		ui_mainwindow.h \
		aboutbox.h \
		modeltest/modeltest.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

readthread.o: readthread.cpp config.h \
		readthread.h \
		errordialog.h \
		DeviceLib/candevice.h \
		obscan.h \
		messagebufferinterface.h \
		FilterDialog.h \
		filterdef.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o readthread.o readthread.cpp

DevDialog.o: DevDialog.cpp config.h \
		DevDialog.h \
		ui_DevDialog.h \
		DeviceLib/candevice.h \
		obscan.h \
		errordialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DevDialog.o DevDialog.cpp

errordialog.o: errordialog.cpp errordialog.h \
		ui_errordialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o errordialog.o errordialog.cpp

messagebufferinterface.o: messagebufferinterface.cpp messagebufferinterface.h \
		obscan.h \
		config.h \
		errordialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o messagebufferinterface.o messagebufferinterface.cpp

stringlistmodel.o: stringlistmodel.cpp config.h \
		stringlistmodel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o stringlistmodel.o stringlistmodel.cpp

FilterDialog.o: FilterDialog.cpp FilterDialog.h \
		filterdef.h \
		ui_FilterDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FilterDialog.o FilterDialog.cpp

filterdef.o: filterdef.cpp filterdef.h \
		ui_filterdef.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o filterdef.o filterdef.cpp

graphicwindow.o: graphicwindow.cpp config.h \
		graphicwindow.h \
		messagebufferinterface.h \
		obscan.h \
		processdatabase.h \
		cansignalcollection.h \
		cansignal.h \
		signalselectordialog.h \
		errordialog.h \
		qcancostumplotcurve.h \
		ui_graphicwindow.h \
		extrect.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o graphicwindow.o graphicwindow.cpp

processdatabase.o: processdatabase.cpp processdatabase.h \
		cansignalcollection.h \
		cansignal.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o processdatabase.o processdatabase.cpp

observerdialog.o: observerdialog.cpp observerdialog.h \
		processdatabase.h \
		cansignalcollection.h \
		cansignal.h \
		signalselectordialog.h \
		errordialog.h \
		messagebufferinterface.h \
		obscan.h \
		config.h \
		stringlistmodel.h \
		ui_observerdialog.h \
		extrect.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o observerdialog.o observerdialog.cpp

aboutbox.o: aboutbox.cpp aboutbox.h \
		ui_aboutbox.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o aboutbox.o aboutbox.cpp

sendmsgdialog.o: sendmsgdialog.cpp sendmsgdialog.h \
		msgdefdialog.h \
		stringlistmodel.h \
		DeviceLib/candevice.h \
		config.h \
		obscan.h \
		cansignal.h \
		cansignalcollection.h \
		signalselectordialog.h \
		processdatabase.h \
		errordialog.h \
		ui_sendmsgdialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sendmsgdialog.o sendmsgdialog.cpp

msgdefdialog.o: msgdefdialog.cpp msgdefdialog.h \
		ui_msgdefdialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o msgdefdialog.o msgdefdialog.cpp

cansignal.o: cansignal.cpp cansignal.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o cansignal.o cansignal.cpp

cansignalcollection.o: cansignalcollection.cpp cansignalcollection.h \
		cansignal.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o cansignalcollection.o cansignalcollection.cpp

extrect.o: extrect.cpp extrect.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o extrect.o extrect.cpp

configdialog.o: configdialog.cpp configdialog.h \
		ui_configdialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o configdialog.o configdialog.cpp

signalselectordialog.o: signalselectordialog.cpp signalselectordialog.h \
		processdatabase.h \
		cansignalcollection.h \
		cansignal.h \
		errordialog.h \
		ui_signalselectordialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o signalselectordialog.o signalselectordialog.cpp

specialeventdialog.o: specialeventdialog.cpp specialeventdialog.h \
		stringlistmodel.h \
		obscan.h \
		config.h \
		ui_specialeventdialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o specialeventdialog.o specialeventdialog.cpp

writethread.o: writethread.cpp config.h \
		writethread.h \
		errordialog.h \
		DeviceLib/candevice.h \
		obscan.h \
		messagebufferinterface.h \
		FilterDialog.h \
		filterdef.h \
		sendmsgdialog.h \
		msgdefdialog.h \
		stringlistmodel.h \
		cansignal.h \
		cansignalcollection.h \
		signalselectordialog.h \
		processdatabase.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o writethread.o writethread.cpp

qcancostumplotcurve.o: qcancostumplotcurve.cpp qcancostumplotcurve.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qcancostumplotcurve.o qcancostumplotcurve.cpp

averagefilter.o: averagefilter.cpp averagefilter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o averagefilter.o averagefilter.cpp

debugterminal.o: debugterminal.cpp debugterminal.h \
		cansignalcollection.h \
		cansignal.h \
		signalselectordialog.h \
		processdatabase.h \
		errordialog.h \
		messagebufferinterface.h \
		obscan.h \
		config.h \
		qconsole.h \
		ui_debugterminal.h \
		extrect.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o debugterminal.o debugterminal.cpp

qconsole.o: qconsole.cpp qconsole.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qconsole.o qconsole.cpp

modeltest.o: modeltest/modeltest.cpp modeltest/modeltest.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o modeltest.o modeltest/modeltest.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_readthread.o: moc_readthread.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_readthread.o moc_readthread.cpp

moc_DevDialog.o: moc_DevDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_DevDialog.o moc_DevDialog.cpp

moc_errordialog.o: moc_errordialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_errordialog.o moc_errordialog.cpp

moc_messagebufferinterface.o: moc_messagebufferinterface.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_messagebufferinterface.o moc_messagebufferinterface.cpp

moc_stringlistmodel.o: moc_stringlistmodel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_stringlistmodel.o moc_stringlistmodel.cpp

moc_FilterDialog.o: moc_FilterDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_FilterDialog.o moc_FilterDialog.cpp

moc_filterdef.o: moc_filterdef.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_filterdef.o moc_filterdef.cpp

moc_graphicwindow.o: moc_graphicwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_graphicwindow.o moc_graphicwindow.cpp

moc_observerdialog.o: moc_observerdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_observerdialog.o moc_observerdialog.cpp

moc_aboutbox.o: moc_aboutbox.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_aboutbox.o moc_aboutbox.cpp

moc_sendmsgdialog.o: moc_sendmsgdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_sendmsgdialog.o moc_sendmsgdialog.cpp

moc_msgdefdialog.o: moc_msgdefdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_msgdefdialog.o moc_msgdefdialog.cpp

moc_configdialog.o: moc_configdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_configdialog.o moc_configdialog.cpp

moc_signalselectordialog.o: moc_signalselectordialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_signalselectordialog.o moc_signalselectordialog.cpp

moc_specialeventdialog.o: moc_specialeventdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_specialeventdialog.o moc_specialeventdialog.cpp

moc_writethread.o: moc_writethread.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_writethread.o moc_writethread.cpp

moc_debugterminal.o: moc_debugterminal.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_debugterminal.o moc_debugterminal.cpp

moc_qconsole.o: moc_qconsole.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_qconsole.o moc_qconsole.cpp

moc_modeltest.o: moc_modeltest.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_modeltest.o moc_modeltest.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

