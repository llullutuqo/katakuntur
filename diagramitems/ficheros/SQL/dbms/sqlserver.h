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

#ifndef SQLSERVER_H
#define SQLSERVER_H
#include <ficheros/SQL/sqlcore.h>
class SQLServer : public SQLCore
{
public:
	SQLServer();
	 static const char *staticNombreDBMS() { return "Microsoft SQLServer"; }
	 virtual const char *nombreDBMS() { return staticNombreDBMS(); }
	 static const char *staticQTDriver() { return "QODBC"; }
	 virtual const char *qtDriver(){return staticQTDriver();}
	 static const char *comentario() { return "-- "; }
	 virtual const char *comentarioSQL(){return comentario();}

	 static const char *staticId(){ return "4";}
	 virtual const char *id(){return staticId();}

	 virtual const char *quoteInitial() {return "[";}
	 virtual const char *quoteIFinal() {return "]";}

	 virtual void generarTablas(QStringList &lista, PantallaDiagrama *, const QString &prefijoTabla, const CaseTable &caseTable);
	 virtual QString tipoDatoLLaveFoanea(QString cadena);
	 virtual bool isFileDataBase(){return false;}
	 virtual QString toStandarQtType(QString sqlType);
	 bool isAutoincrementDataType(QString );

	 virtual bool checkCountAutoIncrement();
	 virtual bool checkAutoIncrementKey();
	 virtual bool createDataBase();
};

#endif // SQLSERVER_H
