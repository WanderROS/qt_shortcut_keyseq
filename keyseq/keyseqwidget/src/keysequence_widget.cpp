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

#include "keysequence_widget.hpp"

#include <QPushButton>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QAction>

#if __cplusplus < 201103 && !defined(nullptr)
    #define nullptr 0
#endif // C++11


struct KeySequence_Widget_Private
{
    KeySequence_Widget *p;
    QKeySequence sequence;
    QKeySequence previous_sequence;
    QKeySequence default_sequence;
    bool is_grabbing;
    QPushButton *button;
    QAction* action;

    KeySequence_Widget_Private(KeySequence_Widget* p,
                               const QKeySequence &seq,
                               QAction* action=nullptr)
        : p(p), sequence(seq), previous_sequence(seq), default_sequence(seq),
          is_grabbing(false), button (new QPushButton(p)), action(nullptr)
    {
        if ( action )
            set_action(action);
        update_label();

        QVBoxLayout *layout = new QVBoxLayout(p);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(button);
        button->setSizePolicy(p->sizePolicy());


        button->setFocusProxy(p);
        button->installEventFilter(p);

        //p->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        p->setFocusPolicy(Qt::StrongFocus);
        p->setAttribute(Qt::WA_MacShowFocusRect, true);
        p->setAttribute(Qt::WA_InputMethodEnabled, false);


    }

    void set_action(QAction* act)
    {
        if ( action )
            QObject::disconnect(action,SIGNAL(changed()),p,SLOT(action_changing()));
        action = act;
        sequence = act->shortcut();
        update_label();
        QObject::connect(action,SIGNAL(changed()),p,SLOT(action_changing()));
    }

    void update_label()
    {
        if ( sequence.isEmpty() )
            button->setText(KeySequence_Widget::tr("(empty)"));
        else
            button->setText(sequence.toString(QKeySequence::NativeText));
    }

    void update_sequence()
    {
        if ( action )
            action->setShortcut(sequence);
        update_label();
        emit p->keySequenceChanged(sequence);
    }

    void begin_grabbing()
    {
        previous_sequence = sequence;
        button->setCheckable(true);
        button->setChecked(true);
        is_grabbing = true;
        p->grabKeyboard();
        update_label();
    }

    void end_grabbing()
    {
        button->setCheckable(false);
        button->setChecked(false);
        p->releaseKeyboard();
        is_grabbing = false;
        update_sequence();
        p->update();
    }

};

KeySequence_Widget::KeySequence_Widget(QWidget *parent) :
    QWidget(parent), d(new KeySequence_Widget_Private(this,QKeySequence()))
{
}


KeySequence_Widget::KeySequence_Widget(const QKeySequence &seq, QWidget *parent) :
    QWidget(parent), d(new KeySequence_Widget_Private(this,seq))
{
}

KeySequence_Widget::KeySequence_Widget(QAction *action, QWidget *parent) :
    QWidget(parent), d(new KeySequence_Widget_Private(this,action->shortcut(),action))
{

}

KeySequence_Widget::~KeySequence_Widget()
{
    delete d;
}

QKeySequence KeySequence_Widget::keySequence() const
{
    return d->sequence;
}

QKeySequence KeySequence_Widget::defaultSequence() const
{
    return d->default_sequence;
}

QAction *KeySequence_Widget::attachedAction() const
{
    return d->action;
}

void KeySequence_Widget::setKeySequence(const QKeySequence &seq)
{
    d->sequence = seq;
    d->update_sequence();
}

void KeySequence_Widget::setDefaultSequence(const QKeySequence &seq)
{
    d->default_sequence = seq;
}

void KeySequence_Widget::clear()
{
    setKeySequence(QKeySequence());
}

void KeySequence_Widget::resetToDefault()
{
    setKeySequence(d->default_sequence);
}

void KeySequence_Widget::setAttachedAction(QAction *action)
{
    d->set_action(action);
    emit attachedActionChanged(action);
}

void KeySequence_Widget::action_changing()
{
    setKeySequence(d->action->shortcut());
}

void KeySequence_Widget::mousePressEvent(QMouseEvent *)
{
    if ( d->is_grabbing )
    {
        d->sequence = d->previous_sequence;
        d->end_grabbing();
    }
    else
        d->begin_grabbing();
}

void KeySequence_Widget::keyPressEvent(QKeyEvent *event)
{
    if ( !d->is_grabbing )
    {
        event->ignore();
        return;
    }

    event->accept();

    Qt::KeyboardModifiers modifiers = event->modifiers();


    int key = event->key();
    bool end_capture = true;

    if ( modifiers & Qt::GroupSwitchModifier )
    {
        modifiers &= ~ Qt::GroupSwitchModifier;
        if ( key != Qt::Key_Shift )
            modifiers &= ~ Qt::ShiftModifier;
    }


    switch (key)
    {
        case Qt::Key_Control:
        case Qt::Key_Alt:
        case Qt::Key_Meta:
        case Qt::Key_Shift:
            d->sequence = QKeySequence(modifiers);
            end_capture = false;
            break;
        case Qt::Key_unknown:
            return;
        case Qt::Key_AltGr:
            end_capture = false;
            break;
        default:
            d->sequence = QKeySequence(modifiers|key);
            break;

    }

    d->update_label();
    if ( end_capture )
    {
        d->end_grabbing();
    }
}

bool KeySequence_Widget::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::KeyPress:
            keyPressEvent(static_cast<QKeyEvent *>(event));
            return true;
        case QEvent::MouseButtonPress:
            mousePressEvent(static_cast<QMouseEvent *>(event));
            return true;
        case QEvent::MouseButtonRelease:
            mouseReleaseEvent(static_cast<QMouseEvent *>(event));
            return true;
        default:
            return QObject::eventFilter(obj, event);
    }
}

bool KeySequence_Widget::event(QEvent *event)
{
    if ( event->type() == QEvent::KeyPress && d->is_grabbing )
    {
         QKeyEvent *kevent = static_cast<QKeyEvent *>(event);
         if (kevent->key() == Qt::Key_Tab)
         {
             keyPressEvent(kevent);
             return true;
         }
    }

    return QWidget::event(event);
}

void KeySequence_Widget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
        case QEvent::LanguageChange:
            d->update_label();
            break;



        default:
            break;
    }
}
