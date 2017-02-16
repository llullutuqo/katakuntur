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

#ifndef LOADSAVEDIAGRAM_H
#define LOADSAVEDIAGRAM_H

#include <QObject>
#include <QDomDocument>
#include <QTextDocument>
#include "diagramitems_global.h"
class Entity;
class SelfRelacion;
class Relacion;
class PantallaDiagrama;
class BaseItem;

class DIAGRAMITEMSSHARED_EXPORT LoadSaveProject : public QObject
{
	Q_OBJECT
public:
	LoadSaveProject();
	static bool guardar(const QString &fileName,PantallaDiagrama *pantalla);
	static bool exportaraPNG(QString &filename,PantallaDiagrama *pantalla);
	static bool exportarMapeoODT(QString filename,PantallaDiagrama * pantalla,int ejemplos=4);
	static bool exportarDiccinarioDatosODT(QString filename, PantallaDiagrama * pantalla, const QString &format);
	static bool exportarMapeoPNG(QString filename,PantallaDiagrama * pantalla,int ejemplos=4);
	static bool exportarYAML(QString filename,PantallaDiagrama * pantalla);

	static QTextDocument * generarMapeo(PantallaDiagrama *pantalla,int ejemplos=4);
	static QString generarDiccionarioDatos(PantallaDiagrama *pantalla);

	static PantallaDiagrama *cargar(const QString &fileName);

	static QDomElement itemToXml(BaseItem *item,QDomDocument doc,int version=versionActual);
	static BaseItem * itemFromXml(QDomElement element,PantallaDiagrama *pantalla,int version=versionActual);


	static QDomElement TablaToXml(Entity*, QDomDocument doc);
	static QDomElement RelacionToXml(Relacion*,QDomDocument doc);
	static QDomElement SelfRelacionToXml(SelfRelacion*,QDomDocument doc);

	static Entity *tablaFromXml(QDomElement element,int version=versionActual);
	static Relacion *relacionFromXml(QDomElement element,PantallaDiagrama *document,int version=versionActual);
	static SelfRelacion *selfRelacionFromXml(QDomElement element,PantallaDiagrama *document,int version=versionActual);

	static const int versionActual=8;
	static QString lastError();
private:
	static QString m_lastError;
};

#endif // LOADSAVEDIAGRAM_H
