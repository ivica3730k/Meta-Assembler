#pragma once
#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

std::vector<std::string> keywords = { "$", "DF", "GROUP", "ORG", "*", "DGROUP", "GT", "%OUT", "+",
		"DOSSEG", "HIGH", "PAGE", "_", "DQ", "IF", "PARA,", ".", "DS", "IF1", "PROC", "/", "DT",
		"IF2", "PTR", "=", "DUP", "IFB", "PUBLIC", "?", "DW", "IFDEF", "PURGE", "DWORD",
		"IFGIF", "QWORD", ".186", "ELSE", "IFDE", ".RADIX", ".286", "END", "IFIDN",
		"RECORD", ".286P", "ENDIF", "IFNB", "REPT", ".287", "ENDM", "IFNDEF", ".SALL", ".386",
		"ENDP", "INCLUDE", "SEG", ".386P", "ENDS", "INCLUDELIB", "SEGMENT",
		".387", "EQ", "IRP", ".SEQ", ".8086", "EQU", "IRPC", ".SFCOND", ".8087", ".ERR",
		"LABEL", "SHL", "ALIGN", ".ERR1", ".LALL", "SHORT", ".ALPHA", ".ERR2", "LARGE",
		"SHR", "AND", ".ERRB", "LE", "SIZE", "ASSUME", ".ERRDEF", "LENGTH", "SMALL", "AT",
		".ERRDIF", ".LFCOND", "STACK", "BYTE", ".ERRE", ".LIST", "@STACK", ".CODE",
		".ERRIDN", "LOCAL", ".STACK", "@CODE", ".ERRNB", "LOW", "STRUC", "@CODESIZE",
		".ERRNDEF", "LT", "SUBTTL", "COMM", ".ERRNZ", "MACRO", "TBYTE", "COMMENT", "EVEN",
		"MASK", ".TFCOND", ".CONST", "EXITM", "MEDIUM", "THIS", ".CREF", "EXTRN", "MOD", "TITLE",
		"@CURSEG", "FAR", ".MODEL", "TYPE", "@DATA", "@FARDATA", "NAME", ".TYPE", ".DATA", ".FARDATA",
		"NE", "WIDTH", "@DATA?", "@FARDATA?", "NEAR", "WORD", ".DATA",
		"?", ".FARDATA ?", "NOT", "@WORDSIZE", "@DATASIZE", "@FILENAME", "NOTHING",
		".XALL", "DB", "FWORD", "OFFSET", ".XCREP", "DD", "GE", "OR", ".XLIST", "XOR" };