import qbs

Project {
    name: "CPlusPlus autotests"
    references: [
        "ast/ast.qbs",
        "checksymbols/checksymbols.qbs",
        "codeformatter/codeformatter.qbs",
        "cxx11/cxx11.qbs",
        "findusages/findusages.qbs",
        "lexer/lexer.qbs",
        "lookup/lookup.qbs",
        "misc/misc.qbs",
        "preprocessor/preprocessor.qbs",
        "semantic/semantic.qbs",
        "simplifytypes/simplifytypes.qbs",
        "typeprettyprinter/typeprettyprinter.qbs"
    ]
}
