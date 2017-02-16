#include "itemdelegate.h"

ItemDelegate(QObject* parent = 0) : QStyledItemDelegate(parent)
{
}

void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_ASSERT(index.isValid());

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	if (opt.state & QStyle::State_MouseOver)
	opt.icon = opt.icon.pixmap(opt.decorationSize, QIcon::Active); // <--

	QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, 0);
}
