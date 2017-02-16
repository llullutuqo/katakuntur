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

#ifndef FORMTABLA_H
#define FORMTABLA_H

#include <QWidget>
#include "propiedades.h"
#include "model/entity.h"
#include "models/columnlistmodel.h"
#include "delegates/comboboxdelegate.h"
namespace Ui {
	class FormTabla;
}

class FormTabla : public Propiedades {
	Q_OBJECT
public:
	FormTabla(QWidget *parent = 0);
	~FormTabla();

protected:
	void changeEvent(QEvent *e);
	void cambiarItemActual(BaseItem *oldItem, BaseItem *newItem);

private:
	Ui::FormTabla *ui;
	Entity* m_tablaActual;
	void actualizarNombre();
	void actualizarDescripcion();
	ComboBoxDelegate *delegate;
private slots:
	void on_pteDescripcion_textChanged();
	void on_pbMoverAbajo_clicked();
	void on_pbMoverArriba_clicked();
	void on_pbEliminar_clicked();
	void on_leTabla_textEdited(QString );
	void on_pbAgregar_clicked();
};

#endif // FORMTABLA_H
