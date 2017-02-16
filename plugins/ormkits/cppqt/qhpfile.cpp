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

#include <QDomDocument>
#include "qhpfile.h"
#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDir>
struct Sections{
	QString title;
	QString file;
};

class QhpFile::QhpFilePrivate{
public :
	QList<Sections> filesList;
};

QhpFile::QhpFile():d(new QhpFilePrivate)
{
}
bool QhpFile::save(QString dir){
	QDomDocument doc;
	QDomProcessingInstruction xml = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(xml);

	QDomElement helpProjet = doc.createElement("QtHelpProject");
	helpProjet.setAttribute("version","1.0");

	QDomElement item = doc.createElement("namespace");
	item.appendChild(doc.createTextNode("com.help"));
	helpProjet.appendChild(item);

	item = doc.createElement("virtualFolder");
	item.appendChild(doc.createTextNode("help"));
	helpProjet.appendChild(item);

	QDomElement filter= doc.createElement("filterSection");

	QDomElement toc = doc.createElement("toc");
	QDomElement sectionbase = doc.createElement("section");
	QDomElement files =doc.createElement("files");
	sectionbase.setAttribute("title",QObject::trUtf8("Ventanas"));
	for(int i =0;i<d->filesList.count();i++){
		Sections s = d->filesList.value(i);
		QDomElement sect = doc.createElement("section");
		sect.setAttribute("title",s.title);
		sect.setAttribute("ref","./"+s.file);
		sectionbase.appendChild(sect);
		QDomElement file =doc.createElement("file");
		file.appendChild(doc.createTextNode(s.file));
		files.appendChild(file);
	}	
	toc.appendChild(sectionbase);
	filter.appendChild(toc);
	QDomElement css = doc.createElement("file");
	css.appendChild(doc.createTextNode("normal.css"));
	files.appendChild(css);
	QDomElement im=doc.createElement("file");
	im.appendChild(doc.createTextNode("images/*"));
	files.appendChild(im);
	filter.appendChild(files);
	helpProjet.appendChild(filter);
	doc.appendChild(helpProjet);
	QString filepath=dir+"help.qhp";
	QFile file(filepath);
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream stream(&file);
		doc.save(stream, 2);
		file.close();
	}
	else{
		qDebug()<<"No se puede escribir el fichero("<<filepath<<"), compruebe que tiene permisos de escritura.";
		return false;
	}
		QFile::copy("../plantillas/css/normal.css",dir +"normal.css");
	return true;
}
void QhpFile::addFile(QString title,QString file){
	Sections s;
	s.title = title;
	s.file = file;
	d->filesList.append(s);
}
