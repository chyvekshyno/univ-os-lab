#include "labutils.h"

using namespace std;

/**
 *  @argc N
 *  @argv
 *      @first input filepath; Need to be .txt extension
 *      @second type of run : 0 =>'thread' or 1 =>'process'
 *      @last dividers
 *
 *  @output result files have name : inputFilePath + dividerNumber + .res
 */
int main(int argc, char** argv) {

    string filepath = (string) argv[1];
    const int divcount = argc - 3;

    int dividers[argc-divcount];
    for(int i = 0; i < divcount; ++i)
        dividers[i] = atoi(argv[i+3]);

    int typo = atoi(argv[2]);
    switch (typo)
    {
        case 0: runWithThreads(filepath, dividers, divcount);
            break;
        case 1: runWithProcesses(filepath, dividers, divcount);
            break;
        default:
            cerr << "\nInvalid_argument of type : " << argv[2] << endl;
    }

    return 0;
}
