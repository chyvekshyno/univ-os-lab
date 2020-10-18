//
// Created by tukY on 17.10.2020.
//

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>

#ifndef LAB1_LABUTILS_H
#define LAB1_LABUTILS_H
void copyLinesDividedBy(const std::string& infile, const std::string& outfile, const int& div);

void runWithThreads(const std::string& infile, const int* dividers, const int& div_count);
void runWithProcesses(const std::string& filepath, const int* dividers, const int& div_count);
#endif //LAB1_LABUTILS_H
