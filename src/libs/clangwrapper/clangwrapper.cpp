#include "clangwrapper.h"
#include "reuse.h"
#include "sourcemarker.h"
#include "unsavedfiledata.h"

#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QTime>

#include <clang-c/Index.h>

#undef DEBUG_TIMING
#undef NEVER_REPARSE_ALWAYS_PARSE

namespace {

static inline QString toString(CXCompletionChunkKind kind)
{
    switch (kind) {
    case CXCompletionChunk_Optional:
        return QLatin1String("Optional");
    case CXCompletionChunk_TypedText:
        return QLatin1String("TypedText");
    case CXCompletionChunk_Text:
        return QLatin1String("Text");
    case CXCompletionChunk_Placeholder:
        return QLatin1String("Placeholder");
    case CXCompletionChunk_Informative:
        return QLatin1String("Informative");
    case CXCompletionChunk_CurrentParameter:
        return QLatin1String("CurrentParameter");
    case CXCompletionChunk_LeftParen:
        return QLatin1String("LeftParen");
    case CXCompletionChunk_RightParen:
        return QLatin1String("RightParen");
    case CXCompletionChunk_LeftBracket:
        return QLatin1String("LeftBracket");
    case CXCompletionChunk_RightBracket:
        return QLatin1String("RightBracket");
    case CXCompletionChunk_LeftBrace:
        return QLatin1String("LeftBrace");
    case CXCompletionChunk_RightBrace:
        return QLatin1String("RightBrace");
    case CXCompletionChunk_LeftAngle:
        return QLatin1String("LeftAngle");
    case CXCompletionChunk_RightAngle:
        return QLatin1String("RightAngle");
    case CXCompletionChunk_Comma:
        return QLatin1String("Comma");
    case CXCompletionChunk_ResultType:
        return QLatin1String("ResultType");
    case CXCompletionChunk_Colon:
        return QLatin1String("Colon");
    case CXCompletionChunk_SemiColon:
        return QLatin1String("SemiColon");
    case CXCompletionChunk_Equal:
        return QLatin1String("Equal");
    case CXCompletionChunk_HorizontalSpace:
        return QLatin1String("HorizontalSpace");
    case CXCompletionChunk_VerticalSpace:
        return QLatin1String("VerticalSpace");
    default:
        return QLatin1String("<UNKNOWN>");
    }
}

static bool clangInitialised = false;
static QMutex initialisationMutex;

static void initClang()
{
    if (clangInitialised)
        return;

    QMutexLocker locker(&initialisationMutex);
    if (clangInitialised)
        return;

    clang_toggleCrashRecovery(1);
    clang_enableStackTraces();
    clangInitialised = true;

    qRegisterMetaType<Clang::Diagnostic>();
    qRegisterMetaType<QList<Clang::Diagnostic> >();
}

} // Anonymous namespace

class Clang::ClangWrapper::PrivateData
{
public:
    PrivateData(bool useForCodeCompletion)
        : m_unit(0)
    {
        initClang();

        const int excludeDeclsFromPCH = 1;
        const int displayDiagnostics = 1;
        m_index = clang_createIndex(excludeDeclsFromPCH, displayDiagnostics);

        m_editingOpts = clang_defaultEditingTranslationUnitOptions();
        if (!useForCodeCompletion)
            m_editingOpts &= ~CXTranslationUnit_CacheCompletionResults;
    }

    ~PrivateData()
    {
        invalidateTranslationUnit();
        if (m_index) {
            clang_disposeIndex(m_index);
            m_index = 0;
        }
    }

    void invalidateTranslationUnit()
    {
        Q_ASSERT(m_index);

        if (m_unit) {
            clang_disposeTranslationUnit(m_unit);
            m_unit = 0;
        }
    }

    bool parseFromFile(const UnsavedFiles &unsavedFiles, bool isEditable = true)
    {
        Q_ASSERT(!m_unit);

        clearDiagnostics();

        if (m_fileName.isEmpty())
            return false;

        m_args.clear();
        const int argc = m_options.size();
        const char **argv = new const char*[argc];
        for (int i = 0; i < argc; ++i) {
            const QByteArray arg = m_options.at(i).toUtf8();
            m_args.append(arg);
            argv[i] = arg.constData();
        }

        const QByteArray fn(m_fileName.toUtf8());
        Clang::Internal::UnsavedFileData unsaved(unsavedFiles);

#ifdef DEBUG_TIMING
        QTime t;t.start();
#endif // DEBUG_TIMING

        if (isEditable) {
            unsigned opts = m_editingOpts;
//            opts = opts & ~CXTranslationUnit_CXXPrecompiledPreamble;
            m_unit = clang_parseTranslationUnit(m_index, fn.constData(), argv, argc, unsaved.files(), unsaved.count(), opts);
        } else {
            m_unit = clang_createTranslationUnitFromSourceFile(m_index, fn.constData(), argc, argv, unsaved.count(), unsaved.files());
        }

#ifdef DEBUG_TIMING
        qDebug() << "=== Parsing"
                 << (isEditable ? "(editable)" : "(not editable")
                 << "->" << (m_unit ? "successful" : "failed")
                 << "in" << t.elapsed() << "ms";
        qDebug() << "Command-line:";
        qDebug("clang -fsyntax-only %s %c", m_fileName.toUtf8().data(), (m_options.isEmpty() ? ' ' : '\\'));
        for (int i = 1; i <= m_options.size(); ++i)
            qDebug("  %s%s", m_options[i - 1].toUtf8().data(), (i == m_options.size() ? "" : " \\"));
#endif // DEBUG_TIMING

        checkDiagnostics();
        delete[] argv;
        return m_unit != 0;
    }

    bool parseFromPCH()
    {
        Q_ASSERT(!m_unit);

        clearDiagnostics();

        if (m_fileName.isEmpty())
            return false;

        m_unit = clang_createTranslationUnit(m_index,
                                             m_fileName.toUtf8().constData());
        checkDiagnostics();
        return m_unit != 0;
    }

    bool save(const QString &fileName) const
    {
        if (!m_unit)
            return false;

        const QByteArray fn(fileName.toUtf8());
        return 0 == clang_saveTranslationUnit(m_unit, fn.constData(), clang_defaultSaveOptions(m_unit));
    }

    void checkDiagnostics()
    {
        if (!m_unit)
            return;

        const unsigned diagCount = clang_getNumDiagnostics(m_unit);
#ifdef DEBUG_TIMING
        qDebug() <<"..." << diagCount << "diagnostics.";
#endif // DEBUG_TIMING
        for (unsigned i = 0; i < diagCount; ++i) {
            CXDiagnostic diag = clang_getDiagnostic(m_unit, i);

            unsigned opt = CXDiagnostic_DisplaySourceLocation
                    | CXDiagnostic_DisplayColumn
                    | CXDiagnostic_DisplaySourceRanges
                    | CXDiagnostic_DisplayOption
                    | CXDiagnostic_DisplayCategoryId
                    | CXDiagnostic_DisplayCategoryName
                    ;
            m_formattedDiagnostics << Internal::getQString(clang_formatDiagnostic(diag, opt));
            clang_disposeDiagnostic(diag);
        }
    }

    void clearDiagnostics()
    {
        m_formattedDiagnostics.clear();
    }

public:
    QString m_fileName;
    QStringList m_options;
    CXIndex m_index;
    unsigned m_editingOpts;
    CXTranslationUnit m_unit;
    QStringList m_formattedDiagnostics;

private:
    QList<QByteArray> m_args;
};

using namespace Clang;
using namespace Clang::Internal;

CodeCompletionResult::CodeCompletionResult()
    : m_priority(0)
    , m_completionKind(Other)
    , m_availability(Available)
    , m_hasParameters(false)
{}

CodeCompletionResult::CodeCompletionResult(unsigned priority)
    : m_priority(priority)
    , m_completionKind(Other)
    , m_availability(Available)
    , m_hasParameters(false)
{
}

const QString &Diagnostic::severityAsString() const
{
    static QStringList strs = QStringList() << "ignored"
                                            << "note"
                                            << "warning"
                                            << "error"
                                            << "fatal" ;
    return strs.at(m_severity);
}

ClangWrapper::ClangWrapper(bool useForCodeCompletion)
    : m_d(new PrivateData(useForCodeCompletion))
    , m_mutex(QMutex::Recursive)
{
}

ClangWrapper::~ClangWrapper()
{
    Q_ASSERT(m_d);

    delete m_d;
    m_d = 0;
}

QString ClangWrapper::fileName() const
{
    Q_ASSERT(m_d);

    return m_d->m_fileName;
}

void ClangWrapper::setFileName(const QString &fileName)
{
    Q_ASSERT(m_d);
    if (m_d->m_fileName != fileName) {
        m_d->m_fileName = fileName;
        m_d->invalidateTranslationUnit();
    }
}

QStringList ClangWrapper::options() const
{
    Q_ASSERT(m_d);

    return m_d->m_options;
}

void ClangWrapper::setOptions(const QStringList &options) const
{
    Q_ASSERT(m_d);

    if (m_d->m_options != options) {
        m_d->m_options = options;
        m_d->invalidateTranslationUnit();
    }
}

bool ClangWrapper::reparse(const UnsavedFiles &unsavedFiles)
{
    Q_ASSERT(m_d);

    m_d->clearDiagnostics();

#ifdef NEVER_REPARSE_ALWAYS_PARSE
    if (m_d->m_unit) {
        clang_disposeTranslationUnit(m_d->m_unit);
        m_d->m_unit = 0;
    }

    return m_d->parseFromFile(unsavedFiles);
#else // !NEVER_REPARSE_ALWAYS_PARSE
    if (!m_d->m_unit)
        return m_d->parseFromFile(unsavedFiles);

    UnsavedFileData unsaved(unsavedFiles);

#ifdef DEBUG_TIMING
    QTime t; t.start();
    qDebug() << "Re-parsring with" << unsaved.count << "unsaved files...";
    qDebug() << "Command-line:";
    qDebug("clang -fsyntax-only %s %c", m_d->m_fileName.toUtf8().data(), (m_d->m_options.isEmpty() ? ' ' : '\\'));
    for (int i = 1; i <= m_d->m_options.size(); ++i)
        qDebug("  %s%s", m_d->m_options[i - 1].toUtf8().data(), (i == m_d->m_options.size() ? "" : " \\"));
#endif // DEBUG_TIMING

    unsigned opts = clang_defaultReparseOptions(m_d->m_unit);
    if (clang_reparseTranslationUnit(m_d->m_unit, unsaved.count(), unsaved.files(), opts) == 0) {
        // success:
#ifdef DEBUG_TIMING
        qDebug() << "-> reparsing successful in" << t.elapsed() << "ms.";
#endif // DEBUG_TIMING

        m_d->checkDiagnostics();
        return true;
    } else {
        // failure:
#ifdef DEBUG_TIMING
        qDebug() << "-> reparsing failed in" << t.elapsed() << "ms.";
#endif // DEBUG_TIMING

        m_d->checkDiagnostics();
        m_d->invalidateTranslationUnit();
        return false;
    }
#endif // NEVER_REPARSE_ALWAYS_PARSE
}

QList<CodeCompletionResult> ClangWrapper::codeCompleteAt(unsigned line, unsigned column, const UnsavedFiles &unsavedFiles)
{
    Q_ASSERT(m_d);

    m_d->clearDiagnostics();

    QList<CodeCompletionResult> completions;

    if (!m_d->m_unit) {
        QTime t;
        t.start();
        m_d->parseFromFile(unsavedFiles);
#ifdef DEBUG_TIMING
        qDebug() << "Initial parse done in" << t.elapsed() << "ms.";
#endif // DEBUG_TIMING
    }

    if (!m_d->m_unit)
        return completions;

#ifdef DEBUG_TIMING
    qDebug() << "codeCompleteAt line" << line << "column" << column;
#endif // DEBUG_TIMING

    const QByteArray fn(m_d->m_fileName.toUtf8());
    UnsavedFileData unsaved(unsavedFiles);
    unsigned opts = clang_defaultCodeCompleteOptions();
    CXCodeCompleteResults *results = clang_codeCompleteAt(m_d->m_unit, fn.constData(), line, column, unsaved.files(), unsaved.count(), opts);
    if (results) {
        for (unsigned i = 0; i < results->NumResults; ++i) {
            CXCompletionString complStr = results->Results[i].CompletionString;
            unsigned priority = clang_getCompletionPriority(complStr);

            CodeCompletionResult ccr(priority);

#if 0
            CXCursorKind kind = results->Results[i].CursorKind;
            qDebug() << "--- kind:"
                     << Internal::getQString(clang_getCursorKindSpelling(kind))
                     << "(" << priority << ")";
#endif

            bool previousChunkWasLParen = false;
            unsigned chunckCount = clang_getNumCompletionChunks(complStr);
            for (unsigned j = 0; j < chunckCount; ++j) {
                CXCompletionChunkKind chunkKind = clang_getCompletionChunkKind(complStr, j);
                const QString chunkText =
                        Internal::getQString(clang_getCompletionChunkText(complStr, j), false);

                if (chunkKind == CXCompletionChunk_TypedText)
                    ccr.setText(chunkText);

                if (chunkKind == CXCompletionChunk_RightParen && previousChunkWasLParen)
                    ccr.setHasParameters(false);

                if (chunkKind == CXCompletionChunk_LeftParen) {
                    previousChunkWasLParen = true;
                    ccr.setHasParameters(true);
                } else {
                    previousChunkWasLParen = false;
                }

                if (!chunkText.isEmpty()) {
                    if (ccr.hint().size() > 0 && ccr.hint().at(ccr.hint().size() - 1).isLetterOrNumber())
                        ccr.setHint(ccr.hint() + QLatin1Char(' '));

                    ccr.setHint(ccr.hint() + chunkText);
                }

#if 0
                qDebug() << "    chunk" << j << ": kind: " << chunkKind << toString(chunkKind);
                qDebug() << "              text:" << chunkText;
#endif
            }

            switch (results->Results[i].CursorKind) {
            case CXCursor_Constructor:
                ccr.setCompletionKind(CodeCompletionResult::ConstructorCompletionKind);
                break;

            case CXCursor_Destructor:
                ccr.setCompletionKind(CodeCompletionResult::DestructorCompletionKind);
                break;

            case CXCursor_CXXMethod:
            case CXCursor_ConversionFunction:
            case CXCursor_FunctionDecl:
            case CXCursor_FunctionTemplate:
            case CXCursor_MemberRef:
            case CXCursor_MemberRefExpr:
                ccr.setCompletionKind(CodeCompletionResult::FunctionCompletionKind);
                break;

            case CXCursor_FieldDecl:
            case CXCursor_VarDecl:
                ccr.setCompletionKind(CodeCompletionResult::VariableCompletionKind);
                break;

            case CXCursor_Namespace:
            case CXCursor_NamespaceAlias:
            case CXCursor_NamespaceRef:
                ccr.setCompletionKind(CodeCompletionResult::NamespaceCompletionKind);
                break;

            case CXCursor_StructDecl:
            case CXCursor_UnionDecl:
            case CXCursor_ClassDecl:
            case CXCursor_TypeRef:
            case CXCursor_TemplateRef:
            case CXCursor_TypedefDecl:
            case CXCursor_ClassTemplate:
            case CXCursor_ClassTemplatePartialSpecialization:
            // TODO: objc cursors
                ccr.setCompletionKind(CodeCompletionResult::ClassCompletionKind);
                break;

            case CXCursor_EnumConstantDecl:
                ccr.setCompletionKind(CodeCompletionResult::EnumeratorCompletionKind);
                break;

            case CXCursor_EnumDecl:
                ccr.setCompletionKind(CodeCompletionResult::EnumCompletionKind);
                break;

            case CXCursor_PreprocessingDirective:
            case CXCursor_MacroDefinition:
            case CXCursor_MacroExpansion:
            case CXCursor_InclusionDirective:
                ccr.setCompletionKind(CodeCompletionResult::PreProcessorCompletionKind);
                break;

            default:
//                qDebug() << "cursor kind:" << results->Results[i].CursorKind << "for" << ccr.text();
                break;
            }

            switch (clang_getCompletionAvailability(complStr)) {
            case CXAvailability_Deprecated:
                ccr.setAvailability(CodeCompletionResult::Deprecated);
                break;

            case CXAvailability_NotAvailable:
                ccr.setAvailability(CodeCompletionResult::NotAvailable);
                break;

            case CXAvailability_NotAccessible:
                ccr.setAvailability(CodeCompletionResult::NotAccessible);
                break;

            default:
                break;
            }

            completions.append(ccr);
        }

        clang_disposeCodeCompleteResults(results);
    } else {
//        m_d->checkDiagnostics();
    }

    return completions;
}

QPair<bool, QStringList> ClangWrapper::precompile(const QString &headerFileName, const QStringList &options, const QString &outFileName)
{
    initClang();

#ifdef DEBUG_TIMING
    qDebug() << "** Precompiling" << outFileName << "from" << headerFileName << "...";
#endif // DEBUG_TIMING

    bool ok;

    ClangWrapper wrapper;
    wrapper.setFileName(headerFileName);
    wrapper.setOptions(options);
    if (wrapper.reparse(UnsavedFiles())) {
        ok = wrapper.m_d->save(outFileName);

#ifdef DEBUG_TIMING
        qDebug() << "** Precompiling done.";
#endif // DEBUG_TIMING
    } else {
#ifdef DEBUG_TIMING
        qWarning() << "** Precompiling failed!";
#endif // DEBUG_TIMING
        ok = false;
    }

    return qMakePair(ok, wrapper.formattedDiagnostics());
}

QStringList ClangWrapper::formattedDiagnostics() const
{
    Q_ASSERT(m_d);

    return m_d->m_formattedDiagnostics;
}

bool ClangWrapper::objcEnabled() const
{
    Q_ASSERT(m_d);

    static const QString objcOption = QLatin1String("-ObjC++");

    return m_d->m_options.contains(objcOption);
}
