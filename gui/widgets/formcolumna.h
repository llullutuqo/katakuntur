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

#ifndef FORMCOLUMNA_H
#define FORMCOLUMNA_H

#include <QWidget>
#include "model/entityattibute.h"
#include "qdialog.h"
#include <QUndoStack>
#include "formpantalladiagrama.h"

namespace Ui {
	class FormColumna;
}

class QUndoStack;
class FormColumna : public QWidget {
	Q_OBJECT
public:
	FormColumna(QWidget *parent = 0);
	void setItemActual(EntityAttibute *columna);
	void setStack(QUndoStack *stack);
	void setTipoDatos(QStringList lista);
	~FormColumna();

protected:
	void changeEvent(QEvent *e);
private:
	Ui::FormColumna *ui;
	EntityAttibute * m_col;
	QUndoStack *m_stack;
	FormPantallaDiagrama * m_pant;
private slots:
	void on_cbTipoDato_editTextChanged(QString );
	void on_ptNotas_textChanged();
	void on_chbNulo_clicked(bool checked);
	void on_cb_llaveUnica_currentTextChanged(const QString &arg1);
	void on_chbLlaveprimaria_clicked(bool checked);
	void on_lneNombre_textChanged(QString );
};

#endif // FORMCOLUMNA_H
