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

#include "sqlcore.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#ifdef QT_DEBUG
#include <QDebug>
#endif
#include "model/entityattibute.h"
#include "pantalladiagrama.h"
#include "model/entity.h"
#include <QDateTime>

class SQLCore::SQLCorePrivate{
public:
	SQLCorePrivate(){
		alterCount = 0;
		m_useQuotes = true;
	}
	bool verificarTipoDato(const QString &tipodato,QStringList &listaItems,QStringList &lista, const QString &databaseName,const QString &columnName)const {
		QString dato = tipodato;
		if(tipodato.length()<=0){
			lista.append(QObject::trUtf8("Debe especificar un tipo de dato para el atributo."));
			return false;
		}
		dato = dato.toLower();
		if(dato.contains("(n)")){
			lista.append(QObject::trUtf8("Debe reemplazar el caracter n por un valor numérico en el tipo de dato(%1) del atributo %2.").arg(dato).arg(columnName));
			return false;
		}

		for(int i =0; i< listaItems.count();i++){
			QString m_tipo =listaItems.at(i).toLower();

			if(m_tipo==dato)
				return true;
			//Verificamos si el tipo de dato contiene ()
			if(m_tipo.contains("(n)")){
				QString a=dato.left(tipodato.indexOf("(")),b=m_tipo.left(m_tipo.indexOf("("));
	#ifdef QT_DEBUG
				qDebug()<<a<<"=="<<b;
	#endif
				if(a==b){
					QString exp =b+"\\([0-9]+\\)";
					QRegExp reg(exp);
					//reg.setPatternSyntax(QRegExp::Wildcard);
					if(reg.exactMatch(dato))
						return true;
					else{
						lista.append(QObject::trUtf8("Debe especificar una cantidad para el tipo de dato %1, ejemplo %1(2).").arg(a.toUpper()));
	#ifdef QT_DEBUG
				qDebug()<<reg.isValid()<<" No concuerda falta el indice:"<<a<<"=="<<b;
	#endif
						return false;
					}
				}
			}
		}
		lista.append(QObject::trUtf8("El tipo de dato %1 no es soportado por %2.").arg(tipodato).arg(databaseName));
		return false;
	}
	int alterCount;
	bool m_useQuotes;
};

SQLCore::SQLCore():
	d(new SQLCorePrivate())
{
	ejecutar_por_comas = false;
	m_limiteCaracteres = 60;
}

SQLCore::~SQLCore()
{
	delete d;
}

QStringList SQLCore::generarSQL(PantallaDiagrama *pn, const QString &prefijoTabla, const CaseTable &caseTable, bool &ok){
	QStringList list;
	list.insert(0,QString(this->comentarioSQL()) + tr(" Creado con Kata Kuntur - Modelador de Datos"));
	list.insert(1,QString(this->comentarioSQL()) + tr(" Versión: %1").arg(KK_VERSION));
	list.insert(2,QString(this->comentarioSQL()) + tr(" Sitio Web: %1").arg("http://katakuntur.jeanmazuelos.com/"));
	list.insert(3,QString(this->comentarioSQL()) + tr(" Si usted encuentra algún error le agradeceriamos lo reporte en:"));
	list.insert(4,QString(this->comentarioSQL()) + tr(" http://pm.jeanmazuelos.com/katakuntur/issues/new"));
	list.insert(5,QString());
	list.insert(6,QString(this->comentarioSQL()) + tr(" Administrador de Base de Datos: %1").arg(pn->dbms()->nombreDBMS()));
	list.insert(7,QString(this->comentarioSQL()) + tr(" Diagrama: %1").arg(pn->nombreDiagrama()));
	list.insert(8,QString(this->comentarioSQL()) + tr(" Autor: %1").arg(pn->nombreAutor()));
	list.insert(9,QString(this->comentarioSQL()) + tr(" Fecha y hora: %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy H:mm:ss")));

	generarTablas(list,pn,prefijoTabla,caseTable);
	/*Se determina si existen relaciones para crear, en caso de no HABER no se genern*/
	if(!m_alterTable.isEmpty()){
		list.append("");
		list.append(QString(this->comentarioSQL()) + tr("GENERANDO RELACIONES"));
		list.append(m_alterTable);
	}
	ok =true;
	return list;
}
QString SQLCore::generarBaseDatos( QString nombreBaseDatos){
	return "CREATE DATABASE " +nombreBaseDatos+";";
}
void SQLCore::generarTablas(QStringList &lista, PantallaDiagrama *, const QString &prefijoTabla, const CaseTable &caseTable){
	Q_UNUSED(prefijoTabla)
	Q_UNUSED(caseTable)
	lista.append("");
	lista.append(QString(this->comentarioSQL()) + tr("GENERANDO TABLAS"));
	d->alterCount =1;
}

void SQLCore::exportarScript(PantallaDiagrama * pantalla, QString filename,QString nombreBaseDatos, const QString &prefijoTabla, const CaseTable &caseTable){
	Q_UNUSED(nombreBaseDatos);
	bool ok;
	QStringList tx = generarSQL(pantalla,prefijoTabla,caseTable,ok);
	if(ok){
		this->exportar(tx,filename);
	}
}
QString SQLCore::generarSQL(PantallaDiagrama* pn, const QString &prefijoTabla, const CaseTable &caseTable){
	bool ok;
	return this->listaToString(this->generarSQL(pn,prefijoTabla,caseTable,ok));
}
void SQLCore::exportarDBMS(QString sql, QString servidor, QString usuario, QString contrasenia, QString baseDatos, int port, bool generarBaseDatos){
#ifdef QT_DEBUG
	qDebug()<<ejecutar_por_comas;
#endif
	QString connectionName;
	if(!QSqlDatabase::isDriverAvailable(this->qtDriver())){
		emit onMessage(trUtf8("%1El driver %2 no está disponible.").arg("E: ").arg(this->qtDriver()));
		return;
	}
	if(generarBaseDatos && baseDatos.isEmpty()){
		emit onMessage(trUtf8("%1Si la opción generar base de datos está activada, se debe proveer un nombre de base de datos.").arg("E: "));
		return;
	}
	emit onMessage(trUtf8("%1 Exportando a %2 ...").arg("I: ").arg(nombreDBMS()));
	bool m_useODBC = false;
	if(QString(qtDriver())== QString("QODBC"))
		m_useODBC = true;

	bool conti = true;
	{
		QSqlDatabase basedatos = QSqlDatabase::database();
		if(basedatos.isValid())
			basedatos.close();
		basedatos= QSqlDatabase::addDatabase(this->qtDriver());
		connectionName = basedatos.connectionName();

		if(!usuario.isEmpty())
			basedatos.setUserName(usuario);
		if(!contrasenia.isEmpty())
			basedatos.setPassword(contrasenia);
		if(!servidor.isEmpty()&& !m_useODBC)
			basedatos.setHostName(servidor);
		if(port!=0)
			basedatos.setPort(port);
		if(m_useODBC)
			basedatos.setDatabaseName(servidor);
		if(!isFileDataBase() && generarBaseDatos){
			if (!basedatos.open()){
				emit onMessage("B: "+ QString(nombreDBMS()) +" Error : " +basedatos.lastError().databaseText());
				conti = false;
			}
			else if(!this->ejecutarQuery(this->generarBaseDatos(baseDatos)))
				conti=false;
			basedatos.close();
		}
		if(conti){
			conti = true;
			if(!m_useODBC)
				basedatos.setDatabaseName(baseDatos);
			if (!basedatos.open()){
				emit onMessage(trUtf8("%1 %2 Error al intentar conectar : ").arg("B: ").arg(nombreDBMS())+basedatos.lastError().databaseText());
				conti=false;
			}
			if(conti){
				if(m_useODBC){
					/*se agrega el comando use en caso de usar odbc*/
					sql.insert(0,"use " + baseDatos);
				}
				if(ejecutar_por_comas){
					QStringList ls = sql.split(";");
					for(int i =0;i< ls.count();i++){
						if(ls.at(i).isNull())
							continue;
						if(!this->ejecutarQuery(ls.at(i))){
							conti = false;
							break;
						}
					}
				}
				else{
					if(!this->ejecutarQuery(sql))
						conti = false;
				}
			}
		}
		basedatos.close();
	}

	QSqlDatabase::removeDatabase(connectionName);
	if(conti)
		emit onMessage(trUtf8("%1Exportación finalizada.").arg("S: "));
	else
		emit onMessage(trUtf8("%1La exportación no finalizó correctamente, revise los mensajes anteriores para más detalles.").arg("E: "));
}
bool SQLCore::ejecutarQuery(const QString &string){
	//Se verifica que el texto a exportar no sea vacío
	QString text = string;
	text = text.replace(QString("\r"),QString("")).replace(QString("\n"),QString(""));
	if(text.isEmpty())
		return true;

	QSqlQuery sqla(string);
	emit onMessage(tr("Query : ") + string);
	if(sqla.lastError().isValid()){
		emit onMessage(tr("%1 %2 Error : ").arg("B: ").arg(nombreDBMS()) +sqla.lastError().databaseText());
		return false;
	}
	else{
		emit onMessage(tr("%1Ejecutado correctamente.").arg("S: "));
	}
	return true;
}


void
SQLCore::inspectDiagram(PantallaDiagrama * pantalla){
	foreach(Entity *item, pantalla->itemsByType<Entity>()){
		int autoIncrement = 0;
		int primaryCount =0;
		QStringList errorEnt;
		const QString limitChar = QString::number(m_limiteCaracteres);
		if(item->nombre().length()>m_limiteCaracteres)
			errorEnt<<trUtf8("El nombre de la  entidad \"%1\" excede el tamaño de %2 para nombres de objeto. Debe acortar el nombre de la entidad.").arg(item->nombre()).arg(limitChar);

		foreach(EntityAttibute *col, item->columnas()){
			/*Si la columna a analizar es llave primaria se le agrega al conteo sin importar si es llave foranea*/
			if(col->isPrimaryKey())
				primaryCount++;


			if(col->isForeignKey()){
				if(col->name().length() > m_limiteCaracteres){
					QStringList af;
					af<< trUtf8("El nombre de la llave foránea \"%1\" excede el tamaño de %2 para nombres de objeto. Debe modificar la fórmula del diagrama o acortar el nombre del atributo origen de la llave foránea.").arg(col->name()).arg(limitChar);
					emit onColumnError(col,af);
				}
				continue;
			}

			QStringList a;
			d->verificarTipoDato(col->dataType(),m_listatipos,a,nombreDBMS(),col->name());


			if(!col->isPrimaryKey()){
				if(checkAutoIncrementKey() && isAutoincrementDataType(col->dataType())){
					a << trUtf8("El atributo %1 tiene un tipo de dato Auto Incremental, en el DBMS %2 solo los atributos de llaves primarias(Clave de unicidad) pueden tener valores autoincrementales.").arg(col->name()).arg(nombreDBMS());
				}
			}

			if(checkCountAutoIncrement() && isAutoincrementDataType(col->dataType())){
				autoIncrement++;
			}
			if(!a.isEmpty()){
				emit onColumnError(col,a);
			}
		}
		if(primaryCount<1)
			emit onEntidadError(item,QStringList()<<tr("La Entidad %1 no tiene designado un atributo como \"Clave de unicidad\"").arg(item->nombre()));
		if(primaryCount>1 && autoIncrement > 0)
			emit  onEntidadError(item,QStringList()<<tr("Inconsistencia de data, en la entidad %1 se tiene una llave primaria compuesta por un atributo autoincremental. Cuando la llave primaria es compuesta y uno de los atributos es AUTOINCREMENTAL puede generar errores inprevistos a la base de datos.").arg(item->nombre()));
		if(checkCountAutoIncrement()){
			if(autoIncrement>1)
				errorEnt <<tr("La entidad %1 no puede tener 2 atributos con tipo de dato autoincremental.").arg(item->nombre());
		}

		if(errorEnt.count()>0)
			emit onEntidadError(item,errorEnt);
	}
}

void SQLCore::setQuotes(bool value)
{
	d->m_useQuotes = value;
}

bool SQLCore::useQuotes()
{
	return d->m_useQuotes;
}

int SQLCore::limiteCaracteresNombre()
{
	return m_limiteCaracteres;
}

const QString SQLCore::generateAlterName(EntityAttibute *col)
{
	const QString numero = QString::number(d->alterCount);
	QString cols =col->tabla()->nombre() +"_"+col->foreignTable()->nombre() + "_" + col->name();
	if(cols.length()>m_limiteCaracteres)
		cols = cols.left(m_limiteCaracteres - numero.length()) + numero;
	d->alterCount++;
	return makeObjectName(cols.toLower());
}

const QString SQLCore::generateTableName(const QString &name, const QString &prefijoTabla, const SQLCore::CaseTable &caseTable)
{
	QString tableName=prefijoTabla.isNull()|prefijoTabla.isEmpty()? name:prefijoTabla + name;
	if(caseTable==Minusculas)
		tableName = tableName.toLower();
	else if(caseTable == Mayusculas)
		tableName = tableName.toUpper();
	return makeObjectName(tableName);
}

const QString SQLCore::delLastChar(const QString chr, const QString value)
{
	QString val = value;
		if(val.endsWith(chr))
			val = val.left(val.count() -1);
		return val;
}
/*!
 * \brief SQLCore::makeObjectName
 * \return
 */
const QString SQLCore::makeObjectName(const QString name)
{
	QString a = name;
	if(useQuotes()){
		a = quoteInitial() + a + quoteIFinal();
	}
	return a;
}
