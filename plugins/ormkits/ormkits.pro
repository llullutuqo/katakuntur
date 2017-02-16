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

include(../plugins.pri)
QT       = widgets xml

TARGET = ORMKits
TEMPLATE = lib

DEFINES += ORMKITS_LIBRARY

SOURCES += \
    ormkitsplugin.cpp \
    cppqt/classfile.cpp \
    cppqt/cppfile.cpp \
    cppqt/hfile.cpp \
    cppqt/htmlfile.cpp \
    cppqt/propertiesdialog.cpp \
    cppqt/qhpfile.cpp \
    cppqt/uifile.cpp \
    cppqt/utils.cpp \
    cppqt/ormkatakowi.cpp

HEADERS +=\
        ormkits_global.h \
    ormkitsplugin.h \
    cppqt/classfile.h \
    cppqt/cppfile.h \
    cppqt/hfile.h \
    cppqt/htmlfile.h \
    cppqt/propertiesdialog.h \
    cppqt/qhpfile.h \
    cppqt/uifile.h \
    cppqt/utils.h \
    cppqt/vars.h \
    cppqt/ormkatakowi.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:INCLUDEPATH += ./
include(../../pluginsystem/pluginsystem.pri)
include(../../diagramitems/diagramitems.pri)
include(../../ormmanager/ormmanager.pri)

FORMS += \
    cppqt/propertiesdialog.ui
