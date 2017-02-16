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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "vistadiagrama.h"
#include "pantalladiagrama.h"
#include <QActionGroup>
#include <QStackedWidget>
#include <QPrinter>
#include "widgets/formrelacion.h"
#include "widgets/formtabla.h"
#include "widgets/formselfrelacion.h"
#include "formpantalladiagrama.h"
#include "formexportdbms.h"
#include <QTranslator>
#include <QComboBox>
#include <QDockWidget>
#include <QUndoView>
#include "actualizador.h"
#include "inspectdiagramwidget.h"

namespace Ui
{
	class MainWindow;
}
class WgBienvenida;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	enum View{DataBase=0,ORM=1};
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void actualizarAccionesFicherosRecientes();
	void setActualFichero(const QString &fileName);
	void abrirFichero(const QString &fileName);
	static bool autoCargarUltimoDiagrama;
	static QString title;
protected:
	void changeEvent(QEvent *event);
	void closeEvent ( QCloseEvent * event ) ;
	void setView(View view);
private:
	bool showActionBar;
	Ui::MainWindow *ui;
	PantallaDiagrama *m_pantallaActual;
	void salir();
	PantallaDiagrama * agregarDiagrama(bool isNewDiagram =false);
	PantallaDiagrama * agregarDiagrama(PantallaDiagrama *,bool isNewDiagram =false);
	bool guardar(PantallaDiagrama *);
	bool guardarComo(PantallaDiagrama *);
	void copiar(QString type);
	void cortar();
	void pegar();
	void guardarFichero(const QString &fileName, PantallaDiagrama *pantalla);
	void abrir();
	void mostrarAyuda();
	QActionGroup* m_ActionDiagrama;
	QActionGroup* m_ActionLanguage;
	enum { MaxRecentFiles = 10 };
	QAction *archivosRecientes[MaxRecentFiles];
	QString actualFichero;
	QPrinter *printer;
	PantallaDiagrama* obtenerPantalla(int index);
	QGraphicsView *obtenerVistaPantalla(int index);
	bool cerrarPestana(int index,bool forceClose=false);
	FormTabla * m_ft;
	FormRelacion *m_fr;
	FormSelfRelacion *m_fsr;
	FormPantallaDiagrama* m_propfr;
	FormExportDBMS *m_export;
	InspectDiagramWidget *m_inspect;
	QComboBox *m_combo;
	QUndoView * undoView;
	QDialog *dlg;
	Actualizador * ac;
	QLabel * newVersion;
	QString defaultFilePath;
	WgBienvenida *wg;
	enum TipoDialogo{Guardar, Abrir,GuardarComo};

	QString mostrarDialogo(TipoDialogo tipo,  const QString &filters,const QString &suffix);
	void addTab(QWidget *widget);
	void insertTab(int index, QWidget * widget);
	void saveSettings();
	void loadSettings();
private slots:
	void onOpenRequest(const QString file);
	void onNewRequest();
	void on_actionP_gina_oficial_triggered();
	void on_actionExportar_ORM_triggered();
	void on_actionODT_triggered();
	void on_actionPreferencias_triggered();
	void comboChanged(int );
	void on_action_ppng_triggered();
	void on_actionVerificar_diagrama_triggered();
	void on_action_yml_triggered();
	void on_actionExportarPNG_triggered();
	void on_actionExportarSQL_triggered();
	void on_actionImprimirMapeo_triggered();
	void on_printDiagram_triggered();
	void on_actionExportar_a_DBMS_triggered();
	void on_actionPropiedades_de_diagrama_triggered();
	void on_actionZoom_Out_triggered();
	void on_actionZoom_In_triggered();
	void printMapeo(QPrinter *printer);
	void on_actionGuardar_Como_triggered();
	void on_actionAcerca_de_triggered();
	void printPreview(QPrinter *printer);
	void on_actionPantalla_completa_triggered();
	void abrirArchivoReciente();
	void on_tabWidget_currentChanged(int );
	void on_actionPegar_triggered();
	void on_actionCortar_triggered();
	void on_actionCopiar_triggered();
	void on_actionEliminar_triggered();
	void on_actionAbrir_triggered();
	void on_actionGuardar_triggered();
	void on_tabWidget_tabCloseRequested(int index);
	void on_actionNuevo_triggered();
	void on_actionSalir_triggered();
	void grupoActionTigerded(QAction *);
	void grupoActionLanguage(QAction *);
	void alCambiarModo(PantallaDiagrama::Modo);
	void actualizarSeleccion();
	void mostrarPropiedadItem();
	void on_actionDuplicar_triggered();
	void changeUndoText(QString);
	void changeRedoText(QString);
	void nuevaVersion(const QString, const QString);
	void on_actionFacebook_triggered();
	void onNameChanged(const QString);
	void on_actionManual_de_ayuda_triggered();
	void on_actionReportar_errores_triggered();
	void loadPlugins();
	void onacttionClic();
	void on_actionBorrar_archivos_recientes_triggered();
	void on_actionDonar_triggered();
	void on_actionBarra_de_acciones_toggled(bool arg1);
	void on_actionLimpiar_historial_triggered();
	void on_actionDiccionario_de_dato_triggered();
};

#endif // MAINWINDOW_H
