//
// Created by Artem Shemidko on 09.12.2020.
//

#include "Editor.h"
#include <QFileInfo>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexeravs.h>
#include <Qsci/qscilexercoffeescript.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjson.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexercmake.h>
#include <Qsci/qscilexermakefile.h>
#include <qdebug.h>
#include <QAction>

Editor::Editor(QWidget *parent)
    : QsciScintilla(parent) {
    setUtf8(true);
    setWrapMode(WrapNone);

    auto f = font();
    f.setPointSize(15);
    setFont(f);

    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setIndentationsUseTabs(false);
    setIndentationWidth(4);
    setBackspaceUnindents(true);
    setIndentationGuides(true);
}

void Editor::setFile(QFile *fileIn) {
    if (file) {
        delete file;
    }
    if (lex) {
        delete lex;
    }
    file = fileIn;
    setLex(file->fileName());
    connect(this, &Editor::textChanged, this, [=] { edited = true; });
}

void Editor::setLex(const QString &fileName) {
    QString ext = QFileInfo(fileName).completeSuffix();
    auto name = fileName.mid(fileName.lastIndexOf('/') + 1);
    if (ext == "avs")
        lex = new QsciLexerAVS(this);
    else if (ext == "sh")
        lex = new QsciLexerBash(this);
    else if (ext == "bat")
        lex = new QsciLexerBatch(this);
    else if (ext == "coffee")
        lex = new QsciLexerCoffeeScript(this);
    else if (ext == "cpp" || ext == "h" || ext == "hpp") {
        auto temp = new QsciLexerCPP(this);
        temp->setFoldAtElse(true);
        temp->setFoldComments(true);
        temp->setFoldCompact(false);
        temp->setFoldPreprocessor(true);
        lex = temp;
    }
    else if (ext == "cmake" || name == "CMakeLists.txt")
        lex = new QsciLexerCMake(this);
    else if (ext == "cs")
        lex = new QsciLexerCSharp(this);
    else if (ext == "css")
        lex = new QsciLexerCSS(this);
    else if (ext == "html")
        lex = new QsciLexerHTML(this);
    else if (ext == "java")
        lex = new QsciLexerJava(this);
    else if (ext == "js")
        lex = new QsciLexerJavaScript(this);
    else if (ext == "json")
        lex = new QsciLexerJSON(this);
    else if (ext == "py")
        lex = new QsciLexerPython(this);
    else if (ext == "xml" || ext == "ui" || ext == "glade")
        lex = new QsciLexerXML(this);
    else if (name == "Makefile")
        lex = new QsciLexerMakefile(this);
    if (lex) {
        setLexer(lex);
    }
}

[[maybe_unused]] void Editor::fold_by_level(int level) {
    qDebug() << "triggered";
    if (level) {
        level += 0x400;
        auto MASK = QsciScintilla::SC_FOLDLEVELNUMBERMASK;
        for (auto line = 0; line < lines(); ++line) {
            auto foldlevel = SendScintilla(QsciScintilla::SCI_GETFOLDLEVEL, line) & MASK;
            qDebug() << "line=" << line + 1 << ", level=" << foldlevel;
            if (foldlevel == level) {
                line = SendScintilla(QsciScintilla::SCI_GETFOLDPARENT, line);
                if (SendScintilla(QsciScintilla::SCI_GETFOLDEXPANDED, line)) {
                    qDebug() << ", foldline:" << line + 1;
                    SendScintilla(QsciScintilla::SCI_FOLDLINE, line,
                                  QsciScintilla::SC_FOLDACTION_CONTRACT);
                }

            }
            qDebug();
        }

    }
    else {
        SendScintilla(QsciScintilla::SCI_FOLDALL, QsciScintilla::SC_FOLDACTION_EXPAND);
    }
}

Editor::~Editor() {
    if (file) {
        delete file;
    }
    if (lex)
        delete lex;
}

const bool &Editor::isEdited() const {
    return edited;
}

void Editor::setEdited(const bool &edited) {
    Editor::edited = edited;
}

bool Editor::findPrevious() {
//    qDebug() << SCI_GETT;
//    SCI_SEARCHPREV(SCI_GETSEARCHFLAGS, SCI_GETCHA);
//    SCI_SETSEARCHFLAGS = SCI_SEARCHPREV;
//    findNext();
}
