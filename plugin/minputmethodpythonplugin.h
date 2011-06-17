/*
    This file is part of PySide project.
    Copyright (C) 2009-2011 Nokia Corporation and/or its subsidiary(-ies).
    Contact: PySide team <contact@pyside.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef M_INPUT_METHOD_PYTHON_PLUGIN
#define M_INPUT_METHOD_PYTHON_PLUGIN

#include <minputmethodplugin.h>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QSet>

class MInputMethodPythonPluginPrivate;

class MInputMethodPythonPlugin
    : public MInputMethodPlugin
{
public:
    MInputMethodPythonPlugin(const QString &pythonFile);
    virtual ~MInputMethodPythonPlugin();

    virtual QString name() const;
    virtual QStringList languages() const;
    virtual MAbstractInputMethod *createInputMethod(MAbstractInputMethodHost *host,
                                                    QWidget *mainWindow);
    virtual MAbstractInputMethodSettings *createInputMethodSettings();
    virtual QSet<MInputMethod::HandlerState> supportedStates() const;

private:
    Q_DISABLE_COPY(MInputMethodPythonPlugin);
    Q_DECLARE_PRIVATE(MInputMethodPythonPlugin);
    MInputMethodPythonPluginPrivate *const d_ptr;
};

Q_DECLARE_INTERFACE(MInputMethodPythonPlugin,
                    "com.meego.meegoimframework.MInputMethodPythonPlugin/1.1")

#endif // M_INPUT_METHOD_PYTHON_PLUGIN
