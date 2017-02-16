/****************************************************************************
 ** Kata Kuntur is Copyright (C) 2010 - 2017 Jean Mazuelos(LlulluTuqo)
 ** Contact   : http://jeanmazuelos.com/contact
 **
 ** This file is part of Kata Kuntur - Data Modelling Tool.
 ** LGPL V3 License Usage
 **
 ** You should use this file under the terms of the
 ** GNU LESSER GENERAL PUBLIC LICENSE - https://www.gnu.org/licenses/lgpl-3.0.txt
 **
 ** Additionally, any Redistributions of source code must retain the above
 ** copyright notice, this condition and the following disclaimer.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ****************************************************************************/

#include "sqladminplugin.h"
#include <sqladmin.h>
#include <QWidget>
#include <QtPlugin>
#include <QAction>
#include <QDesktopServices>
#include <QDir>
#include <propiedades.h>

SQLAdminPlugin::SQLAdminPlugin(QObject *parent):
	IPluginWidget(parent)
{
}

SQLAdminPlugin::~SQLAdminPlugin(){
	delete m_baseWidget;
	delete m_action;
}



bool SQLAdminPlugin::inicializar()
{
	QString p = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
	QDir dr(p);
	if(!QFile::exists(p)){
		dr.mkpath(p);
	}
	Propiedades::DataDirectory = p + "/";
	m_baseWidget = new SQLAdmin();
	m_action = new QAction(QIcon(":/icons/icon.png"),tr("Administrador de consultas"),NULL);
	return true;
}

bool SQLAdminPlugin::inicializado()
{
	return true;
}

QWidget * SQLAdminPlugin::widget()
{
	return m_baseWidget;
}

//Q_EXPORT_PLUGIN(SQLAdminPlugin)

QAction * SQLAdminPlugin::action()
{
	return m_action;
}
