/**

\file

\author Mattia Basaglia

\section License

Copyright (C) 2014  Mattia Basaglia

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "keysequence_listwidget.hpp"
#include "keysequence_widget.hpp"

#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <QHeaderView>
#include <QMainWindow>
#include <QDebug>
KeySequence_ListWidget::KeySequence_ListWidget(QWidget *parent) :
    QTableWidget(parent), show_reset(false)
{
    horizontalHeader()->hide();//水平头不显示
    verticalHeader()->hide();
    //setShowGrid(false);
    setSelectionMode(NoSelection);

    insertColumn(0);
    insertColumn(1);
    insertColumn(2);
  //  insertColumn(3);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void KeySequence_ListWidget::append_row(QAction* action)
{
    int row = rowCount();
    insertRow(row);

    QTableWidgetItem* item = new QTableWidgetItem(action->icon(),action->iconText());
    item->setData(Qt::UserRole,action->objectName());
    setItem(row,0,item);

    KeySequence_Widget* ksw = new KeySequence_Widget(action);
    //ksw->setDefaultSequence(action->shortcut());
    setCellWidget(row,1,ksw);

    QToolButton *clear = new QToolButton();
    clear->setIcon(QIcon::fromTheme("edit-clear"));
    clear->setText(tr("Clear"));
    clear->setToolTip(tr("Clear shortcut for \"%1\"").arg(action->iconText()));
    connect(clear,SIGNAL(clicked()),ksw,SLOT(clear()));
    setCellWidget(row,2,clear);
/*
    QToolButton *reset = new QToolButton();
    reset->setIcon(QIcon::fromTheme("edit-undo"));
    reset->setText(tr("Reset"));
    reset->setToolTip(tr("Reset shortcut for \"%1\" to the default %2")
                      .arg(action->iconText())
                      .arg(ksw->defaultSequence().toString(QKeySequence::NativeText)));
    connect(reset,SIGNAL(clicked()),ksw,SLOT(resetToDefault()));
    setCellWidget(row,3,reset);
    reset->setVisible(false);*/

}

void KeySequence_ListWidget::append_title_row(QString title)
{
    int row = rowCount();
    insertRow(row);
    QTableWidgetItem *item = new QTableWidgetItem(title);
    QFont f = item->font();
    f.setBold(true);
    item->setFont(f);
  //  item->setFlags(item->flags()&(~Qt::ItemIsEditable));
  // item->setFlags(item->flags()&(~Qt::ItemIsSelectable));
    setItem(row,0,item);
    setSpan(row,0,1,columnCount());
}

void KeySequence_ListWidget::append_rows(QList<QAction *> actions)
{
    foreach(QAction* act, actions)
        if ( !act->isSeparator() )
            append_row(act);
}

void KeySequence_ListWidget::append_menu(QMenu *menu)
{
    if ( !menu->actions().empty() )
    {
        append_title_row(menu->title().replace('&',""));
        append_rows(menu->actions());
    }
}

void KeySequence_ListWidget::append_window(QMainWindow *window)
{
    foreach(QMenu* menu, window->findChildren<QMenu*>())
        append_menu(menu);
    resizeColumnsToContents();
}

void KeySequence_ListWidget::set_default(QString action_name, QKeySequence default_sequence)
{
    for ( int i = 0; i < rowCount(); i++ )
    {
        QTableWidgetItem* it = item(i,0);
        if ( it && it->data(Qt::UserRole).toString() == action_name )
        {
            KeySequence_Widget* ksw = qobject_cast<KeySequence_Widget*>(cellWidget(i,1));
            QWidget* w = cellWidget(i,3);
            if ( ksw && w )
            {
                ksw->setDefaultSequence(default_sequence);
            }
        }
    }
}

bool KeySequence_ListWidget::show_reset_buttons() const
{
    return show_reset;
}


void KeySequence_ListWidget::set_show_reset_buttons(bool show)
{

        show_reset = show;
        for ( int i = 0; i < rowCount(); i++ )
        {

            KeySequence_Widget* ksw = qobject_cast<KeySequence_Widget*>(cellWidget(i,1));
            QWidget* w = cellWidget(i,0);
            if ( ksw && w )
            {
                w->setVisible(show);
                qDebug()<<i;

            }
        }

}
