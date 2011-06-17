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

#include "pymaliit.h"
#include <stdio.h>

PyMaliit::PyMaliit()
    : QObject(0), MInputMethodPythonPlugin(PYTHON_PLUGIN_FILENAME)
{
}

Q_EXPORT_PLUGIN2(pymaliit, PyMaliit)
