from PySide.QtCore import QRect
from PySide.QtGui import QPushButton, QRegion, qApp
from maliit import MInputMethodPlugin, MInputMethod, MAbstractInputMethod

class PythonKeyboard(MAbstractInputMethod):
    EXAMPLE_SUBVIEW_ID = "ExamplePluginSubview1"

    def __init__(self, host, mainWindow):
        MAbstractInputMethod.__init__(self, host, mainWindow)

        self._showIsInhibited = False
        self._showRequested = False
        self._mainWidget = QPushButton("Hello World by PySide", mainWindow)
        self._mainWidget.clicked[None].connect(self.onButtonClicked)

        host.sendCommitString("Maliit")
        host.sendPreeditString("Mali", [], 0, 6)

    def onButtonClicked(self):
        self.inputMethodHost().sendCommitString(self._mainWidget.text())

    def show(self):
        self._showRequested = True
        if self._showIsInhibited:
            return

        screenSize = qApp.desktop().screenGeometry().size()
        self._mainWidget.parentWidget().resize(screenSize)

        imGeometry = QRect(0, screenSize.height() - 200, screenSize.width(), 200);
        self._mainWidget.setGeometry(imGeometry)

        self.inputMethodHost().setScreenRegion(QRegion(self._mainWidget.geometry()))
        self.inputMethodHost().setInputMethodArea(QRegion(self._mainWidget.geometry()))

        self._mainWidget.show()


    def hide(self):
        if not self._showRequested:
            return;
        self._showRequested = False
        self._mainWidget.hide()
        self.inputMethodHost().setScreenRegion(QRegion())
        self.inputMethodHost().setInputMethodArea(QRegion())

    def subViews(self, state):
        subViews = []
        if state == MInputMethod.OnScreen:
            subView1 = MAbstractInputMethod.MInputMethodSubView()
            subView1.subViewId = PythonKeyboard.EXAMPLE_SUBVIEW_ID
            subView1.subViewTitle = "Example plugin subview 1";
            subViews.append(subView1)

        return subViews

    def activeSubView(self, state):
        if state == MInputMethod.OnScreen:
            return PythonKeyboard.EXAMPLE_SUBVIEW_ID
        else:
            return ""

    def setState(self, states):
        if MInputMethod.OnScreen in states:
            if self._showRequested and  not self._showIsInhibited:
                self._mainWidget.show()
        else:
            self.mainWidget.hide()

    def handleClientChange(self):
        if self._showRequested:
            self.hide()

    def handleVisualizationPriorityChange(self, inhibitShow):
        if self._showIsInhibited == inhibitShow:
            return

        self._showIsInhibited = inhibitShow
        if self._showRequested:
            if inhibitShow:
                self._mainWidget.hide()
            else:
                self._mainWidget.show()


class PyMaliit(MInputMethodPlugin):
    def __init__(self):
        MInputMethodPlugin.__init__(self)
        pass

    def name(self):
        return "PyMaliit"

    def languages(self):
        return ['en']

    def createInputMethod(self, host, mainWindow):
        return PythonKeyboard(host, mainWindow)

    def createInputMethodSettings(self):
        return None

    def supportedStates(self):
        return [MInputMethod.OnScreen]
