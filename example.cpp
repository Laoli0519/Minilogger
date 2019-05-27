#include "minilogger.h"
#include<iostream>
#include<string>

void myfunction(){
    int a = 1;
    int b = 2;
    char arr1[] = "Hello World";
    char arr2[] = "MiniLogger!";


    MiniLog::Logger *logger = new MiniLog::Logger("myfunction", MiniLog::log_level_info);
    logger->info("hello world");
    logger->error("a = {}, b = {}, arr1[0] = {}, arr1 = {}, arr2 = {}.", a, b, arr1[0], arr1, arr2);

    //will not print anything
    logger->debug("a = {}, b = {}, arr1[0] = {}, arr1 = {}, arr2 = {}.", a, b, arr1[0], arr1, arr2);

    //The following operations will be output to the file.
    MiniLog::Logger *logger2 = new MiniLog::Logger("C:/tmp/log.txt", "main", MiniLog::log_level_debug);
    logger2->info("hello world");
    logger2->debug("...");

}


//this is some example for using minilogger
int main(){

    myfunction();
    return 0;

}
