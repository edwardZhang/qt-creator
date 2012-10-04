/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: http://www.qt-project.org/
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// This file was generated by qlalr - DO NOT EDIT!
#ifndef GLSLPARSERTABLE_P_H
#define GLSLPARSERTABLE_P_H

#include <qglobal.h>

QT_BEGIN_NAMESPACE

class GLSLParserTable
{
public:
  enum VariousConstants {
    EOF_SYMBOL = 0,
    T_ADD_ASSIGN = 3,
    T_AMPERSAND = 4,
    T_AND_ASSIGN = 5,
    T_AND_OP = 6,
    T_ATTRIBUTE = 7,
    T_BANG = 8,
    T_BOOL = 9,
    T_BREAK = 10,
    T_BVEC2 = 11,
    T_BVEC3 = 12,
    T_BVEC4 = 13,
    T_CARET = 14,
    T_CASE = 15,
    T_CENTROID = 16,
    T_COLON = 17,
    T_COMMA = 18,
    T_COMMENT = 172,
    T_CONST = 19,
    T_CONTINUE = 20,
    T_DASH = 21,
    T_DEC_OP = 22,
    T_DEFAULT = 23,
    T_DISCARD = 24,
    T_DIV_ASSIGN = 25,
    T_DMAT2 = 26,
    T_DMAT2X2 = 27,
    T_DMAT2X3 = 28,
    T_DMAT2X4 = 29,
    T_DMAT3 = 30,
    T_DMAT3X2 = 31,
    T_DMAT3X3 = 32,
    T_DMAT3X4 = 33,
    T_DMAT4 = 34,
    T_DMAT4X2 = 35,
    T_DMAT4X3 = 36,
    T_DMAT4X4 = 37,
    T_DO = 38,
    T_DOT = 39,
    T_DOUBLE = 40,
    T_DVEC2 = 41,
    T_DVEC3 = 42,
    T_DVEC4 = 43,
    T_ELSE = 44,
    T_EQUAL = 45,
    T_EQ_OP = 46,
    T_ERROR = 173,
    T_FALSE = 170,
    T_FEED_EXPRESSION = 2,
    T_FEED_GLSL = 1,
    T_FLAT = 47,
    T_FLOAT = 48,
    T_FOR = 49,
    T_GE_OP = 50,
    T_HIGHP = 51,
    T_IDENTIFIER = 52,
    T_IF = 53,
    T_IN = 54,
    T_INC_OP = 55,
    T_INOUT = 56,
    T_INT = 57,
    T_INVARIANT = 58,
    T_ISAMPLER1D = 59,
    T_ISAMPLER1DARRAY = 60,
    T_ISAMPLER2D = 61,
    T_ISAMPLER2DARRAY = 62,
    T_ISAMPLER2DMS = 63,
    T_ISAMPLER2DMSARRAY = 64,
    T_ISAMPLER2DRECT = 65,
    T_ISAMPLER3D = 66,
    T_ISAMPLERBUFFER = 67,
    T_ISAMPLERCUBE = 68,
    T_ISAMPLERCUBEARRAY = 69,
    T_IVEC2 = 70,
    T_IVEC3 = 71,
    T_IVEC4 = 72,
    T_LAYOUT = 73,
    T_LEFT_ANGLE = 74,
    T_LEFT_ASSIGN = 75,
    T_LEFT_BRACE = 76,
    T_LEFT_BRACKET = 77,
    T_LEFT_OP = 78,
    T_LEFT_PAREN = 79,
    T_LE_OP = 80,
    T_LOWP = 81,
    T_MAT2 = 82,
    T_MAT2X2 = 83,
    T_MAT2X3 = 84,
    T_MAT2X4 = 85,
    T_MAT3 = 86,
    T_MAT3X2 = 87,
    T_MAT3X3 = 88,
    T_MAT3X4 = 89,
    T_MAT4 = 90,
    T_MAT4X2 = 91,
    T_MAT4X3 = 92,
    T_MAT4X4 = 93,
    T_MEDIUMP = 94,
    T_MOD_ASSIGN = 95,
    T_MUL_ASSIGN = 96,
    T_NE_OP = 97,
    T_NOPERSPECTIVE = 98,
    T_NUMBER = 99,
    T_OR_ASSIGN = 100,
    T_OR_OP = 101,
    T_OUT = 102,
    T_PATCH = 103,
    T_PERCENT = 104,
    T_PLUS = 105,
    T_PRECISION = 106,
    T_PREPROC = 171,
    T_QUESTION = 107,
    T_RESERVED = 174,
    T_RETURN = 108,
    T_RIGHT_ANGLE = 109,
    T_RIGHT_ASSIGN = 110,
    T_RIGHT_BRACE = 111,
    T_RIGHT_BRACKET = 112,
    T_RIGHT_OP = 113,
    T_RIGHT_PAREN = 114,
    T_SAMPLE = 115,
    T_SAMPLER1D = 116,
    T_SAMPLER1DARRAY = 117,
    T_SAMPLER1DARRAYSHADOW = 118,
    T_SAMPLER1DSHADOW = 119,
    T_SAMPLER2D = 120,
    T_SAMPLER2DARRAY = 121,
    T_SAMPLER2DARRAYSHADOW = 122,
    T_SAMPLER2DMS = 123,
    T_SAMPLER2DMSARRAY = 124,
    T_SAMPLER2DRECT = 125,
    T_SAMPLER2DRECTSHADOW = 126,
    T_SAMPLER2DSHADOW = 127,
    T_SAMPLER3D = 128,
    T_SAMPLERBUFFER = 129,
    T_SAMPLERCUBE = 130,
    T_SAMPLERCUBEARRAY = 131,
    T_SAMPLERCUBEARRAYSHADOW = 132,
    T_SAMPLERCUBESHADOW = 133,
    T_SEMICOLON = 134,
    T_SLASH = 135,
    T_SMOOTH = 136,
    T_STAR = 137,
    T_STRUCT = 138,
    T_SUBROUTINE = 139,
    T_SUB_ASSIGN = 140,
    T_SWITCH = 141,
    T_TILDE = 142,
    T_TRUE = 169,
    T_TYPE_NAME = 143,
    T_UINT = 144,
    T_UNIFORM = 145,
    T_USAMPLER1D = 146,
    T_USAMPLER1DARRAY = 147,
    T_USAMPLER2D = 148,
    T_USAMPLER2DARRAY = 149,
    T_USAMPLER2DMS = 150,
    T_USAMPLER2DMSARRAY = 151,
    T_USAMPLER2DRECT = 152,
    T_USAMPLER3D = 153,
    T_USAMPLERBUFFER = 154,
    T_USAMPLERCUBE = 155,
    T_USAMPLERCUBEARRAY = 156,
    T_UVEC2 = 157,
    T_UVEC3 = 158,
    T_UVEC4 = 159,
    T_VARYING = 160,
    T_VEC2 = 161,
    T_VEC3 = 162,
    T_VEC4 = 163,
    T_VERTICAL_BAR = 164,
    T_VOID = 165,
    T_WHILE = 166,
    T_XOR_ASSIGN = 167,
    T_XOR_OP = 168,

    ACCEPT_STATE = 462,
    RULE_COUNT = 316,
    STATE_COUNT = 463,
    TERMINAL_COUNT = 175,
    NON_TERMINAL_COUNT = 85,

    GOTO_INDEX_OFFSET = 463,
    GOTO_INFO_OFFSET = 4681,
    GOTO_CHECK_OFFSET = 4681
  };

  static const char  *const    spell [];
  static const short             lhs [];
  static const short             rhs [];
  static const short    goto_default [];
  static const short  action_default [];
  static const short    action_index [];
  static const short     action_info [];
  static const short    action_check [];

  static inline int nt_action (int state, int nt)
  {
    const int yyn = action_index [GOTO_INDEX_OFFSET + state] + nt;
    if (yyn < 0 || action_check [GOTO_CHECK_OFFSET + yyn] != nt)
      return goto_default [nt];

    return action_info [GOTO_INFO_OFFSET + yyn];
  }

  static inline int t_action (int state, int token)
  {
    const int yyn = action_index [state] + token;

    if (yyn < 0 || action_check [yyn] != token)
      return - action_default [state];

    return action_info [yyn];
  }
};


QT_END_NAMESPACE
#endif // GLSLPARSERTABLE_P_H

