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

#ifndef CPPQT4_H
#define CPPQT4_H
#include "ormkit.h"

class ORMKataKowi : public ORMManagerLib::ORMKit
{
	Q_OBJECT
	Q_PROPERTY(QString nombreORM READ nombreORM)
public:
	ORMKataKowi(QObject *parent =0);
	~ORMKataKowi();
	static const char *staticNombreORM() { return "KataQowi (Qt5 C++)"; }
	virtual const char *nombreORM();
	static const char *staticLanguageORM() { return ""; }
	virtual const char *languageORM();
	virtual bool exportProject(PantallaDiagrama * diagram, const QString basedir);

	static const char *staticId(){ return "1";}
	virtual const char *id(){return staticId();}

	QString directoryBase();
	QString docDirectoryName();
	QString modelDirectoryName();
	QString widgetsDirectoryName();
	QString widgetsPrefix();
	QString target();

	/*Opciones de generacion*/
	bool generateDocumentation();
	bool generateWidgets();
	bool generateFullProject();
	bool generateQhcpFile();

	void setGenerateDocumentation(bool value);
	void setGenerateWidgets(bool value);
	void setGenerateFullProject(bool value);
	void setGenerateQhcpFile(bool value);

public slots:
	void setDirectoryBase(QString directory);
	void setDocDirectoryName(QString name);
	void setModelDirectoryName(QString name);
	void setWidgetsDirectoraName(QString name);
	void setWidgetsPrefix(QString prefix);
	void setTarget(QString target);
private:
	class ORMKataKowiPrivate;
	ORMKataKowiPrivate *d;
};

#endif // CPPQT4_H
