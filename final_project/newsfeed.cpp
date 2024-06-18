#include "include/newsfeed.h"
#include <string.h>
#include <iostream>

Newsfeed::Newsfeed(char * content)
{
    buffer = content;
}

void Newsfeed::printNews(DFRobot_RGBLCD &lcd, int &displayState) {

    lcd.setCursor(0, 0);
    lcd.printf("News");
    
    for(int i=0; i < strlen(news); i++) {
        lcd.setCursor(0, 1);
        lcd.printf("%.*s", 16, news + i);
        if (displayState != 4) {
            break;
        }
        thread_sleep_for(200);
    }
}


void Newsfeed::extractHeadlines()
{
    // Start and end of first item
    char * firstItemStart = strstr(buffer, "<item>");
    char * firstItemEnd = strstr(firstItemStart, "</item>");

    // Start and end of second item

    char * secondItemStart = strstr(firstItemEnd, "<item>");
    char * secondItemEnd = strstr(secondItemStart, "</item>");


    // Start and end of third item
    char * thirdItemStart = strstr(secondItemEnd, "<item>");
    char * thirdItemEnd = strstr(thirdItemStart, "</item>");


    // First headline
    char firstHeadlineContent[100];
    memset(firstHeadlineContent,0,100);

    firstHeadlineStart = strstr(firstItemStart, "<title><![CDATA[") + 16;
    char * firstHeadlineEnd = strstr(firstHeadlineStart, "]]></title>");

    memcpy(firstHeadlineContent, firstHeadlineStart, (firstHeadlineEnd-firstHeadlineStart));
    firstHeadlineLength = firstHeadlineEnd-firstHeadlineStart;


    // Second headline

    char secondHeadlineContent[100];
    memset(secondHeadlineContent, 0,100);

    secondHeadlineStart = strstr(secondItemStart, "<title><![CDATA[") + 16;
    char * secondHeadlineEnd = strstr(secondItemStart, "]]></title>");
    memcpy(secondHeadlineContent, secondHeadlineStart, (secondHeadlineEnd-secondHeadlineStart));
    secondHeadlineLength = secondHeadlineEnd-secondHeadlineStart;


    // Third headline
    char thirdHeadlineContent[100];
    memset(thirdHeadlineContent, 0, 100);

    thirdHeadlineStart = strstr(thirdItemStart, "<title><![CDATA[") + 16;
    char * thirdHeadlineEnd = strstr(thirdItemStart, "]]></title>");

    memcpy(thirdHeadlineContent, thirdHeadlineStart, (thirdHeadlineEnd-thirdHeadlineStart));
    thirdHeadlineLength = thirdHeadlineEnd-thirdHeadlineStart;

    std::strncat(news, firstHeadlineStart, firstHeadlineLength);
    std::strcat(news, " | ");
    std::strncat(news, secondHeadlineStart, secondHeadlineLength);
    std::strcat(news, " | ");
    std::strncat(news, thirdHeadlineStart, thirdHeadlineLength);
    std::strcat(news, " | ");

}