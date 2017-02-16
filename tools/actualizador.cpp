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

#include "actualizador.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QUrl>
#include <QStringList>
#ifdef QT_DEBUG
#include <QDebug>
#endif
class Actualizador::ActualizadorPrivate{
public:
	ActualizadorPrivate(Actualizador * base){
		qnam = new QNetworkAccessManager(base);
	}

	QNetworkAccessManager *qnam;
};

Actualizador::Actualizador(QObject * parent) :
	QObject(parent),
	d(new ActualizadorPrivate(this))
{
}

Actualizador::~Actualizador()
{
	delete d;
}
void
Actualizador::searchUpdate(){
	connect(d->qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finalizado(QNetworkReply*)) );
	d->qnam->get(QNetworkRequest(QUrl("http://utils.jeanmazuelos.com/updates/katakuntur.xml")));
}

void
Actualizador::finalizado(QNetworkReply * reply)
{
	QDomDocument doc;
	if(doc.setContent(reply->readAll())){
		QDomElement base=doc.firstChildElement("app");
		QDomElement version = base.firstChildElement("version");
		QStringList verGeted =version.text().split('.');
		QStringList verAct = QString(KK_VERSION).split('.');
		bool first = true;
		for(int i =0; i <3;i++){
			QString a = verGeted.at(i), b= verAct.at(i);
			int va = a.toInt();
			int vb = b.toInt();
			if(!first){
				if(a.length() == 1)
					va = va*10;

				if(b.length() == 1)
					vb = vb *10;
				first= true;
			}
#ifdef QT_DEBUG
			qDebug()<<va<<":"<<vb;
#endif
			if(va > vb){
				emit getNewVersion(version.text(),base.firstChildElement("description").text());
				return;
			}
			else if(va < vb){
				break;
			}
			first = false;
		}
	}
}
