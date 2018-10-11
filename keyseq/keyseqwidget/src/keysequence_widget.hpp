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

#ifndef KEYSEQUENCE_WIDGET_HPP
#define KEYSEQUENCE_WIDGET_HPP

#include <QWidget>
#include <QKeySequence>



class KeySequence_Widget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QKeySequence keySequence READ keySequence WRITE setKeySequence NOTIFY keySequenceChanged USER true)
    Q_PROPERTY(QAction* attachedAction READ attachedAction WRITE setAttachedAction NOTIFY attachedActionChanged)
    Q_PROPERTY(QKeySequence defaultSequence READ defaultSequence WRITE setDefaultSequence)

    friend class KeySequence_Widget_Private;
    class KeySequence_Widget_Private* d;

public:
    explicit KeySequence_Widget(QWidget *parent = 0);
    explicit KeySequence_Widget(const QKeySequence& seq, QWidget *parent = 0);
    explicit KeySequence_Widget(QAction* action, QWidget *parent = 0);
    ~KeySequence_Widget();

    QKeySequence keySequence() const;
    QKeySequence defaultSequence() const;
    QAction* attachedAction() const;


public slots:
    void setKeySequence ( const QKeySequence& seq );
    void setDefaultSequence ( const QKeySequence& seq );
    void clear();
    void resetToDefault();

    void setAttachedAction(QAction* action);

private slots:
    void action_changing();

protected:
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    bool event(QEvent *event);
    void changeEvent(QEvent *event);

signals:
    void keySequenceChanged(QKeySequence);
    void attachedActionChanged(QAction*);

};

#endif // KEYSEQUENCE_WIDGET_HPP
