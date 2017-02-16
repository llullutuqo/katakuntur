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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <QClipboard>
#include <QSettings>
#include <QTextStream>
#include "baseitem.h"
#include "itemmovible.h"
#include "widgets/formtabla.h"
#include "widgets/formrelacion.h"
#include <QPrintPreviewDialog>
#include <math.h>
#include <QImage>
#include <QPainter>
#include <QTranslator>
#include "formpantalladiagrama.h"
#include <formabout.h>
#include <QComboBox>
#include "formpropiedades.h"
#include "loadsaveproject.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMimeData>
#include <QProcess>
#include <QGraphicsView>
#include <actualizador.h>
#include <pluginmanager.h>
#include <ipluginwidget.h>
#include <wgbienvenida.h>
#include <QDate>
#include <QUndoStack>

using namespace PluginSystem;

bool MainWindow::autoCargarUltimoDiagrama=false;
QString MainWindow::title=QString();

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	title = trUtf8("Kata Kuntur %1 - Modelador de Datos").arg(KK_VERSION);

	ui->setupUi(this);
	wg = new WgBienvenida(this);
	m_pantallaActual=NULL;
	m_ft =new FormTabla(this);
	m_fr =new FormRelacion(this);
	m_fsr = new FormSelfRelacion(this);
	m_inspect = new InspectDiagramWidget(this);
	m_inspect->setWindowFlags(Qt::Dialog);
	m_inspect->setWindowModality(Qt::WindowModal);

	//usar doble click huonnnnnn
	m_ft->setWindowFlags(Qt::Dialog);
	m_ft->setWindowModality(Qt::WindowModal);
	m_fr->setWindowFlags(Qt::Dialog);
	m_fr->setWindowModality(Qt::WindowModal);
	m_fsr->setWindowFlags(Qt::Dialog);
	m_fsr->setWindowModality(Qt::WindowModal);

	//Widgets para undo
	undoView = new QUndoView(ui->dockWidgetUndo);
	undoView->setCleanIcon(QIcon(":/icons/images/document-save.png"));
	ui->dockWidgetUndo->setWidget(undoView);
	//Estableciendo lo botones en un grupo
	m_ActionDiagrama = new QActionGroup(this);
	ui->actionDesplazar->setData(PantallaDiagrama::Ninguno);
	ui->actionSeleccionar->setData(PantallaDiagrama::Seleccionado);
	ui->actionAgregar_relacion->setData(PantallaDiagrama::AgregarRelacion);
	ui->actionAgregar_tabla->setData(PantallaDiagrama::AgregarEntidad);
	ui->actionAgregar_Vista->setData(PantallaDiagrama::AgregarVista);

	m_ActionDiagrama->addAction(ui->actionDesplazar);
	m_ActionDiagrama->addAction(ui->actionSeleccionar);
	m_ActionDiagrama->addAction(ui->actionAgregar_relacion);
	m_ActionDiagrama->addAction(ui->actionAgregar_tabla);
	m_ActionDiagrama->addAction(ui->actionAgregar_Vista);
	connect(m_ActionDiagrama, SIGNAL(triggered(QAction*)), this, SLOT(grupoActionTigerded(QAction*)));
	m_ActionDiagrama->setEnabled(false);

	m_ActionLanguage = new QActionGroup(this);
	ui->actionEnglish->setData("en");
	ui->actionEspa_ol->setData("es");
	m_ActionLanguage->addAction(ui->actionEnglish);
	m_ActionLanguage->addAction(ui->actionEspa_ol);
	connect(m_ActionLanguage,SIGNAL(triggered(QAction*)),this,SLOT(grupoActionLanguage(QAction*)));

	ui->tabWidget->setElideMode(Qt::ElideMiddle);
	m_propfr = new FormPantallaDiagrama(this);
	m_export = new FormExportDBMS(this);

	//Carga el combobox que muestra el modelo fisico-logico
	m_combo= new QComboBox();
	m_combo->addItem(trUtf8("Lógico"));
	m_combo->addItem(trUtf8("Físico"));
	connect(m_combo,SIGNAL(activated(int)),this,SLOT(comboChanged(int)));
	ui->mainToolBar->insertWidget(ui->actionZoom_Out,m_combo);
	m_combo->setEnabled(false);

	//Create a recent files menu
	for (int i = 0; i < MaxRecentFiles; ++i) {
		archivosRecientes[i] = new QAction(this);
		archivosRecientes[i]->setVisible(false);
		connect(archivosRecientes[i], SIGNAL(triggered()),this, SLOT(abrirArchivoReciente()));
		ui->menuDiagramas_recientes->addAction(archivosRecientes[i]);
	}
	actualizarAccionesFicherosRecientes();
	if(archivosRecientes[0]->isVisible()&& autoCargarUltimoDiagrama)
		abrirFichero(archivosRecientes[0]->data().toString());

	ac = new Actualizador(this);
	connect(ac,SIGNAL(getNewVersion(QString,QString)),this,SLOT(nuevaVersion(QString,QString)));
	ac->searchUpdate();
	newVersion = new QLabel(ui->mainToolBar);
	newVersion->setOpenExternalLinks(true);
	ui->mainToolBar->addWidget(newVersion);

	loadPlugins();
	wg->connect(wg,SIGNAL(onOpenRequest(QString)),this,SLOT(onOpenRequest(QString)));
	wg->connect(wg,SIGNAL(onNewRequest()),this, SLOT(onNewRequest()));
	insertTab(0,wg);

	loadSettings();
}

MainWindow::~MainWindow()
{
	delete ui;
	delete m_combo;
}
void MainWindow::saveSettings(){
	QSettings settings;
	settings.setValue("geometry", saveGeometry());
	settings.setValue("ftGeometry",m_ft->saveGeometry());
	settings.setValue("frGeometry",m_fr->saveGeometry());
	settings.setValue("fsrGeometry",m_fsr->saveGeometry());
	settings.setValue("inspectGeometry",m_inspect->saveGeometry());
	settings.setValue("lastDir",defaultFilePath);
	settings.setValue("barraAcciones", ui->actionBarra_de_acciones->isChecked());
}

void MainWindow::loadSettings(){
	QSettings settings;
	restoreGeometry(settings.value("geometry").toByteArray());

	m_ft->restoreGeometry(settings.value("ftGeometry").toByteArray());
	m_fr->restoreGeometry(settings.value("frGeometry").toByteArray());
	m_fsr->restoreGeometry(settings.value("fsrGeometry").toByteArray());
	m_inspect->restoreGeometry(settings.value("inspectGeometry").toByteArray());
	defaultFilePath=settings.value("lastDir").toString();

	//El formulario introductorio se mostrará una vez al día
	const QString aboutKey = "showedAbout";
	const QString date = QDate::currentDate().toString("dd/MM/yyyy");
	if(settings.value(aboutKey,"13/09/1987").toString() != date){
		FormAbout fr;
		fr.exec();
		settings.setValue(aboutKey,date);
	}

	wg->loadRecents();
	bool val =settings.value("barraAcciones",true).toBool();
	showActionBar=val;
	ui->actionBarra_de_acciones->setChecked(val);

	//Verificación del lenguage
	const QString lang = settings.value("language").toString();
	foreach(QAction * action, m_ActionLanguage->actions()){
		if(action->data().toString() == lang){
			action->setChecked(true);
			break;
		}
	}
}

void MainWindow::on_actionSalir_triggered()
{
	salir();
}

void MainWindow::salir(){
	int indiceActual =0;
	while(indiceActual<ui->tabWidget->count()){
		if(!cerrarPestana(indiceActual,true))
			indiceActual++;
	}
	saveSettings();
	qApp->exit(0);
}

void MainWindow::on_actionNuevo_triggered()
{
	agregarDiagrama(true);
}
PantallaDiagrama*
MainWindow::agregarDiagrama(bool isNewDiagram){
	PantallaDiagrama *m_model = new PantallaDiagrama(this);
	return agregarDiagrama(m_model,isNewDiagram);
}
PantallaDiagrama*
MainWindow::agregarDiagrama(PantallaDiagrama * m_model,bool isNewDiagram){
	bool accept =true;
	QGraphicsView *m_view = new QGraphicsView();
	//TODO: hacer el uso de Antialiasing opcional
	//m_view->setRenderHint(QPainter::Antialiasing);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	m_view->setScene(m_model);
	m_view->setWindowIcon(QIcon(":/icons/images/diagram-edit.png"));
	if(isNewDiagram){
		m_propfr->setPantallaDiagrama(m_model);
		if(m_propfr->exec()!=1)
			accept =false;
	}
	if(accept){
		m_view->setWindowTitle(m_model->nombreDiagrama());
		addTab(m_view);
	}
	else{
		delete m_model;
		m_model =NULL;
	}
	return m_model;
}
void
MainWindow::cortar()
{
	copiar("cortar");
	if(m_pantallaActual){
		m_pantallaActual->eliminarItemsSeleccionados();
	}
}

void
MainWindow::copiar(QString type)
{
	QList<BaseItem *> items = m_pantallaActual->itemsSeleccionados();
	if (!items.isEmpty()) {
		QDomDocument doc;
		QDomProcessingInstruction xml = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(xml);
		QDomElement element = doc.createElement("items");
		element.setAttribute("type-iteraction",type);
		foreach(BaseItem *Item, items ){
			element.appendChild(LoadSaveProject::itemToXml(Item,doc));
		}
		doc.appendChild(element);
		QByteArray data;
		QTextStream stream(&data);
		doc.save(stream, 0);
		QMimeData *mimeData = new QMimeData();
		mimeData->setData("application/KataKuntur.items", data);
		QApplication::clipboard()->setMimeData(mimeData);
	}
}

void
MainWindow::pegar()
{
	if(m_pantallaActual){
		const QMimeData *mimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);
		if (mimeData) {
			QByteArray data = mimeData->data("application/KataKuntur.items");
			if(data.isEmpty())
				return;
			QDomDocument doc;
			doc.setContent(data);

			QDomElement element = doc.firstChildElement("items");
			QDomElement items = element.firstChildElement("item");
			bool isCopiar =true;
			if(element.attribute("type-iteraction")=="cortar")
				isCopiar=false;
			while(!items.isNull()){
				BaseItem * it =LoadSaveProject::itemFromXml(items,m_pantallaActual);
				ItemMovible *item =dynamic_cast<ItemMovible *>(it);
				if (item) {
					if(isCopiar){
						//BUG: se genera una nueva ID cuando el item es pegado
						item->generarId();
						item->setPos(item->x() + 40,item->y()+40);
					}
					m_pantallaActual->agregarItem(item);
				}
				items = items.nextSiblingElement("item");
			}
			if(!isCopiar)
				QApplication::clipboard()->setMimeData(new QMimeData(),QClipboard::Clipboard);
		}
	}
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
	cerrarPestana(index);
}
PantallaDiagrama *MainWindow::obtenerPantalla(int index){
	if(index > ui->tabWidget->count())
		return NULL;
	QWidget *wg = ui->tabWidget->widget(index);
	QGraphicsView *m_v = qobject_cast<QGraphicsView *>(wg);
	if(m_v){
		return qobject_cast<PantallaDiagrama *>(m_v->scene());
	}
	return NULL;
}
QGraphicsView *MainWindow::obtenerVistaPantalla(int index){
	if(index > ui->tabWidget->count())
		return NULL;
	QWidget *wg = ui->tabWidget->widget(index);
	QGraphicsView *m_v = qobject_cast<QGraphicsView *>(wg);
	return m_v;
}
bool MainWindow::cerrarPestana(int index,bool forceClose){
	PantallaDiagrama* pant = obtenerPantalla(index);
	if(pant){
		if(pant->hasChanges()){
			QMessageBox::StandardButtons a=QMessageBox::Ok|QMessageBox::No;
			if(!forceClose)
				a = a | QMessageBox::Cancel;
			QMessageBox::StandardButton as = QMessageBox::question(this,trUtf8("¿Desea cerrar?"),trUtf8("Hay cambios que no se han guardado en el proyecto: <b>%1</b>, ¿Desea guardarlos?").arg(pant->nombreDiagrama()),a,QMessageBox::No);
			if(as==QMessageBox::Ok){
				guardar(pant);
			}
			else if(as == QMessageBox::Cancel){
				return false;
			}
		}
		QGraphicsView *vt = obtenerVistaPantalla(index);
		//Se compara si el diagrama a cerrrarse es el diagrama actual
		if(m_pantallaActual==vt->scene()){
			m_pantallaActual=NULL;
			undoView->setStack(NULL);
			ui->actionUndo->setEnabled(false);
			ui->actionRedo->setEnabled(false);
		}
		delete vt;
		return true;
	}
	else{
		QWidget *wag = ui->tabWidget->widget(index);
		if(wag==wg){
			return false;
		}
		ui->tabWidget->removeTab(index);
	}
	return false;
}

QString MainWindow::mostrarDialogo(const MainWindow::TipoDialogo tipo, const QString &filters,const QString &suffix)
{
	QString file;
	switch(tipo){
	case Guardar:
	case GuardarComo:
	{
		QFileDialog df(this,QString(),defaultFilePath,filters);
		df.setFileMode(QFileDialog::AnyFile);
		if(!suffix.isEmpty())
			df.setDefaultSuffix(suffix);
		df.setAcceptMode(QFileDialog::AcceptSave);
		df.setWindowTitle(tr("Guardar"));
		if(tipo==GuardarComo)
			df.setWindowTitle(tr("Guardar como"));
		df.setLabelText(QFileDialog::Accept,tr("Guardar"));
		if (df.exec()){
			file = df.selectedFiles().at(0);
			file = file.trimmed();
			if(file.isEmpty())
				break;
		}
		break;
	}
	case Abrir:
		file = QFileDialog::getOpenFileName(this, QString(), defaultFilePath,filters);
		break;
	}
	if (!file.isNull()){
		QFileInfo path(file);
		defaultFilePath = path.absoluteDir().path();
	}
	return file;
}

bool
MainWindow::guardar(PantallaDiagrama* pant)
{
	if(pant){
		bool isNew = pant->isNew();
		if (isNew)
			return guardarComo(pant);
		QString fileName = pant->nombreFichero();
		if(fileName.endsWith(".qcm")){
			if(QMessageBox::question(this,trUtf8("Cambio de formato"),trUtf8("No se puede guardar al formato *.qcm(QCaseModel)¿Desea convertirlo al formato de Kata Kuntur?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
				return guardarComo(pant);
			}
			return false;
		}
		guardarFichero(fileName,pant);
		return true;
	}
	return false;
}
void
MainWindow::guardarFichero(const QString &fileName, PantallaDiagrama *pantalla)
{
	//pantalla->guardar(fileName);
	if(!LoadSaveProject::guardar(fileName,pantalla)){
		QMessageBox::warning(this,"Error",LoadSaveProject::lastError());
		return;
	}
	if(m_pantallaActual!=NULL)
		setWindowTitle(title + " : " +m_pantallaActual->nombreFichero());
	else
		setWindowTitle(title);
	//pantalla->undoStack()->setClean();
	setActualFichero(fileName);
	QMessageBox::information(this,trUtf8("Guardado"),trUtf8("El proyecto <b>%1</b> ha sido guardado correctamente en el archivo: \n<b>%2</b>").arg(pantalla->nombreDiagrama()).arg(pantalla->nombreFichero()));
	//addRecentFile(fileName);
}
bool
MainWindow::guardarComo(PantallaDiagrama * pant)
{
	if(pant){
		QString fileName = mostrarDialogo(GuardarComo,trUtf8("Diagrama Kata Kuntur (%1)").arg("*.kkd"),"kkd");
		if (fileName.isEmpty())
			return false;
		guardarFichero(fileName,pant);
		return true;
	}
	return false;
}
void
MainWindow::abrir()
{
	QString fileName = mostrarDialogo(Abrir, trUtf8("Diagramas Soportados (%1);;Diagrama Kata Kuntur  (%2);;Diagrama QCaseModel (%3)").arg("*.kkd *.qcm").arg("*.kkd").arg("*.qcm"),QString());
	if (!fileName.isNull())
		abrirFichero(fileName);
}

void MainWindow::mostrarAyuda()
{
	//if(m_help==NULL)
	//	m_help = new HelpWidget(this);
	//addTab(m_help);
}

void MainWindow::abrirArchivoReciente(){
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		abrirFichero(action->data().toString());
}
void
MainWindow::abrirFichero(const QString &fileName)
{
	//Detectar si el diagrama ha sido cargado
	for(int i =0;i<ui->tabWidget->count();i++){
		PantallaDiagrama * p =obtenerPantalla(i);
		if(p==NULL)
			continue;
		if(p->nombreFichero()==fileName){
			ui->tabWidget->setCurrentIndex(i);
			return;
		}
	}
	if(QFile::exists(fileName)){
		PantallaDiagrama *model = LoadSaveProject::cargar(fileName);
		if(!model) {
			//QMessageBox::critical(this, tr("Error"), tr("Formato desconocido."));
			QMessageBox::critical(this, tr("No es posible abrir el diagrama"), LoadSaveProject::lastError());
			return;
		}
		agregarDiagrama(model);
		//ui->treeProject->setModel(model->model());
		setActualFichero(fileName);
	}
	else{
		QMessageBox::critical(this, tr("Error"), tr("El fichero no existe."));
		return;
	}
}

void MainWindow::on_actionGuardar_triggered()
{
	guardar(m_pantallaActual);
}

void MainWindow::on_actionAbrir_triggered()
{
	abrir();
}

void MainWindow::on_actionEliminar_triggered()
{
	if(m_pantallaActual)
		m_pantallaActual->eliminarItemsSeleccionados();
}

void MainWindow::on_actionCopiar_triggered()
{
	copiar("copiar");
}

void MainWindow::on_actionCortar_triggered()
{
	cortar();
}

void MainWindow::on_actionPegar_triggered()
{
	pegar();
}

void MainWindow::on_actionDuplicar_triggered()
{
	 copiar("copiar");
	 pegar();
}
void MainWindow::on_tabWidget_currentChanged(int index)
{
	if(m_pantallaActual){
		//Se desconectan todas las conexiones antes de cambiar
		//de pantalla actual
		disconnect(m_pantallaActual,0,this,0);
		m_pantallaActual->undoStack()->disconnect();
		ui->actionUndo->disconnect();
		ui->actionRedo->disconnect();
	}
	m_pantallaActual=NULL;
	ui->menuExportar_Diagrama->setEnabled(false);
	ui->menuExportar_mapeo->setEnabled(false);
	ui->actionGuardar->setEnabled(false);
	ui->actionGuardar_Como->setEnabled(false);
	ui->actionZoom_In->setEnabled(false);
	ui->actionZoom_Out->setEnabled(false);
	ui->actionPropiedades_de_diagrama->setEnabled(false);
	ui->actionExportar_a_DBMS->setEnabled(false);
	ui->actionExportar_ORM->setEnabled(false);
	ui->actionVerificar_diagrama->setEnabled(false);
	ui->actionLimpiar_historial->setEnabled(false);
	m_combo->setEnabled(false);
	m_pantallaActual = obtenerPantalla(index);
	if(m_pantallaActual){
		//TODO: colocar preparacion de cambio
		alCambiarModo(m_pantallaActual->modo());
		m_ActionDiagrama->setEnabled(true);
		connect(m_pantallaActual,SIGNAL(modoChanged(PantallaDiagrama::Modo)),this,SLOT(alCambiarModo(PantallaDiagrama::Modo)));
		connect(m_pantallaActual,SIGNAL(selectionChanged()),this,SLOT(actualizarSeleccion()));
		connect(m_pantallaActual,SIGNAL(nombreChanged(QString)),this,SLOT(onNameChanged(QString)));
		connect(m_pantallaActual,SIGNAL(doubleClick()),this,SLOT(mostrarPropiedadItem()));

		ui->menuExportar_Diagrama->setEnabled(true);
		ui->menuExportar_mapeo->setEnabled(true);
		ui->actionGuardar->setEnabled(true);
		ui->actionGuardar_Como->setEnabled(true);
		ui->actionPropiedades_de_diagrama->setEnabled(true);
		ui->actionZoom_In->setEnabled(true);
		ui->actionZoom_Out->setEnabled(true);
		ui->actionExportar_a_DBMS->setEnabled(true);
		ui->actionExportar_ORM->setEnabled(true);
		ui->actionVerificar_diagrama->setEnabled(true);
		ui->actionLimpiar_historial->setEnabled(true);
		m_combo->setEnabled(true);
		m_combo->setCurrentIndex(m_pantallaActual->vista());

		//SetUP de Undo/Redo Actions.
		undoView->setStack(m_pantallaActual->undoStack());
		QUndoStack * und = m_pantallaActual->undoStack();
		ui->dockWidgetUndo->setVisible(true&&showActionBar);
		changeUndoText(und->undoText());
		changeRedoText(und->redoText());
		connect(ui->actionUndo,SIGNAL(triggered()),und,SLOT(undo()));
		connect(ui->actionRedo,SIGNAL(triggered()),und,SLOT(redo()));
		ui->actionUndo->setEnabled(und->canUndo());
		ui->actionRedo->setEnabled(und->canRedo());
		connect(und,SIGNAL(canUndoChanged(bool)),ui->actionUndo,SLOT(setEnabled(bool)));
		connect(und,SIGNAL(canRedoChanged(bool)),ui->actionRedo,SLOT(setEnabled(bool)));
		connect(und,SIGNAL(undoTextChanged(QString)),this,SLOT(changeUndoText(QString)));
		connect(und,SIGNAL(redoTextChanged(QString)),this,SLOT(changeRedoText(QString)));
		setWindowTitle(title + " : " +m_pantallaActual->nombreFichero());
	}
	else{
		m_ActionDiagrama->setEnabled(false);
		undoView->setStack(NULL);
		ui->dockWidgetUndo->setVisible(false);
		setWindowTitle(title);
	}
	actualizarSeleccion();
}

void MainWindow::grupoActionTigerded(QAction * act){

	if(!m_pantallaActual)
		return;
	QGraphicsView *m_view =obtenerVistaPantalla(ui->tabWidget->currentIndex());
	PantallaDiagrama::Modo a = PantallaDiagrama::Modo(act->data().toInt());
	if( a== PantallaDiagrama::Ninguno){
		m_view->setDragMode(QGraphicsView::ScrollHandDrag);
	}
	else{
		if(m_pantallaActual->modo()==PantallaDiagrama::Ninguno)
			m_view->setDragMode(QGraphicsView::RubberBandDrag);
	}
	m_pantallaActual->setModo(a);
}

void MainWindow::grupoActionLanguage(QAction * action)
{
	QSettings qs;
	qs.setValue("language",action->data().toString());
	QMessageBox::information(this,"Se requiere reiniciar","Debe reiniciar para que se carge el nuevo idioma");
}

void MainWindow::alCambiarModo(PantallaDiagrama::Modo mode){
	ui->actionDesplazar->setChecked(mode == PantallaDiagrama::Ninguno);
	ui->actionSeleccionar->setChecked(mode == PantallaDiagrama::Seleccionado);
	ui->actionAgregar_tabla->setChecked(mode == PantallaDiagrama::AgregarEntidad);
	ui->actionAgregar_relacion->setChecked(mode == PantallaDiagrama::AgregarRelacion);
}

void MainWindow::mostrarPropiedadItem(){
	QList<BaseItem *> items = m_pantallaActual->itemsSeleccionados();
	if (items.size() == 1) {
		BaseItem *item = items.first();
		if(qstrcmp(item->typeName(),"modelo-tabla")==0){
			m_ft->setItemActual(item);
			m_ft->show();
		}
		else if(qstrcmp(item->typeName(),"modelo-relacion")==0){
			m_fr->setItemActual(item);
			m_fr->show();
		}
		else if(qstrcmp(item->typeName(),"modelo-selfrelacion")==0){
			m_fsr->setItemActual(item);
			m_fsr->show();
		}
	}
}

void
MainWindow::actualizarSeleccion()
{
	if (m_pantallaActual == 0){
		return;
	}

	QList<BaseItem *> items = m_pantallaActual->itemsSeleccionados();
	// Update the property editor

	if (items.size() > 0) {
		ui->actionCortar->setEnabled(true);
		ui->actionCopiar->setEnabled(true);
		ui->actionEliminar->setEnabled(true);
		ui->actionDuplicar->setEnabled(true);
	}
	else {
		ui->actionCortar->setEnabled(false);
		ui->actionCopiar->setEnabled(false);
		ui->actionEliminar->setEnabled(false);
		ui->actionDuplicar->setEnabled(false);
	}
}
void MainWindow::setActualFichero(const QString &file)
{
	QString fileName = QDir::toNativeSeparators(file);
	actualFichero = fileName;
	setWindowFilePath(actualFichero);
	QSettings settings;
	QStringList files = settings.value("ficherosRecientes").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	settings.setValue("ficherosRecientes", files);
	actualizarAccionesFicherosRecientes();
	wg->loadRecents();
}

void MainWindow::actualizarAccionesFicherosRecientes()
{
	QSettings settings;
	QStringList files = settings.value("ficherosRecientes").toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
	int id=0;
	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(id + 1).arg(files[i]);
		if(i==0)
			setWindowFilePath(text);
		archivosRecientes[id]->setText(text);
		archivosRecientes[id]->setData(files[i]);
		archivosRecientes[id]->setVisible(true);
		if(!QFile::exists(files[i]))
			archivosRecientes[id]->setEnabled(false);
		id++;
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		archivosRecientes[j]->setVisible(false);
	if(numRecentFiles>0)
		ui->actionBorrar_archivos_recientes->setEnabled(true);
	else
		ui->actionBorrar_archivos_recientes->setEnabled(false);
}

void MainWindow::on_actionBorrar_archivos_recientes_triggered()
{
	if(QMessageBox::question(this,trUtf8("¿Está seguro?"),trUtf8("¿Está seguro que desea borrar los archivos recientes?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
		return;
	for (int i = 0; i < MaxRecentFiles; ++i) {
		archivosRecientes[i]->setVisible(false);
	}
	QSettings settings;
	settings.setValue("ficherosRecientes",QVariant());
	ui->actionBorrar_archivos_recientes->setEnabled(false);
	wg->loadRecents();
}

void MainWindow::on_actionPantalla_completa_triggered()
{
	if(windowState()!=Qt::WindowFullScreen){
		setWindowState(Qt::WindowFullScreen);
	}
	else{
		setWindowState(Qt::WindowMaximized);
	}
	ui->actionPantalla_completa->setChecked(windowState()==Qt::WindowFullScreen);
}

void
MainWindow::printPreview(QPrinter *printer)
{
	m_pantallaActual->print(printer);
}

void MainWindow::on_actionAcerca_de_triggered()
{
	FormAbout fr(this);
	fr.exec();
}

void
MainWindow::on_actionFacebook_triggered(){
	QDesktopServices::openUrl(QUrl("http://www.facebook.com/KataKuntur"));
}

void MainWindow::on_actionGuardar_Como_triggered()
{
	guardarComo(m_pantallaActual);
}

void
MainWindow::printMapeo(QPrinter *printer)
{
	//m_pantallaActual->printMapeo(printer);
	QTextDocument* doc =LoadSaveProject::generarMapeo(m_pantallaActual);
	doc->print(printer);
	delete doc;
}

void MainWindow::on_actionZoom_In_triggered()
{
	int zo=ui->tabWidget->currentIndex();
	QGraphicsView *vt = obtenerVistaPantalla(zo);
	vt->scale(1.1,1.1);
}

void MainWindow::on_actionZoom_Out_triggered()
{
	int zo=ui->tabWidget->currentIndex();
	QGraphicsView *vt = obtenerVistaPantalla(zo);
	vt->scale(0.9,0.9);
}

void MainWindow::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui->retranslateUi(this);
	} else
		QMainWindow::changeEvent(event);
}

void MainWindow::on_actionPropiedades_de_diagrama_triggered()
{
	m_propfr->setPantallaDiagrama(m_pantallaActual);
	m_propfr->show();
}

void MainWindow::on_actionExportar_a_DBMS_triggered()
{
	if(m_pantallaActual->canExportarSql()){
		m_export->setPantallaDiagrama(m_pantallaActual);
		addTab(m_export);
	}
	else
		QMessageBox::information(this,tr("Error al exportar"),tr("No se puede exportar a *.sql, debe seleccionar un DBMS primero en las propiedades del diagrama."));
}

void MainWindow::on_actionExportar_ORM_triggered()
{
	//TODO: uncoment
	/*if(!m_pantallaActual->canExportORM()){
		QMessageBox::warning(this,tr("Exportar ORM."),trUtf8("No se puede exportar a un Modelo ORM , ya que no se ha seleccionado ninguno en las propiedades del diagrama."));
		return;
	}*/

	//TODO: uncoment
	//if(m_pantallaActual->exportarORM(qApp->applicationDirPath()))
	//	QMessageBox::information(this,tr("Finalizado"),tr("Exportado correctamente."));
}

void MainWindow::on_printDiagram_triggered()
{
	printer = new QPrinter();
	QPrintPreviewDialog printPreviewDialog(printer, this);
	connect(&printPreviewDialog, SIGNAL(paintRequested(QPrinter*)),
								 SLOT(printPreview(QPrinter*)));
	printPreviewDialog.exec();
}

void MainWindow::on_actionImprimirMapeo_triggered()
{
//	if(!m_pantallaActual->canPrintMapeo()){
//		QMessageBox::warning(this,tr("Error al generar mapeo"),trUtf8("Asegúrese de que todas las relaciones no tengan cadenas vacías en el nombre de la relación."));
//		return;
//	}
	printer = new QPrinter();
	QPrintPreviewDialog printPreviewDialog(printer, this);
	connect(&printPreviewDialog, SIGNAL(paintRequested(QPrinter*)),
								 SLOT(printMapeo(QPrinter*)));
	printPreviewDialog.exec();
}

void MainWindow::on_actionExportarSQL_triggered()
{
	QString fileName = mostrarDialogo(Guardar, tr("Lenguaje de Consulta Estructurado (%1)").arg("*.sql"),"sql");
	if (fileName.isNull())
		return;
	if(m_pantallaActual->canExportarSql())
		m_pantallaActual->exportarSQL(fileName);
	else
		QMessageBox::information(this,tr("Error al exportar"),tr("No se puede exportar a *.sql, debe seleccionar un DBMS primero en las propiedades del diagrama."));
}

void MainWindow::on_actionExportarPNG_triggered()
{
	QString fileName = mostrarDialogo(Guardar,tr("Imagen PNG(%1)").arg("*.png"),"png");
	if (fileName.isNull())
		return;
	if(!LoadSaveProject::exportaraPNG(fileName,m_pantallaActual))
		QMessageBox::warning(this,"error",LoadSaveProject::lastError());
}

void MainWindow::on_action_yml_triggered()
{
	QString fileName = mostrarDialogo(Guardar, tr("YAML(%1)").arg("*.yml"),"yml");
	if (fileName.isNull())
		return;
	if(!LoadSaveProject::exportarYAML(fileName,m_pantallaActual))
		QMessageBox::warning(this,"error",LoadSaveProject::lastError());
}

void MainWindow::closeEvent(QCloseEvent *){
	salir();
}

void MainWindow::on_actionVerificar_diagrama_triggered()
{
	if(!m_pantallaActual->canExportarSql()){
		QMessageBox::information(this,tr("Verificar diagrama."),trUtf8("No se puede revisar el diagrama por que no hay un gestor DBMS selecionado."));
		return;
	}
	m_inspect->setDiagrama(m_pantallaActual);
	m_inspect->show();
}

void MainWindow::on_action_ppng_triggered()
{
//	if(!m_pantallaActual->canPrintMapeo()){
//		QMessageBox::warning(this,tr("Error al generar mapeo"),trUtf8("Asegúrese de que todas las relaciones no tengan cadenas vacías en el nombre de la relación."));
//		return;
//	}
	QString fileName = mostrarDialogo(Guardar, tr("Imagen PNG(%1)").arg("*.png"),"png");
	if (fileName.isNull())
		return;
	LoadSaveProject::exportarMapeoPNG(fileName,m_pantallaActual);
}
void MainWindow::on_actionODT_triggered()
{
	QString fileName = mostrarDialogo(Guardar,tr("Open Document Text(%1)").arg("*.odt"),"odt");
	if (fileName.isNull())
		return;
	if(!LoadSaveProject::exportarMapeoODT(fileName,m_pantallaActual))
		QMessageBox::warning(this,"Error",LoadSaveProject::lastError());
}

void MainWindow::comboChanged(int id){
	Q_ASSERT(m_pantallaActual);
	if(id==0)
		m_pantallaActual->setVista(PantallaDiagrama::Logica);
	else
		m_pantallaActual->setVista(PantallaDiagrama::Fisica);
}

void MainWindow::on_actionPreferencias_triggered()
{
	FormPropiedades fr(this);
	fr.exec();
}

void MainWindow::on_actionP_gina_oficial_triggered()
{
	QDesktopServices::openUrl(QUrl("http://katakuntur.jeanmazuelos.com/"));
}

void MainWindow::changeUndoText(QString value){
	ui->actionUndo->setText(trUtf8("Deshacer : %1").arg(value));
}

void MainWindow::changeRedoText(QString value){
	ui->actionRedo->setText(trUtf8("Rehacer : %1").arg(value));
}

void
MainWindow::nuevaVersion(const QString string, const QString description){
	newVersion->setText(trUtf8("<a href=\"http://katakuntur.jeanmazuelos.com/downloads\">Nueva versión disponible : <b>%1</b></a> %2").arg(string,description));
	QMessageBox::information(this,trUtf8("Nueva Versión"),trUtf8("Hay una nueva versión disponible : <b>%1</b> <br /> Puedes descargarlo en: <a href=\"http://katakuntur.jeanmazuelos.com/downloads\">http://katakuntur.jeanmazuelos.com/downloads</a>.").arg(string));
}

void
MainWindow::onNameChanged(const QString name){
	int a =ui->tabWidget->currentIndex();
	ui->tabWidget->setTabText(a,name);
}

void MainWindow::on_actionManual_de_ayuda_triggered()
{
	mostrarAyuda();
}

void MainWindow::on_actionReportar_errores_triggered()
{
	QDesktopServices::openUrl(QUrl("http://pm.jeanmazuelos.com/katakuntur/issues/new"));

}

void MainWindow::addTab(QWidget *widget )
{

	int a=ui->tabWidget->addTab(widget,widget->windowIcon(), widget->windowTitle());
	if(a>=0)
		ui->tabWidget->setCurrentIndex(a);
}

void MainWindow::insertTab(int index, QWidget *widget)
{
	ui->tabWidget->insertTab(index,widget,widget->windowIcon(), widget->windowTitle());
}

void MainWindow::loadPlugins()
{
	QList<IPlugin *> plugins = PluginManager::instance()->loadedPlugins();
	foreach(IPlugin *plugin, plugins){
		IPluginWidget *pluginw = qobject_cast<IPluginWidget*>(plugin);
		if(!pluginw)
			continue;
		QAction * act = pluginw->action();
		act->setData(QVariant::fromValue(pluginw));
		ui->menuHerramientas->addAction(act);
		connect(act,SIGNAL(triggered()),this,SLOT(onacttionClic()));
	}
}

void MainWindow::onacttionClic()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QVariant obj =action->data();
	IPluginWidget * plug = action->data().value<IPluginWidget *>();
	if(plug){
		QWidget * w = plug->widget();
		addTab(w);
	}
}

void MainWindow::onOpenRequest(const QString file)
{
	if(file =="com:open")
		abrir();
	else
		abrirFichero(file);
}

void MainWindow::onNewRequest()
{
	agregarDiagrama(true);
}

void MainWindow::on_actionDonar_triggered()
{
	QDesktopServices::openUrl(QUrl("http://katakuntur.jeanmazuelos.com/donate"));
}

void MainWindow::on_actionBarra_de_acciones_toggled(bool arg1)
{
	showActionBar= arg1;
	ui->dockWidgetUndo->setVisible(true&arg1&(bool)m_pantallaActual);
}

void MainWindow::on_actionLimpiar_historial_triggered()
{
	m_pantallaActual->undoStack()->clear();
}

void MainWindow::on_actionDiccionario_de_dato_triggered()
{
	QFileDialog df(this,QString(),defaultFilePath,tr("Open Document Text(%1);;HiperText Makup Language(%2)").arg("*.odt").arg("*.html"));
	df.setFileMode(QFileDialog::AnyFile);
	df.setAcceptMode(QFileDialog::AcceptSave);
	df.setWindowTitle(tr("Guardar"));
	df.setLabelText(QFileDialog::Accept,tr("Guardar"));
	if (df.exec()){
		QString file = df.selectedFiles().at(0);
		file = file.trimmed();
		if(file.isEmpty())
			return;
		const QString form = df.selectedNameFilter();
		QString format;
		if(form.contains("*.html"))
			format = "html";
		else if(form.contains("*.odt"))
			format = "odt";
		if(!LoadSaveProject::exportarDiccinarioDatosODT(file,m_pantallaActual,format))
			QMessageBox::warning(this,"Error",LoadSaveProject::lastError());
		else
			QMessageBox::information(this,QString(),trUtf8("Se ha guardado correctamente"));
	}


}
