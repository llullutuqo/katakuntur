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

#include <QApplication>
#include "mainwindow.h"
#include <QTextStream>
#include <QBitmap>
#include <QTranslator>
#include <QSplashScreen>
#include <QObject>
#include <QSettings>
#include <QTranslator>

#include "propiedadesdiagramador.h"
#include "pluginmanager.h"
#include "ormmanager.h"

bool loadLanguage(const QString lang)
{
	QString language=lang;
	QTranslator * traductor = new QTranslator();
#ifdef WIN32
	const QString dir =qApp->applicationDirPath() + "\\..\\lang";
#else
	const QString dir =qApp->applicationDirPath() + "/../lang";
#endif
	bool isLoaded=false;

	if(language!="es"){
		const QString file ="lang_"+language;
		isLoaded=traductor->load(file, dir);
	}
	else{
		traductor->load("");
		isLoaded=true;
	}

	//en caso de no cargar algun lenguaje anterior se itentara cargar en ingles.
	if(!isLoaded){
		isLoaded=traductor->load("lang_en",dir);
		language="en";
	}

	if(isLoaded){
		qApp->installTranslator(traductor);
	}
	else
		language = "es";


	QSettings settings;
	settings.setValue("language",language);

	return isLoaded;
}

void loadDefaultLanguage(){
	QSettings settings;
	QString locale = settings.value("language").toString();
	if(locale.isEmpty()){
		locale= QLocale::system().name();
		locale.truncate(locale.lastIndexOf("_"));
	}
	loadLanguage(locale);
}
void catchLogMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	switch (type) {
	case QtDebugMsg:
		fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtInfoMsg:
		fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtWarningMsg:
		fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtCriticalMsg:
		fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtFatalMsg:
		fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		abort();
	}
}

int main(int argc, char *argv[])
{
	qInstallMessageHandler(catchLogMessages);
	QApplication a(argc, argv);
	a.addLibraryPath(a.applicationDirPath() +"/../lib/plugins/");

	a.setApplicationName("Kata Kuntur");
	a.setOrganizationName("Jean Pierre Ruedi Mazuelos Saavedra");
	a.setApplicationVersion(KK_VERSION);

	QSplashScreen * splash = new QSplashScreen();
	QPixmap pixmap(":/icons/images/splash.png");

	splash->setPixmap(pixmap);
	splash->setMask(pixmap.mask());
	splash->show();
	a.processEvents();

	loadDefaultLanguage();


	Qt::Alignment pos= Qt::AlignLeft|Qt::AlignBottom;

	splash->showMessage(QObject::trUtf8("Cargando plugins......"),pos,Qt::white);
	PluginSystem::PluginManager p;
	p.addPath(a.applicationDirPath() +"/../plugins");
	p.loadPlugins();

	ORMManagerLib::ORMManager oManager;
	//TODO: Load ORM plugins here

	splash->showMessage(QObject::trUtf8("Cargando configuración......"),pos,Qt::white);
	PropiedadesDiagramador::cargarConfiguracion();

	/*splash->showMessage(QObject::trUtf8("Cargando skin......"),pos,Qt::white);
	QFile f(":/skin/style.css");
	f.open(QFile::ReadOnly);
	a.setStyleSheet(f.readAll());*/

	splash->showMessage(QObject::tr("Cargando ventana principal......"),pos,Qt::white);
	MainWindow w;
	if(a.arguments().count()==2){
		splash->showMessage(QObject::tr("Cargando diagrama......"),pos,Qt::white);
		w.abrirFichero(a.arguments().at(1));
	}
	w.show();

	splash->finish(&w);
	delete splash;
	return a.exec();
}
