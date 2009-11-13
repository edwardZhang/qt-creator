/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef CPPQUICKFIX_H
#define CPPQUICKFIX_H

#include <texteditor/icompletioncollector.h>

#include <cplusplus/CppDocument.h>

#include <QtCore/QSharedPointer>
#include <QtGui/QTextCursor>

namespace CppTools {
    class CppModelManagerInterface;
} // end of namespace CppTools

namespace CppEditor {
namespace Internal {

class CPPEditor;
class QuickFixOperation;
typedef QSharedPointer<QuickFixOperation> QuickFixOperationPtr;

class QuickFixOperation
{
    Q_DISABLE_COPY(QuickFixOperation)

public:
    QuickFixOperation(CPlusPlus::Document::Ptr doc,
                      const CPlusPlus::Snapshot &snapshot);

    virtual ~QuickFixOperation();

    virtual QString description() const = 0;
    virtual void apply(QTextCursor cursor) = 0;

    CPlusPlus::Document::Ptr document() const { return _doc; }
    CPlusPlus::Snapshot snapshot() const { return _snapshot; }

    QTextCursor textCursor() const;
    void setTextCursor(const QTextCursor &tc);

protected:
    const CPlusPlus::Token &tokenAt(unsigned index) const;
    void getTokenStartPosition(unsigned index, unsigned *line,
                               unsigned *column) const;
    void getTokenEndPosition(unsigned index, unsigned *line,
                             unsigned *column) const;

    QTextCursor cursor(unsigned index) const;
    QTextCursor moveAtStartOfToken(unsigned index) const;
    QTextCursor moveAtEndOfToken(unsigned index) const;

private:
    CPlusPlus::Document::Ptr _doc;
    CPlusPlus::Snapshot _snapshot;
    QTextCursor _textCursor;
};

class CPPQuickFixCollector: public TextEditor::IQuickFixCollector
{
    Q_OBJECT

public:
    CPPQuickFixCollector();
    virtual ~CPPQuickFixCollector();

    virtual bool supportsEditor(TextEditor::ITextEditable *editor);
    virtual bool triggersCompletion(TextEditor::ITextEditable *editor);
    virtual int startCompletion(TextEditor::ITextEditable *editor);
    virtual void completions(QList<TextEditor::CompletionItem> *completions);
    virtual void complete(const TextEditor::CompletionItem &item);
    virtual void cleanup();

private:
    CppTools::CppModelManagerInterface *_modelManager;
    CPPEditor *_editor;
    QList<TextEditor::CompletionItem> _quickFixItems;
};

} // end of namespace Internal
} // end of namespace CppEditor

#endif // CPPQUICKFIX_H
