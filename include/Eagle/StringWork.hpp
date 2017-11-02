
/*
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef StringWork_H
#define StringWork_H


#include <vector>
#include <string>
#include <list>

/// TODO : Cleanup, organize

extern unsigned int STRINGPRINTF_BUFFER_SIZE;

#if defined __GNUC__
   #define PRINTF_FORMAT_STYLE __attribute__ ((format (printf, 1, 2)))
#else
   #define PRINTF_FORMAT_STYLE
#endif

std::string StringPrintF(const char* format_str , ...) PRINTF_FORMAT_STYLE;/// 1024 character limit!

int CountNewLines(std::string s);

std::vector<std::string> SplitByNewLines(std::string s);

std::vector<std::string> SplitByDelimiterString(std::string string_to_split , const std::string token);

// returns pointer to one past end of quoted string, returns null on failure
const char* CopyQuotedString(const char* start_quote , std::string& s);

const char* SkipWhiteSpace(const char* str);
void TrimTrailingWhiteSpace(char* str);

std::string Replace(std::string str , const char* exp , const char* rep);

char* CStrDup(const char* str);



// Splits a gui string into two, one w/o ampersands, and one with underlines and spaces only
// TODO : Replace with a gui text drawing function
std::string GetGuiText(std::string gui_text);// gui_text carries ampersands prepended to characters that should be underlined
std::string GetGuiUnderlineText(std::string gui_text);// returns underline text only, based off of ampersand markers in text


std::string GetFileExtension(std::string& s);

std::list<std::string> TokenizeString(const std::string& str , char delimiter);

std::string RemoveTrailingSlash(std::string str);
std::string FixFilenameSlashes(const std::string& str);

std::string GetShortFileName(const std::string& s);

std::string StripFileName(const std::string& s);


int MatchingParenthesisIndex(const char* cstr);

int ReverseStringSearch(const char* str , int start , char c);// returns offset from start or 1 if not found

int StringSearch(const char* str , int start , char c);// returns offset from start or -1 if not found
int StringSearch(const char* str , int start , const char* find);// returns offset from start or -1 if not found

std::string FormatDouble(double d);
std::string FormatDoubleString(std::string str);





#endif // StringWork_H


