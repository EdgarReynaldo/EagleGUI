
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file GlobalText.hpp
 * @brief The interface for some text helper functions, used by the text widget classes
 *
 */

#ifndef GlobalText_HPP
#define GlobalText_HPP



#include <vector>
#include <string>




class EagleFont;



/**! Stores the text attributes of a specific string and font with specified line spacing and count
 *   Stores the lines, line widths, max width, and total height of the text with the specified attributes
 */

void GetTextAttributes(std::string text , EagleFont* font , int line_spacing , int* pnlines ,
                       std::vector<std::string>* plines , std::vector<int>* pwidths_vector ,
                       int* pmaxwidth , int* ptotalheight);




extern const unsigned int TOPIC_TEXT_WIDGET;///< The default topic ID for all text widgets



unsigned int GetNextFreeTextMessageId();///< For text widgets to create a new message ID



#endif // GlobalText_HPP



