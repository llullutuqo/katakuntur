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

QMAKE_TARGET_COMPANY = JPR Mazuelos Saavedra
QMAKE_TARGET_PRODUCT = KataKuntur
QMAKE_TARGET_DESCRIPTION = Data Modeling Tool
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2010 - 2017 Jean Mazuelos http://jeanmazuelos.com/contact
VERSION=2.5.4
DEFINES += "KK_VERSION=\\\"$$VERSION\\\""
win32:DATE_TIME = $$system(date /T) $$system(time /T)
unix:DATE_TIME = $$system(date +%d/%m/%Y) $$system(date +%H:%M:%S)
DEFINES += "KK_DATE=\"\\\"$$DATE_TIME\\\"\""
DEFINES += "KK_PAGE=\"\\\"http://katakuntur.jeanmazuelos.com\\\"\""
DEFINES += "AUTOR_PAGE=\"\\\"http://jeanmazuelos.com\\\"\""
