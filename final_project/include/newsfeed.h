#include <string>
#include "DFRobot_RGBLCD.h"

class Newsfeed
{

public:

    Newsfeed(char * content);
    char * firstHeadlineStart;
    int firstHeadlineLength;

    char * secondHeadlineStart;
    int secondHeadlineLength;

    char * thirdHeadlineStart;
    int thirdHeadlineLength;

    char * buffer;

    char news[500];


    void printNews(DFRobot_RGBLCD &lcd, int &displayState);


    void extractHeadlines(); // This will extract the headlines from the RSS server response
};