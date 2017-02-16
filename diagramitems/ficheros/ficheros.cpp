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

#include "ficheros.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Ficheros::Ficheros(QObject *parent):QObject(parent)
{

}
bool Ficheros::exportar(const QStringList &lista, const QString &fileName){
	exportar(listaToString(lista),fileName);
	return true;
}
bool Ficheros::exportar(const QString &texto, const QString fileName, const char* encode){
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "No se puede abrir el documento para escritura("<<fileName<<"): "<< qPrintable(file.errorString());
		return false;
	}

	QTextStream out(&file);
	out.setCodec(encode);
	out << texto;
	return true;
}
QString Ficheros::listaToString(const QStringList &lista){
	QString d ;
	d+=lista.at(0);
	for(int i=1;i<lista.count(); i++)
		d+= "\r\n"+ lista.at(i);
	return d;
}
bool Ficheros::save(QString dir){
	return exportar(m_textFile,dir +"/" + m_filename);
}

QString Ficheros::fileName(){
	return m_filename;
}

void Ficheros::setFileName(const QString fileName){
	if(m_filename!=fileName)
		m_filename = fileName;
}
