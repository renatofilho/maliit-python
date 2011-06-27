 /*
 * Copyright (C) 2011 INdT
 * All rights reserved.
 *
 * If you have questions regarding the use of this file, please contact
 * Nokia at directui@nokia.com.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#include <Python.h>
#include "minputmethodpythonfactoryplugin.h"

#include <shiboken.h>
#include <QString>
#include <QFileInfo>
#include <QDebug>

PyTypeObject** SbkmaliitTypes;

namespace {
    MInputMethodPlugin *loadPythonFile(const QString &pythonFile)
    {
        PyObject *mainModule;
        PyObject *mainLocals;
        PyObject *fromlist;
        PyObject *module;
        MInputMethodPlugin *pluginObject;

        Py_Initialize();
        Shiboken::init();
        Shiboken::GilState pyGil;

        Shiboken::importModule("maliit", &SbkmaliitTypes);
        Shiboken::TypeResolver* sbkType = Shiboken::TypeResolver::get("MInputMethodPlugin*");
        if (!sbkType) {
            qWarning() << "Plugin type not found.";
            return 0;
        }
        PyObject *pluginType = reinterpret_cast<PyObject*>(sbkType->pythonType());

        mainModule = PyImport_AddModule("__main__");
        mainLocals = PyModule_GetDict(mainModule);

        //Append our plugin path to system
        QFileInfo pluginPath(pythonFile);
        PyObject *sys_path = PySys_GetObject(const_cast<char*>("path"));
        PyObject *path = PyString_FromString(qPrintable(pluginPath.path()));
        if (PySequence_Contains(sys_path, path) == 0)
            PyList_Insert(sys_path, 0, path);
        Py_DECREF(path);

        fromlist = PyTuple_New(0);
        qDebug() << "PATH" <<  pythonFile << "name" << pluginPath.baseName();
        module = PyImport_ImportModuleEx(const_cast<char*>(qPrintable(pluginPath.baseName())), mainLocals, mainLocals, fromlist);
        Py_DECREF(fromlist);
        if (!module) {
            PyErr_Print();
            return 0;
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
                PyObject *pyPlugin = PyObject_Call(value, args, 0);
                Py_DECREF(args);
                if (!pyPlugin || PyErr_Occurred()) {
                    PyErr_Print();
                    error = true;
                    break;
                }
                pluginObject = reinterpret_cast<MInputMethodPlugin*>(Shiboken::Object::cppPointer(reinterpret_cast<SbkObject*>(pyPlugin),
                                                                                                  reinterpret_cast<PyTypeObject*>(pluginType)));
                if (pluginObject)
                    Shiboken::Object::releaseOwnership(reinterpret_cast<SbkObject*>(pyPlugin)); // transfer ownership to C++

                Py_XDECREF(pyPlugin);
                break;
            }
        }

        Q_ASSERT(pluginObject);
        Py_DECREF(mainModule);
        Py_DECREF(module);

        return pluginObject;
    }
} // namespace

MInputMethodPythonFactoryPlugin::MInputMethodPythonFactoryPlugin()
{
}

MInputMethodPythonFactoryPlugin::~MInputMethodPythonFactoryPlugin()
{
}

QString MInputMethodPythonFactoryPlugin::mimeType() const
{
    return "py";
}

MInputMethodPlugin* MInputMethodPythonFactoryPlugin::create(const QString &fileName) const
{
    return loadPythonFile(fileName);
}

Q_EXPORT_PLUGIN2(pymaliit, MInputMethodPythonFactoryPlugin)
