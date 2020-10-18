#include "labutils.h"

using namespace std;

const char* INVALID_FILEPATH = "Filepath is invalid";

int time_microseconds(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end)
{
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

void copyLinesDividedBy(const string& infile, const string& outfile, const int& div)
{
    auto start = chrono::system_clock::now();

    ifstream istream(infile);
    if(!istream.is_open())
        throw std::invalid_argument(INVALID_FILEPATH);

    ofstream ostream(outfile);
    if(!ostream.is_open())
        throw std::invalid_argument(INVALID_FILEPATH);

    string line;
    while (std::getline(istream, line))
    {
        if (line.empty() || line.size() % div)
            continue;

        ostream << line << endl;
    }

    auto end = chrono::system_clock::now();

    //  log results
    ostream << "\n<===================>"
            << "\nProgram runtime : " << time_microseconds(start, end) << " microseconds" << endl;

    if (istream.is_open())
        istream.close();
    if (ostream.is_open())
        ostream.close();
}

string generateOutFilePath(const string& filepath, const int& num, const char* ext)
{
    return filepath
            .substr(0, filepath.find(".txt"))
            + "_" + to_string(num) + ext;
}

void runWithThreads(const string& infile, const int* dividers, const int& div_count)
{
    auto start = chrono::system_clock::now();

    auto* workers = new thread[div_count];

    string outfile;
    int divider;

    for(int i = 0; i < div_count; ++i)
    {
        divider = dividers[i];
        outfile = generateOutFilePath(infile, divider, ".thread");

        workers[i] = thread([divider, infile, outfile](){
            try {
                copyLinesDividedBy(infile, outfile, divider);
            }
            catch (std::ios_base::failure& fail) {
                cerr << fail.what() << endl;
            } catch (std::exception& e) {
                cerr << e.what() << endl;
            }
        });
    }

    for (int i = 0; i < div_count; ++i) {
        if(workers[i].joinable())
            workers[i].join();
    }

    auto end = chrono::system_clock::now();
    ofstream("thread.log")  << "Summary runtime of all threads : "
                            << time_microseconds(start, end)
                            << " microseconds";
}

void runWithProcesses(const string& infile, const int* dividers, const int& div_count)
{
    auto start = chrono::system_clock::now();

    string outfile;
    int divider;

    pid_t pid, wpid;

    int counter = 0;
    while (counter < div_count)
    {
        divider = dividers[counter];
        outfile = generateOutFilePath(infile, divider, ".process");

        pid = fork();
        if (pid == -1) {    //  error of creating process
            cerr << "Error with creating process; counter = " << to_string(counter) << endl;
            break;
        } else if (pid == 0){   //  child's code
            try {
                copyLinesDividedBy(infile, outfile, divider);
            } catch (std::ios_base::failure& fail) {
                cerr << fail.what() << endl;
            } catch (std::exception& e) {
                cerr << e.what() << endl;
            } break;
        } else {    //  parent's code
            ++counter;
        }
    }

    //  wait for all processes
    int status = 0;
    while((wpid = wait(&status)) > 0);

    //  log
    auto end = chrono::system_clock::now();
    ofstream("process.log")  << "Summary runtime of all threads : "
                             << time_microseconds(start, end)
                             << " microseconds";
}
