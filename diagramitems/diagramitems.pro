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

QT += xml xmlpatterns sql printsupport widgets

TARGET = DiagramItems
TEMPLATE = lib
DEFINES += DIAGRAMITEMS_LIBRARY
SOURCES += \
	pantalladiagrama.cpp \
	hub.cpp \
	conector.cpp \
	boxsidehub.cpp \
	model/relacion.cpp \
	models/columnlistmodel.cpp \
	delegates/comboboxdelegate.cpp \
	ficheros/ficheros.cpp \
	ficheros/SQL/sqlcore.cpp \
	ficheros/SQL/sqlfactory.cpp \
	ficheros/SQL/dbms/postgres.cpp \
	ficheros/SQL/dbms/mysql.cpp \
	ficheros/SQL/dbms/sqlite.cpp \
	model/selfrelacion.cpp \
	loadsaveproject.cpp \
	ficheros/SQL/dbms/sqlserver.cpp \
        model/columnastabla.cpp \
	comandos.cpp \
	baseitem.cpp \
	xmlbase.cpp \
	itemmovible.cpp \
	itemlinea.cpp \
    ficheros/SQL/dbms/oracle.cpp \
    delegates/uniquedelegate.cpp \
    model/attribute.cpp \
    model/attributes.cpp \
    model/entityattribute.cpp \
    model/entity.cpp

HEADERS += diagramitems_global.h \
	pantalladiagrama.h \
	hub.h \
	conector.h \
	boxsidehub.h \
	model/relacion.h \
	utilidades/singelton.h \
	models/columnlistmodel.h \
	delegates/comboboxdelegate.h \
	ficheros/ficheros.h \
	ficheros/SQL/sqlcore.h \
	ficheros/SQL/sqlfactory.h \
	ficheros/SQL/dbms/postgres.h \
	ficheros/SQL/dbms/mysql.h \
	ficheros/SQL/dbms/sqlite.h \
	model/selfrelacion.h \
	loadsaveproject.h \
	ficheros/SQL/dbms/sqlserver.h \
	model/columnastabla.h \
	comandos.h \
	diagramnamespace.h \
	baseitem.h \
	xmlbase.h \
	xmlutils.h \
	itemmovible.h \
	itemlinea.h \
    ficheros/SQL/dbms/oracle.h \
    delegates/uniquedelegate.h \
    model/attribute.h \
    model/attributes.h \
    model/entityattibute.h \
    model/entity.h

win32:INCLUDEPATH += ./

DESTDIR = ../app/bin/

include(../targets.pri)
