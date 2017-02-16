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

#ifndef IPLUGINORM_H
#define IPLUGINORM_H
#include <pluginsystem_global.h>
#include "iplugin.h"

class PantallaDiagrama;
namespace PluginSystem{
class PLUGINSYSTEMSHARED_EXPORT IPluginORM : public IPlugin
{
	Q_OBJECT
public:
	IPluginORM(QObject *parent = 0);
	static const char *staticNombreORM() { return ""; }
	virtual const char *nombreORM() = 0;
	static const char *staticLanguageORM() { return ""; }
	virtual const char *languageORM() = 0;
	virtual bool exportProject(const QString baseDir)=0;
	static const char *staticId(){ return "";}
	virtual const char *id() = 0;

public slots:
	void setDiagram(PantallaDiagrama * diagram);
protected:
	PantallaDiagrama *m_diagram;
};
}

#define IPluginORM_iid "katakuntur/plugins.IPluginORM"
Q_DECLARE_INTERFACE(PluginSystem::IPluginORM, IPluginORM_iid)
#endif // IPLUGINORM_H
