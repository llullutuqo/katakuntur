###############################################################################
#
# Kata Kuntur is Copyright (C) 2010 - 2017 Jean Mazuelos(LlulluTuqo)
# Contact   : http://jeanmazuelos.com/contact
#
# This file is part of Kata Kuntur - Data Modelling Tool.
# LGPL V3 License Usage
#
# You should use this file under the terms of the
# GNU LESSER GENERAL PUBLIC LICENSE - https://www.gnu.org/licenses/lgpl-3.0.txt
#
# Additionally, any Redistributions of source code must retain the above
# copyright notice, this condition and the following disclaimer.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
#
#
###############################################################################

TARGET = katakuntur

QT += xml sql core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport concurrent

TEMPLATE = app
SOURCES += main.cpp \
	mainwindow.cpp \
	formpantalladiagrama.cpp \
	formexportdbms.cpp \
	formabout.cpp \
	formpropiedades.cpp \
	propiedadesdiagramador.cpp \
	widgets/propiedades.cpp \
	widgets/formtabla.cpp \
	widgets/formselfrelacion.cpp \
	widgets/formrelacion.cpp \
	widgets/formcolumna.cpp \
	editores/columneditor.cpp \
	inspectdiagramwidget.cpp \
	wgbienvenida.cpp
HEADERS += mainwindow.h \
	formpantalladiagrama.h \
	formexportdbms.h \
	formabout.h \
	formpropiedades.h \
	paginas.h \
	propiedadesdiagramador.h \
	widgets/propiedades.h \
	widgets/formtabla.h \
	widgets/formselfrelacion.h \
	widgets/formrelacion.h \
	widgets/formcolumna.h \
	editores/columneditor.h \
	inspectdiagramwidget.h \
	wgbienvenida.h
FORMS += mainwindow.ui \
	formpantalladiagrama.ui \
	formexportdbms.ui \
	formabout.ui \
	widgets/formtabla.ui \
	widgets/formselfrelacion.ui \
	widgets/formrelacion.ui \
	widgets/formcolumna.ui \
	inspectdiagramwidget.ui \
	wgbienvenida.ui
INCLUDEPATH += 	../tools/ \
        ../widgets/


unix:LIBS += -L../app/bin/ -ltools -lwidgets
win32:LIBS += -L../app/bin/ -ltools2 -lwidgets2 -lAdvapi32

RESOURCES += i16x.qrc \
	skin.qrc \
	i22x.qrc
INCLUDEPATH += ./

DESTDIR = ../app/bin/

include(../pluginsystem/pluginsystem.pri)
include(../diagramitems/diagramitems.pri)
include(../ormmanager/ormmanager.pri)
include(../targets.pri)
