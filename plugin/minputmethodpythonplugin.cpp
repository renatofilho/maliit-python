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

#include <Python.h>
#include <QFileInfo>
#include <QDebug>
#include <shiboken.h>

#include "minputmethodpythonplugin.h"

PyTypeObject **SbkmaliitTypes;

class MInputMethodPythonPluginPrivate
{

public:
    MInputMethodPythonPluginPrivate(const QString &pythonFile);
    ~MInputMethodPythonPluginPrivate();
    bool load(const QString &pythonFile);

    MInputMethodPlugin *m_plugin;

private:
    PyObject *m_pyPlugin;
};

MInputMethodPythonPluginPrivate::MInputMethodPythonPluginPrivate(const QString &pythonFile)
    : m_plugin(0), m_pyPlugin(0)
{
    if (!load(pythonFile))
        qWarning() << "Invalid python plugin:" << pythonFile;
}

bool MInputMethodPythonPluginPrivate::load(const QString &pythonFile)
{
    PyObject *mainModule;
    PyObject *mainLocals;
    PyObject *fromlist;
    PyObject *module;

    Py_Initialize();
    Shiboken::init();
    Shiboken::GilState pyGil;

    Shiboken::importModule("maliit", &SbkmaliitTypes);
    Shiboken::TypeResolver* sbkType = Shiboken::TypeResolver::get("MInputMethodPlugin*");
    if (!sbkType) {
        qWarning() << "Plugin type not found.";
        return false;
    }
    PyObject *pluginType = reinterpret_cast<PyObject*>(sbkType->pythonType());

    mainModule = PyImport_AddModule("__main__");
    mainLocals = PyModule_GetDict(mainModule);

    //Append our plugin path to system
    QFileInfo pluginPath(pythonFile);
    qDebug() << "STR:" << pythonFile << "PATH" << pluginPath.path() << "FILE" << pluginPath.baseName();
    PyObject *sys_path = PySys_GetObject(const_cast<char*>("path"));
    PyObject *path = PyString_FromString(qPrintable(pluginPath.path()));
    if (PySequence_Contains(sys_path, path) == 0)
        PyList_Insert(sys_path, 0, path);
    Py_DECREF(path);

    fromlist = PyTuple_New(0);
    module = PyImport_ImportModuleEx(const_cast<char*>(qPrintable(pluginPath.baseName())), mainLocals, mainLocals, fromlist);
    Py_DECREF(fromlist);
    if (!module) {
        PyErr_Print();
        return false;
    }

    Py_ssize_t pos=0;
    PyObject *key;
    PyObject *value;
    PyObject *locals = PyModule_GetDict (module);
    bool error = false;

    while(PyDict_Next(locals, &pos, &key, &value)) {
        if (!PyType_Check(value))
            continue;

        if (PyObject_IsSubclass (value, pluginType) && (value != pluginType)) {
            PyObject *args = PyTuple_New(0);
            m_pyPlugin = PyObject_Call(value, args, 0);
            Py_DECREF(args);
            if (!m_pyPlugin || PyErr_Occurred()) {
                PyErr_Print();
                error = true;
                break;
            }
            m_plugin = reinterpret_cast<MInputMethodPlugin*>(Shiboken::Object::cppPointer(reinterpret_cast<SbkObject*>(m_pyPlugin),
                                                                                          reinterpret_cast<PyTypeObject*>(pluginType)));
            break;
        }
    }

    Q_ASSERT(m_plugin);

    Py_DECREF(mainModule);
    Py_DECREF(module);

    if (error)
        Py_XDECREF(m_pyPlugin);

    return !error;
}

MInputMethodPythonPluginPrivate::~MInputMethodPythonPluginPrivate()
{
    Py_DECREF(m_pyPlugin); //this will destroy C++
    m_plugin = 0;
}


MInputMethodPythonPlugin::MInputMethodPythonPlugin(const QString &pythonFile)
    : d_ptr(new MInputMethodPythonPluginPrivate(pythonFile))
{
}

MInputMethodPythonPlugin::~MInputMethodPythonPlugin()
{
    delete d_ptr;
}

QString MInputMethodPythonPlugin::name() const
{
    if (!d_ptr->m_plugin)
        return QString();

    return d_ptr->m_plugin->name();
}

QStringList MInputMethodPythonPlugin::languages() const
{
    if (!d_ptr->m_plugin)
        return QStringList();

    return d_ptr->m_plugin->languages();
}

MAbstractInputMethod *MInputMethodPythonPlugin::createInputMethod(MAbstractInputMethodHost *host,
                                                                 QWidget *mainWindow)
{
    if (!d_ptr->m_plugin)
        return 0;

    return d_ptr->m_plugin->createInputMethod(host, mainWindow);
}

MAbstractInputMethodSettings *MInputMethodPythonPlugin::createInputMethodSettings()
{
    if (!d_ptr->m_plugin)
        return 0;

    return d_ptr->m_plugin->createInputMethodSettings();
}

QSet<MInputMethod::HandlerState> MInputMethodPythonPlugin::supportedStates() const
{
    if (!d_ptr->m_plugin)
        return QSet<MInputMethod::HandlerState>();

    return d_ptr->m_plugin->supportedStates();
}
