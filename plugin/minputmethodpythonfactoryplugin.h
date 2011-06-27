 /*
 * Copyright (C) 2011 INdT
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

#ifndef M_INPUT_METHOD_PYTHON_FACTORY_PLUGIN
#define M_INPUT_METHOD_PYTHON_FACTORY_PLUGIN

#include <minputmethodfactoryplugin.h>
#include <minputmethodplugin.h>

#include <QString>

//! \brief Factory used to create QuickPlugins
class MInputMethodPythonFactoryPlugin
    : public QObject, public MInputMethodFactoryPlugin
{
    Q_OBJECT
    Q_INTERFACES(MInputMethodFactoryPlugin)

public:
    MInputMethodPythonFactoryPlugin();
    virtual ~MInputMethodPythonFactoryPlugin();

    //! \reimp
    virtual QString mimeType() const;
    virtual MInputMethodPlugin* create(const QString &fileName) const;
    //! \reimp_end

private:
    Q_DISABLE_COPY(MInputMethodPythonFactoryPlugin);
};


#endif // M_INPUT_METHOD_PYTHON_FACTORY_PLUGIN
