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

#ifndef KEYSEQUENCE_LISTWIDGET_HPP
#define KEYSEQUENCE_LISTWIDGET_HPP

#include <QTableWidget>

class KeySequence_ListWidget : public QTableWidget
{
    Q_OBJECT
    Q_PROPERTY(bool show_reset_buttons READ show_reset_buttons WRITE set_show_reset_buttons )

    bool show_reset;

public:
    explicit KeySequence_ListWidget(QWidget *parent = 0);

    void append_row(QAction* action);
    void append_title_row(QString title);

    void append_rows(QList<QAction*> actions);
    void append_menu(QMenu* menu);
    void append_window(class QMainWindow* window);

    void set_default(QString action_name, QKeySequence default_sequence);

    bool show_reset_buttons() const;
    void set_show_reset_buttons(bool show);

};

#endif // KEYSEQUENCE_LISTWIDGET_HPP
