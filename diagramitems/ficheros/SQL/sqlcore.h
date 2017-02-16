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

#ifndef SQLCORE_H
#define SQLCORE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "ficheros/ficheros.h"
#include "diagramitems_global.h"

class PantallaDiagrama;
class Entity;
class EntityAttibute;
class DIAGRAMITEMSSHARED_EXPORT SQLCore : public Ficheros
{
	Q_OBJECT
	Q_PROPERTY(bool Quotes READ useQuotes WRITE setQuotes NOTIFY quotesChanged)

public:
	SQLCore();
	~SQLCore();
	enum CaseTable{Normal=0,Mayusculas=1,Minusculas=2};

	static const char *staticNombreDBMS() { return ""; }
	virtual const char *nombreDBMS() = 0;
	static const char *staticQTDriver() { return ""; }
	virtual const char *qtDriver() = 0;
	static const char *comentario() { return ""; }
	virtual const char *comentarioSQL() = 0;

	static const char *staticId(){ return "";}
	virtual const char *id() = 0;

	virtual const char *quoteInitial() = 0;
	virtual const char *quoteIFinal() = 0;

	virtual void exportarScript(PantallaDiagrama * pantalla, QString filename,QString nombreBaseDatos, const QString &prefijoTabla, const CaseTable &caseTable);
	virtual void exportarDBMS(QString sql,QString servidor,QString usuario,QString contrasenia,QString baseDatos,int port=0,bool generarBaseDatos=false);
	QStringList getListaTipos() const {return m_listatipos;}
	virtual void generarTablas(QStringList &lista,PantallaDiagrama*, const QString &prefijoTabla, const CaseTable &caseTable);
	virtual QString generarBaseDatos(QString nombreBaseDatos);
	virtual QString tipoDatoLLaveFoanea(QString cadena)=0;
	virtual QStringList generarSQL(PantallaDiagrama*, const QString &prefijoTabla, const CaseTable &caseTable,bool &ok);
	virtual QString generarSQL(PantallaDiagrama*, const QString &prefijoTabla, const CaseTable &caseTable);
	virtual bool suportsCreateDataBase(){return true;}
	virtual bool isFileDataBase(){return false;}
	virtual QString toStandarQtType(QString sqlType)=0;
	virtual bool isAutoincrementDataType(QString )=0;
	void inspectDiagram(PantallaDiagrama * pantalla);

	/*Propiedades*/
	virtual void setQuotes(bool value);
	virtual bool useQuotes();

	virtual bool checkCountAutoIncrement() =0;
	virtual bool checkAutoIncrementKey() =0;
	virtual bool createDataBase()=0;

	int limiteCaracteresNombre();
protected:
	const QString generateAlterName(EntityAttibute *col);
	const QString generateTableName(const QString &tableName, const QString &prefijoTabla, const CaseTable &caseTable);
	const QString delLastChar(const QString chr, const QString value);
	const QString makeObjectName(const QString name);
	bool ejecutarQuery(const QString &string);
	QString m_nombre;
	QStringList m_listatipos;
	QStringList m_alterTable;
	bool ejecutar_por_comas;
	QString m_cuotes_inital;
	QString m_cuotes_final;
	int m_limiteCaracteres;
private:
	class SQLCorePrivate;
	SQLCorePrivate * d;
signals:
	void onColumnError(EntityAttibute * col,QStringList message);
	void onEntidadError(Entity * col,QStringList message);
	void quotesChanged(const bool value);
};

#endif // SQLCORE_H
