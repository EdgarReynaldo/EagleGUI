
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file StringWork.hpp
 * @brief An interface for working with strings in Eagle
 */

#ifndef StringWork_H
#define StringWork_H



#include <vector>
#include <string>
#include <list>



extern size_t STRINGPRINTF_BUFFER_SIZE;///< How large our working string print f buffer should be, defaults to 1024



#if defined __GNUC__
   #define PRINTF_FORMAT_STYLE __attribute__ ((format (printf, 1, 2)))
#else
   #define PRINTF_FORMAT_STYLE
#endif

///< Allows you to output formatted text to a std::string
std::string StringPrintF(const char* format_str , ...) PRINTF_FORMAT_STYLE;/// 1024 character limit!


///< Gets an iterator to the specified position by line and caret position
bool GetPositionIterator(std::string& selText , std::string::iterator* itPos , int caretLine , int caretPos);



///< Gets iterators to the specified selection
bool GetSelectionIterators(std::string& selText ,
                           std::string::iterator* itLeft , std::string::iterator* itRight,
                           int select_line_start , int select_line_close ,
                           int select_left , int select_right);


bool GetNextWord(const std::string& text , int caretPos , int caretLine , int* newCaretPos , int* newCaretLine);
bool GetPreviousWord(const std::string& text , int caretPos , int caretLine , int* newCaretPos , int* newCaretLine);

bool GetNextWord(bool search_forward , const std::string& text , int caretPos , int caretLine , int* newCaretPos , int* newCaretLine);


///< Returns the newline count in s
int CountNewlines(std::string s);
int CountLines(std::string s);

///< Returns a vector of lines from s
std::vector<std::string> SplitByNewLinesChomp(std::string s);
std::vector<std::string> SplitByNewLinesNoChomp(std::string s);

///< Returns a tokenized vector of strings using token as a separator
std::vector<std::string> SplitByDelimiterString(std::string string_to_split , const std::string token);

///< Returns the sub string starting at zero index and stopping when 'stop' char is found
std::string ReadStringUntil(const std::string& str , const char stop);

///< Skips characters in expr string until openchar is reached, then reads chars until closechar is reached or the end of the string
std::string GetArgumentList(const std::string& expr , const char openchar , const char closechar);



///< Returns pointer to one past end of quoted string, returns null on failure.
///< Stores the string minus quotes in s
const char* CopyQuotedString(const char* start_quote , std::string& s);


///< Skips and advances past all leading white space
const char* SkipWhiteSpace(const char* str);

///< Chops off all trailing whitespace
void TrimTrailingWhiteSpace(char* str);

///< Replace all occurrences of exp with rep and return the new string
std::string Replace(std::string str , const char* exp , const char* rep);

///< Duplicate a c string, must call @ref free when done
char* CStrDup(const char* str);



/**! @fn GetGuiText <std::string>
 *   @fn GetGuiUnderlineText <std::string>
 *   @brief Splits a gui string into two, one w/o ampersands, and one with underlines and spaces only
 * 
 *   TODO : Replace with a gui text drawing function
 */

///< gui text carries ampersands prepended to characters that should be underlined
std::string GetGuiText(std::string gui_text);

///< Returns underline text only, based off of ampersand markers in text, using ' ' as placeholders (may not work for variable width fonts)
std::string GetGuiUnderlineText(std::string gui_text);


///< Get the characters after the last .
std::string GetFileExtension(std::string& s);

///< Tokenize a string by 'delimiter' character
std::list<std::string> TokenizeString(const std::string& str , char delimiter);

///< Remove any trailing slash
std::string RemoveTrailingSlash(std::string str);

///< Fix all filename slashes so they are platform appropriate
std::string FixFilenameSlashes(const std::string& str);

///< Get the short file name (without extension)
std::string GetShortFileName(const std::string& s);

///< Remove the file name from the end of the path
std::string StripFileName(const std::string& s);


///< Get the index of the parenthesis matching the one at cstr[0]
int MatchingParenthesisIndex(const char* cstr);

///< Returns negative offset from start or 1 if not found
int ReverseStringSearch(const char* str , int start , char c);

///< Returns positive offset from start or -1 if not found
int StringSearch(const char* str , int start , char c);

///< Returns positive offset of the expression 'find' from start or -1 if not found
int StringSearch(const char* str , int start , const char* find);

///< Format a double into a string using a specific format
std::string FormatDouble(double d);

///< Takes any double string and formats it in a specific way - [-|+]#.##+E[-|+]### so basically -1.234e-5
///< The precision is based on the number of numerals in the string minus any trailing zeros
std::string FormatDoubleString(std::string str);




#endif // StringWork_H




