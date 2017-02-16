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

#ifndef FORMRELACION_H
#define FORMRELACION_H

#include <QWidget>
#include <QLabel>
#include "propiedades.h"
#include "model/relacion.h"

namespace Ui {
	class FormRelacion;
}

class FormRelacion : public Propiedades {
	Q_OBJECT
public:
	FormRelacion(QWidget *parent = 0);
	~FormRelacion();
	Relacion* actualRelacion();

protected:
	void changeEvent(QEvent *e);
	void cambiarItemActual(BaseItem *oldItem, BaseItem *newItem);

private:
	void actualizarCombos();
	Ui::FormRelacion *ui;
	Relacion * m_relacionActual;
	Relacion::Cardinalidad obtenerCardinalidad(int);
	Relacion::CardinalidadLinea obtenerCardinalidadLineal(int string);

	void actualizarCardinalidadLinealInicio();
	void actualizarCardinalidadLinealFinal();

	void actualizarMultiplicidadInicio();
	void actualizarMultiplicidadFinal();

	void actualizarLabelInicio();
	void actualizarLabelFinal();
	int obtenerCardinalidadLinealInt(Relacion::CardinalidadLinea cardinalidad);
	int obtenerCardinalidadInt(Relacion::Cardinalidad cardinalidad);
private slots:
	void on_leLabelFinal_textEdited(QString );
	void on_cbCardinalidadFinal_activated(int index);
	void on_cbCardinalidadInicio_activated(int index);
	void on_cbMultiplicidadFinal_activated(int index);
	void on_cbMultiplicidadInicio_activated(int index);
	void on_leLabelInicio_textEdited(QString );
};

#endif // FORMRELACION_H
