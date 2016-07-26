



#ifndef Text_HPP
#define Text_HPP



#include <vector>
#include <string>


class EagleFont;



void GetTextAttributes(std::string text , EagleFont* font , int line_spacing , int* pnlines ,
                       std::vector<std::string>* plines , std::vector<int>* pwidths_vector ,
                       int* pmaxwidth , int* ptotalheight);




extern const int TOPIC_TEXT_WIDGET;



int GetNextFreeTextMessageId();





#endif // Text_HPP
