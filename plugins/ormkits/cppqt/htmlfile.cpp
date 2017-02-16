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

#include "htmlfile.h"
struct Atributes{
	QString atributName;
	QString atributDescription;
};
class HtmlFile::HtmlFilePrivate{
public:
	QList<Atributes> lista;
	QList<Atributes> seeAlso;
	static QString subtitle;
	static QString parragraf;
	QString pageTitle;
	QString name;
};
QString HtmlFile::HtmlFilePrivate::subtitle ="<h2>%1</h2>";
QString HtmlFile::HtmlFilePrivate::parragraf="<p>%1</p>";
HtmlFile::HtmlFile(QObject *parent):Ficheros(parent),d(new HtmlFilePrivate)
{
}
void HtmlFile::addAtribute(QString atributeName,QString description){
	if(description.isEmpty())
		description = trUtf8("No hay descripción disponible");
	Atributes at;
	at.atributName = atributeName;
	at.atributDescription = description;
	d->lista.append(at);
}
void HtmlFile::addSeeAlso(QString text,QString file){
	Atributes a;
	a.atributName=text;
	a.atributDescription =file;
	d->seeAlso.append(a);
}

QString HtmlFile::pageTitle(){
	return d->pageTitle;
}

void HtmlFile::setPageTitle(QString title){
	if(d->pageTitle!=title){
		d->pageTitle = title;
	}
}
void HtmlFile::setName(QString value){
	d->name=value;
	setFileName(value + ".html");
}

bool HtmlFile::save(QString dir){
	QString text;
	text +="<html>";
	text +="<head>";
	text +="<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">";
	text +="<link rel=\"stylesheet\" type=\"text/css\" media=\"all\" href=\"normal.css\">";
	text +="<title>"+d->pageTitle+"</title>";
	text +="</head>";
	text +="<body class=\"home\">";
	text +="<div id=\"head\">";
	text +="<center><h1>"+d->pageTitle+"</h1></center>";
	text +="</div>";
	text +="<hr >";
	text +="<div id=\"imagen\">";
	text +="	<center><img src=\"images/"+d->name+".png\"></center>";
	text +="</div>";
	text +="<hr >";
	text +="<div id=\"content\">";
	for(int i =0;i<d->lista.count();i++){
		Atributes at =d->lista.value(i);
		text += d->subtitle.arg(at.atributName);
		text += d->parragraf.arg(at.atributDescription);
	}
	if(d->seeAlso.count()>0){
		text +="<hr >";
		QString a;
		for(int i=0;i<d->seeAlso.count();i++){
			Atributes at = d->seeAlso.value(i);
			a+="<a href=\""+at.atributDescription+"\">"+at.atributName+"</a>,";
		}
		a =a.left(a.length() -1);
		text +="<p>"+QObject::trUtf8("Véa También:")+ a +"</p>";
	}
	text +="</div>";
	text +="</body>";
	text +="</html>";
	exportar(text,dir + fileName());
	return true;
}
