/****************************************************************************
**
** Copyright (C) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
**
** This file is part of the QtCore module of the Qt Toolkit, plus some
** modifications by PySide team.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#undef QT_NO_STL
#undef QT_NO_STL_WCHAR
#define Q_BYTE_ORDER // used to enable QSysInfo.Endian detection on MacOSX

#include "QtCore/qnamespace.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

class QByteArray;

class QString;

#ifndef Q_MOC_OUTPUT_REVISION
#define Q_MOC_OUTPUT_REVISION 61
#endif

// macro for onaming members
#ifdef METHOD
#undef METHOD
#endif
#ifdef SLOT
#undef SLOT
#endif
#ifdef SIGNAL
#undef SIGNAL
#endif

Q_CORE_EXPORT const char *qFlagLocation(const char *method);

#define QTOSTRING_HELPER(s) #s
#define QTOSTRING(s) QTOSTRING_HELPER(s)
#ifndef QT_NO_DEBUG
# define QLOCATION "\0"__FILE__":"QTOSTRING(__LINE__)
# define METHOD(a)   qFlagLocation("0"#a QLOCATION)
# define SLOT(a)     qFlagLocation("1"#a QLOCATION)
# define SIGNAL(a)   qFlagLocation("2"#a QLOCATION)
#else
# define METHOD(a)   "0"#a
# define SLOT(a)     "1"#a
# define SIGNAL(a)   "2"#a
#endif

#ifdef QT3_SUPPORT
#define METHOD_CODE   0                        // member type codes
#define SLOT_CODE     1
#define SIGNAL_CODE   2
#endif

#define QMETHOD_CODE  0                        // member type codes
#define QSLOT_CODE    1
#define QSIGNAL_CODE  2

#define Q_ARG(type, data) QArgument<type >(#type, data)
#define Q_RETURN_ARG(type, data) QReturnArgument<type >(#type, data)

class QObject;
class QMetaMethod;
class QMetaEnum;
class QMetaProperty;
class QMetaClassInfo;


class Q_CORE_EXPORT QGenericArgument
{
public:
    inline QGenericArgument(const char *aName = 0, const void *aData = 0)
        : _data(aData), _name(aName) {}
    inline void *data() const { return const_cast<void *>(_data); }
    inline const char *name() const { return _name; }

private:
    const void *_data;
    const char *_name;
};

class Q_CORE_EXPORT QGenericReturnArgument: public QGenericArgument
{
public:
    inline QGenericReturnArgument(const char *aName = 0, void *aData = 0)
        : QGenericArgument(aName, aData)
        {}
};

template <class T>
class QArgument: public QGenericArgument
{
public:
    inline QArgument(const char *aName, const T &aData)
        : QGenericArgument(aName, static_cast<const void *>(&aData))
        {}
};


template <typename T>
class QReturnArgument: public QGenericReturnArgument
{
public:
    inline QReturnArgument(const char *aName, T &aData)
        : QGenericReturnArgument(aName, static_cast<void *>(&aData))
        {}
};

struct Q_CORE_EXPORT QMetaObject
{
    const char *className() const;
    const QMetaObject *superClass() const;

    QObject *cast(QObject *obj) const;

#ifndef QT_NO_TRANSLATION
    // ### Qt 4: Merge overloads
    QString tr(const char *s, const char *c) const;
    QString trUtf8(const char *s, const char *c) const;
    QString tr(const char *s, const char *c, int n) const;
    QString trUtf8(const char *s, const char *c, int n) const;
#endif // QT_NO_TRANSLATION

    int methodOffset() const;
    int enumeratorOffset() const;
    int propertyOffset() const;
    int classInfoOffset() const;

    int constructorCount() const;
    int methodCount() const;
    int enumeratorCount() const;
    int propertyCount() const;
    int classInfoCount() const;

    int indexOfConstructor(const char *constructor) const;
    int indexOfMethod(const char *method) const;
    int indexOfSignal(const char *signal) const;
    int indexOfSlot(const char *slot) const;
    int indexOfEnumerator(const char *name) const;
    int indexOfProperty(const char *name) const;
    int indexOfClassInfo(const char *name) const;

    QMetaMethod constructor(int index) const;
    QMetaMethod method(int index) const;
    QMetaEnum enumerator(int index) const;
    QMetaProperty property(int index) const;
    QMetaClassInfo classInfo(int index) const;
    QMetaProperty userProperty() const;

    static bool checkConnectArgs(const char *signal, const char *method);
    static QByteArray normalizedSignature(const char *method);
    static QByteArray normalizedType(const char *type);

    // internal index-based connect
    static bool connect(const QObject *sender, int signal_index,
                        const QObject *receiver, int method_index,
                        int type = 0, int *types = 0);
    // internal index-based disconnect
    static bool disconnect(const QObject *sender, int signal_index,
                           const QObject *receiver, int method_index);
    // internal slot-name based connect
    static void connectSlotsByName(QObject *o);

    // internal index-based signal activation
    static void activate(QObject *sender, int signal_index, void **argv);
    static void activate(QObject *sender, int from_signal_index, int to_signal_index, void **argv);
    static void activate(QObject *sender, const QMetaObject *, int local_signal_index, void **argv);
    static void activate(QObject *sender, const QMetaObject *, int from_local_signal_index, int to_local_signal_index, void **argv);
    // internal guarded pointers
    static void addGuard(QObject **ptr);
    static void removeGuard(QObject **ptr);
    static void changeGuard(QObject **ptr, QObject *o);

    static bool invokeMethod(QObject *obj, const char *member,
                             Qt::ConnectionType,
                             QGenericReturnArgument ret,
                             QGenericArgument val0 = QGenericArgument(0),
                             QGenericArgument val1 = QGenericArgument(),
                             QGenericArgument val2 = QGenericArgument(),
                             QGenericArgument val3 = QGenericArgument(),
                             QGenericArgument val4 = QGenericArgument(),
                             QGenericArgument val5 = QGenericArgument(),
                             QGenericArgument val6 = QGenericArgument(),
                             QGenericArgument val7 = QGenericArgument(),
                             QGenericArgument val8 = QGenericArgument(),
                             QGenericArgument val9 = QGenericArgument());

    static inline bool invokeMethod(QObject *obj, const char *member,
                             QGenericReturnArgument ret,
                             QGenericArgument val0 = QGenericArgument(0),
                             QGenericArgument val1 = QGenericArgument(),
                             QGenericArgument val2 = QGenericArgument(),
                             QGenericArgument val3 = QGenericArgument(),
                             QGenericArgument val4 = QGenericArgument(),
                             QGenericArgument val5 = QGenericArgument(),
                             QGenericArgument val6 = QGenericArgument(),
                             QGenericArgument val7 = QGenericArgument(),
                             QGenericArgument val8 = QGenericArgument(),
                             QGenericArgument val9 = QGenericArgument())
    {
        return invokeMethod(obj, member, Qt::AutoConnection, ret, val0, val1, val2, val3,
                val4, val5, val6, val7, val8, val9);
    }

    static inline bool invokeMethod(QObject *obj, const char *member,
                             Qt::ConnectionType type,
                             QGenericArgument val0 = QGenericArgument(0),
                             QGenericArgument val1 = QGenericArgument(),
                             QGenericArgument val2 = QGenericArgument(),
                             QGenericArgument val3 = QGenericArgument(),
                             QGenericArgument val4 = QGenericArgument(),
                             QGenericArgument val5 = QGenericArgument(),
                             QGenericArgument val6 = QGenericArgument(),
                             QGenericArgument val7 = QGenericArgument(),
                             QGenericArgument val8 = QGenericArgument(),
                             QGenericArgument val9 = QGenericArgument())
    {
        return invokeMethod(obj, member, type, QGenericReturnArgument(), val0, val1, val2,
                                 val3, val4, val5, val6, val7, val8, val9);
    }

    static inline bool invokeMethod(QObject *obj, const char *member,
                             QGenericArgument val0 = QGenericArgument(0),
                             QGenericArgument val1 = QGenericArgument(),
                             QGenericArgument val2 = QGenericArgument(),
                             QGenericArgument val3 = QGenericArgument(),
                             QGenericArgument val4 = QGenericArgument(),
                             QGenericArgument val5 = QGenericArgument(),
                             QGenericArgument val6 = QGenericArgument(),
                             QGenericArgument val7 = QGenericArgument(),
                             QGenericArgument val8 = QGenericArgument(),
                             QGenericArgument val9 = QGenericArgument())
    {
        return invokeMethod(obj, member, Qt::AutoConnection, QGenericReturnArgument(), val0,
                val1, val2, val3, val4, val5, val6, val7, val8, val9);
    }

    QObject *newInstance(QGenericArgument val0 = QGenericArgument(0),
                         QGenericArgument val1 = QGenericArgument(),
                         QGenericArgument val2 = QGenericArgument(),
                         QGenericArgument val3 = QGenericArgument(),
                         QGenericArgument val4 = QGenericArgument(),
                         QGenericArgument val5 = QGenericArgument(),
                         QGenericArgument val6 = QGenericArgument(),
                         QGenericArgument val7 = QGenericArgument(),
                         QGenericArgument val8 = QGenericArgument(),
                         QGenericArgument val9 = QGenericArgument()) const;

    enum Call {
        InvokeMetaMethod,
        ReadProperty,
        WriteProperty,
        ResetProperty,
        QueryPropertyDesignable,
        QueryPropertyScriptable,
        QueryPropertyStored,
        QueryPropertyEditable,
        QueryPropertyUser,
        CreateInstance
    };

    int static_metacall(Call, int, void **) const;

#ifdef QT3_SUPPORT
    QT3_SUPPORT const char *superClassName() const;
#endif

    struct { // private data
        const QMetaObject *superdata;
        const char *stringdata;
        const uint *data;
        const void *extradata;
    } d;
};

struct QMetaObjectExtraData
{
    const QMetaObject **objects;
    int (*static_metacall)(QMetaObject::Call, int, void **);
};

inline const char *QMetaObject::className() const
{ return d.stringdata; }

inline const QMetaObject *QMetaObject::superClass() const
{ return d.superdata; }

#ifdef QT3_SUPPORT
inline const char *QMetaObject::superClassName() const
{ return d.superdata ? d.superdata->className() : 0; }
#endif

QT_END_NAMESPACE

QT_END_HEADER

#define qdoc

#if 1
  #define Q_WS_X11
#elif 0
  #define Q_WS_MAC
#elif 0
  #include "pysidewtypes.h"
  #define Q_WS_WIN
#elif 0
  #define Q_WS_SIMULATOR
#endif

// There are symbols in Qt that exist in Debug but
// not in release
#define QT_NO_DEBUG

#include "QtCore/QtCore"
#if 0 || 0
  // Workaround to parse the QApplication header
  #define Q_INTERNAL_QAPP_SRC
  #undef qdoc
#endif
#include "QtGui/QtGui"
#include "qpytextobject.h"  // PySide class
#if 1
  #include "QtGui/QX11Info"
  #include "QtGui/QX11EmbedContainer"
  #include "QtGui/QX11EmbedWidget"
#elif 0
  #include "QtGui/qmacstyle_mac.h"
#endif
#include "QtXml/QtXml"
#include "QtUiTools/QtUiTools"
#include "QtNetwork/QtNetwork"
#include "QtScript/QtScript"
#include "QtScriptTools/QtScriptTools"
#include "QtMultimedia/QtMultimedia"
#include <QtMaemo5/QtMaemo5>
#include "QtDeclarative/QtDeclarative"

// QT_GUI_LIB must be defined to QSqlRelationalDelegate become visible
#define QT_GUI_LIB
#undef Q_DECLARE_INTERFACE
#include "QtSql/QtSql"
#include "QtSvg/QtSvg"

#if 1
#  include "QtXmlPatterns/QtXmlPatterns"
#endif

#if 1
#  include "QtWebKit/QtWebKit"
#endif

#if 1
#  include "QtTest/QtTest"
#if 7 > 5
#  include "pysideqtesttouch.h"
#endif
#endif

// Phonon
#include "phonon/pyside_phonon.h"
#include "phonon/abstractaudiooutput.h"
#include "phonon/abstractmediastream.h"
#include "phonon/abstractvideooutput.h"
#include "phonon/addoninterface.h"
#include "phonon/audiooutput.h"
#include "phonon/audiooutputinterface.h"
#include "phonon/backendcapabilities.h"
#include "phonon/backendinterface.h"
#include "phonon/effect.h"
#include "phonon/effectinterface.h"
#include "phonon/effectparameter.h"
#include "phonon/effectwidget.h"
#include "phonon/mediacontroller.h"
#include "phonon/medianode.h"
#include "phonon/mediaobject.h"
#include "phonon/mediaobjectinterface.h"
#include "phonon/mediasource.h"
#include "phonon/objectdescription.h"
#include "phonon/objectdescriptionmodel.h"
#include "phonon/path.h"
#include "phonon/phonon_export.h"
#include "phonon/phonondefs.h"
#include "phonon/phononnamespace.h"
#include "phonon/platformplugin.h"
#include "phonon/seekslider.h"
#include "phonon/streaminterface.h"
#include "phonon/videoplayer.h"
#include "phonon/videowidget.h"
#include "phonon/videowidgetinterface.h"
#include "phonon/volumefadereffect.h"
#include "phonon/volumefaderinterface.h"
#include "phonon/volumeslider.h"

//QtHelp need be included after QtSql
#include "QtHelp/QtHelp"

#ifndef QT_NO_OPENGL
#include </usr/include/GL/gl.h>
#include <QtOpenGL/QtOpenGL>
#endif // QT_NO_OPENGL

