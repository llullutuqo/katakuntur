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
#ifndef WGBIENVENIDA_H
#define WGBIENVENIDA_H

#include <QWidget>
#include <QListWidgetItem>
#include <QNetworkAccessManager>

namespace Ui {
	class WgBienvenida;
}

class WgBienvenida : public QWidget
{
	Q_OBJECT

public:
	explicit WgBienvenida(QWidget *parent = 0);
	void loadRecents();
	~WgBienvenida();
signals:
	void onOpenRequest(const QString file);
	void onNewRequest();
	void urlOpened();
private slots:
	void on_lstRecientes_itemDoubleClicked(QListWidgetItem *item);
	void on_lstAcciones_itemClicked(QListWidgetItem *item);
	void onUrlOpened();
	void on_lstNews_itemPressed(QListWidgetItem *item);
	void finalizado(QNetworkReply * reply);

protected:
	void changeEvent(QEvent *event);
private:
	Ui::WgBienvenida *ui;
	enum Role{Role=Qt::UserRole+ 10};

	void openURL(const QUrl &url);
	void loadNews();
	QNetworkAccessManager *qnam;
};

#endif // WGBIENVENIDA_H
