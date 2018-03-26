/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Liquid
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QKeySequence>
#include <QString>
#include <QIcon>

#include "widget.h"
#include "actionfuc.h"
#include "qglobalshortcut.h"
#include "windows.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // initial label and setting dialog
    dLabel *label = new dLabel();
    Widget *settingDialog = new Widget();

    label->setText("DesktopSub");
    app.setWindowIcon(QIcon("icon.ico"));
    label->setWindowIcon(QIcon("icon.ico"));
    settingDialog->setWindowIcon(QIcon("icon.ico"));

    // set top
    QTimer *refresh = new QTimer();
    refresh->start(1000);
    QObject::connect(refresh, &QTimer::timeout, [=]
    {
#ifdef Q_OS_WIN32
        SetWindowPos((HWND)label->winId(), HWND_TOPMOST, label->pos().x(), label->pos().y(), label->width(), label->height(), SWP_SHOWWINDOW);
#endif
    });

    // hot key "ctrl + o" to open sub file
    QGlobalShortcut *openFileKey = new QGlobalShortcut;
    openFileKey->setShortcut(QKeySequence::Open);
    QObject::connect(openFileKey, &QGlobalShortcut::activated, label, &dLabel::openSubFile);

    // hot key "ctrl + m" to show last sub
    QGlobalShortcut *lastSubKey = new QGlobalShortcut;
    lastSubKey->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    QObject::connect(lastSubKey, &QGlobalShortcut::activated, label, &dLabel::lastSub);

    // hot key "ctrl + n" to show next sub
    QGlobalShortcut *nextSubKey = new QGlobalShortcut;
    nextSubKey->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    QObject::connect(nextSubKey, &QGlobalShortcut::activated, label, &dLabel::nextSub);

    // hot key "ctrl + Up" to move Up
    QGlobalShortcut *moveUpKey = new QGlobalShortcut;
    moveUpKey->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    QObject::connect(moveUpKey, &QGlobalShortcut::activated, label, &dLabel::moveUp);

    // hot key "ctrl + Down" to move Up
    QGlobalShortcut *moveDownKey = new QGlobalShortcut;
    moveDownKey->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    QObject::connect(moveDownKey, &QGlobalShortcut::activated, label, &dLabel::moveDown);

    // hot key "ctrl + Left" to move Up
    QGlobalShortcut *moveLeftKey = new QGlobalShortcut;
    moveLeftKey->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    QObject::connect(moveLeftKey, &QGlobalShortcut::activated, label, &dLabel::moveLeft);

    // hot key "ctrl + Right" to move Up
    QGlobalShortcut *moveRightKey = new QGlobalShortcut;
    moveRightKey->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    QObject::connect(moveRightKey, &QGlobalShortcut::activated, label, &dLabel::moveRight);

    // hot key "F1" to show setting dialog
    QGlobalShortcut *helpKey = new QGlobalShortcut;
    helpKey->setShortcut(QKeySequence::HelpContents);
    QObject::connect(helpKey, &QGlobalShortcut::activated, settingDialog, &Widget::show);
    // stop refresh for give control to setting dialog
    QObject::connect(helpKey, &QGlobalShortcut::activated, refresh, &QTimer::stop);
    // if close setting dialog will start refresh
    QObject::connect(settingDialog, &Widget::closeSignal, [=]
    {
                         refresh->start(1000);
    });

    // hot key "ctrl + q" to quit
    QGlobalShortcut *quitKey = new QGlobalShortcut;
    quitKey->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    QObject::connect(quitKey, &QGlobalShortcut::activated, label, &dLabel::close);
    QObject::connect(quitKey, &QGlobalShortcut::activated, settingDialog, &dLabel::close);

    // when click save buttun in setting dialog, will initial label one times
    QObject::connect(settingDialog, &Widget::saveSignal, label, &dLabel::initial);

    // set transparent
    label->setAttribute(Qt::WA_TranslucentBackground, true);

    // close topbar
    label->setWindowFlags(Qt::FramelessWindowHint);

    //label->showNormal();
    label->show();
    label->activateWindow();

    return app.exec();
}
