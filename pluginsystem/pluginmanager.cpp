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

#include "pluginmanager.h"
#include <QDir>
#include <QMap>
#include <QPluginLoader>
#include "iplugin.h"
#include "ipluginorm.h"
#include <QDebug>

using namespace PluginSystem;
#ifdef Q_OS_WIN
const char PREFIX_PLUGINS[]="*.dll";
#else
const char PREFIX_PLUGINS[]="*.so";
#endif
class PluginManager::PluginManagerPrivate{
public:

	QStringList paths;
	QList<IPlugin *> m_plugins;
};

PluginManager *PluginManager::m_instance = NULL;

PluginManager::PluginManager():d(new PluginManagerPrivate)
{
	m_instance= this;
}

PluginManager::~PluginManager(){
	delete d;
}
void PluginManager::loadPlugins()
{
	foreach(QString directory,d->paths){
		const QDir dir(directory);
		foreach(QString file,dir.entryList(QStringList()<<PREFIX_PLUGINS,QDir::Files)){
			QMessageLogger(0, 0, 0).debug()<<"Cargando plugin :"<<file;

			const QString fpath =dir.absolutePath()+QDir::separator()+ file;
			QPluginLoader *loader = new QPluginLoader(fpath);
			QObject *plugin = loader->instance();
			if (plugin){
				IPlugin *pluginObject = qobject_cast<IPlugin *>(plugin);
				if(pluginObject){
					pluginObject->inicializar();
					d->m_plugins.append(pluginObject);
				}
			}
			else{
				QMessageLogger(0, 0, 0).debug()<<"error:"<<loader->errorString();
			}
		}
	}
}

PluginManager *PluginManager::instance()
{
	return m_instance;
}

void PluginManager::addPath(QString path)
{
	d->paths.append(path);
}

QStringList PluginManager::paths() const
{
	return d->paths;
}

QList<PluginSystem::IPlugin *> PluginSystem::PluginManager::loadedPlugins()
{
	return d->m_plugins;
}
