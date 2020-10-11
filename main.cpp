#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <exception>
#include <thread>

using namespace std;

const char* INVALID_FILEPATH = "Filepath is invalid";

list<string>* findLinesDividesBy(const string& filepath, const int& div)
{
    ifstream stream(filepath);
    if(!stream.is_open())
        throw std::invalid_argument(INVALID_FILEPATH);

    auto* lines = new list<string>();
    string line;
    while (std::getline(stream, line))
    {
        if (line.empty() || line.size() % div)
            continue;

        lines->push_back(line);
    }
    if (stream.is_open())
        stream.close();

    return lines;
}

void writeInFile(const list<string>* lines, const string& filepath)
{
    ofstream stream(filepath);
    if (!stream.is_open())
        throw std::invalid_argument(INVALID_FILEPATH);

    for (const auto& line : *lines)
        stream << line << endl;

    if (stream.is_open())
        stream.close();
}

/*
 *  argc = N
 *  argv:
 *      first       :   input filepath
 *      last N-1    :   dividers
 *
 *  result files have name : inputFilePath + dividerNumber + .res
 */
int main(int argc, char** argv) {

    string in_file_path = (string) argv[1];
    string out_file_path;

    int divider;

    auto* workers = new thread[argc - 2];

    for(int i = 2; i < argc; ++i)
    {
        divider = atoi(argv[i]);
        out_file_path = in_file_path
                .substr(0, in_file_path.find('.'))
                .append("_")
                .append(argv[i])
                .append(".res");

        workers[i-2] = thread([divider, in_file_path, out_file_path](){
            try {
                writeInFile(findLinesDividesBy(in_file_path, divider), out_file_path);
            }
            catch (std::ios_base::failure& fail) {
                cerr << fail.what();
            } catch (std::exception& e) {
                cerr << e.what();
            }
        });
    }

    for (int i = 0; i < argc - 2; ++i) {
        if(workers[i].joinable())
            workers[i].join();
    }

    return 0;
}
