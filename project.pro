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

TEMPLATE = subdirs
#contains(CONF, items):
#contains(CONF, gui):
#LIBRARIES
SRC_SUBDIRS += src_widgets
SRC_SUBDIRS += src_sqlbase
SRC_SUBDIRS += src_diagramitems
SRC_SUBDIRS += ormmanager
SRC_SUBDIRS += src_tools
SRC_SUBDIRS += pluginsystem \
    plugins
#EXECUTABLES
SRC_SUBDIRS += src_gui

SUBDIRS += $$SRC_SUBDIRS

src_widgets.subdir = widgets
src_widgets.target = sub-widgets
src_sqlbase.subdir = sqlbase
src_sqlbase.target = sub-sqlbase
src_diagramitems.subdir = diagramitems
src_diagramitems.target = sub-diagramitems
src_tools.subdir = tools
src_tools.target = sub-tools
src_gui.subdir = gui
src_gui.target = sub-gui

unix { 
	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}
	libs.path =$$PREFIX/lib/
	libs.files = bin/*.so*

	langs.path = $$PREFIX/share/katakuntur/locale/
	langs.files = bin/lang/*

	target.path = $$PREFIX/bin
	target.files = bin/katakuntur

	short.path = $$PREFIX/share/applications/
	short.files = instaladores/archivos/Otros/kata-kuntur.desktop

	img.path = $$PREFIX/share/pixmaps/
	img.files = instaladores/archivos/Iconos/kata-kuntur.png

	mime.path =$$PREFIX/share/mime/packages/
	mime.files = instaladores/archivos/Otros/x-diagram-qcm.xml instaladores/archivos/Otros/x-diagram-kkd.xml

	mimeicons.path = $$PREFIX/share/icons/hicolor/scalable/mimetypes/
	mimeicons.files = instaladores/archivos/Iconos/application-x-diagram-kkd.png instaladores/archivos/Iconos/application-x-diagram-qcm.png

	INSTALLS += langs \
		libs \
		target \
		short \
		img \
		mime \
		mimeicons
}
