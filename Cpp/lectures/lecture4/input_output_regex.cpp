/*
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
*/
/*
int main()
{

    string data; // changed in 2022

    // open a file in write mode
    ofstream outfile;
    outfile.open("afile.dat");

    cout << "Writing to the file" << endl;
    cout << "Enter your name: ";
    cin >> data;

    // write inputted data into the file
    outfile << data << endl;

    cout << "Enter your age: ";
    cin >> data; // or std::getline(cin,data);
    cin.ignore();

    // again write inputted data into the file
    outfile << data << endl;

    // close the opened file
    outfile.close();

    // open a file in read mode.
    ifstream infile;
    infile.open("afile.dat");

    cout << "Reading from the file " << endl;
    infile >> data;

    // write the data at the screen
    cout << data << endl;

    // again read the data from the file and display it
    infile >> data;
    cout << data << endl;

    // close the opened file
    infile.close();

    return 0;
}
*/
/*
A stringstream associates a string object with a stream allowing you to read from the string as if it
were a stream(like cin). To use stringstream, we need to include sstream header file.
The stringstream class is extremely useful in parsing input.
*/
/*
#include <bits/stdc++.h>
int main()
{
    std::string S, T;
    S = "Hello C++ World!";

    std::stringstream X(S);
    while (std::getline(X, T, ' '))
    {
        std::cout << T << std::endl;
    }
    return 0;
}
*/

/*
// grep in c++

#include <iostream>
#include <vector>
#include <regex>
#include <string>

namespace dutils
{
    // a c++ grep which works like the R grep
    std::vector<int> grep(std::string pattern, std::string str,
                          const std::regex::flag_type &flag = std::regex::basic)
    {
        std::regex rx;
        rx = std::regex(pattern, flag);
        std::vector<int> index_matches; // result saved here
        for (auto it = std::sregex_iterator(str.begin(), str.end(), rx);
             it != std::sregex_iterator(); ++it)
        {
            index_matches.push_back(it->position());
        }
        return (index_matches);
    }

    std::vector<int> grep(std::string pattern, std::vector<std::string> vstring, const std::regex::flag_type &flag = std::regex::basic)
    {
        std::regex rx;
        rx = std::regex(pattern, flag);
        std::vector<int> index_matches = {}; // result saved here
        int i = 0;
        for (auto el : vstring)
        {
            if (std::regex_search(el, rx))
            {
                index_matches.push_back(i);
            }
            i = i + 1;
        }
        return (index_matches);
    }
} // END OF NAMESPACE

int main(int argc, char **argv)
{
    std::vector<int> res = dutils::grep("[Hh][ea]", "Hello and hallo world!");
    for (auto r : res)
        std::cout << r << std::endl;
    for (auto i : dutils::grep("H[ea]", {"Hello", "World!", "Hallo", "Welt", "by", "hallo"},
                               std::regex::icase))
    {
        std::cout << i << std::endl;
    }
}
*/
/*
// overloading c++
#include <iostream>
#include <vector>
#include <numeric>
template <typename T>
double mean(std::vector<T> x)
{
    double sum = std::accumulate(x.begin(), x.end(), 0);
    return (sum / x.size());
}

template <typename T>
std::vector<double> mean(std::vector<std::vector<T>> x)
{
    std::vector<double> res;
    for (auto vec : x)
    {
        res.push_back(mean(vec));
    }
    return (res);
}

int main(int argc, char **argv)
{
    typedef std::vector<int> Vector;
    typedef std::vector<std::vector<int>> Matrix;

    Vector x = {1, 2, 3, 4, 5, 6, 10};
    Matrix M = {
        {0, 1, 2, 6}, {4, 5, 6, 7}, {8, 9, 10, 19}};
    auto xm = mean(x);
    std::cout << "Mean of vector: " << xm << std::endl;
    auto ms = mean(M);
    for (auto v : ms)
    {
        std::cout << "Mean of column: " << v << std::endl;
    }
}
*/

/*
// Itearator and Matches
#include <regex>
#include <iterator>
#include <iostream>
#include <string>

int main()
{
    const std::string s = "Tutorialspoint.com india pvt ltd,";

    std::regex words_regex("[^\\s]+");
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end = std::sregex_iterator();

    std::cout << "Found " << std::distance(words_begin, words_end) << " words:\n";
    for (std::sregex_iterator i = words_begin;
         i != words_end; ++i)
    {
        std::smatch match = *i; // a match object
        std::string match_str = match.str();
        std::cout << match_str << '\n';
    }
}
*/
/*
// Open a FASTA file and give out every sequence wih its length

// IMatrix example - Tabfile reading
class IMatrix
{
    typedef std::vector<std::string> names;
    typedef std::vector<std::vector<int>> IntMatrix;

    void readTabfile(std::string filename)
    {
        names rnms;
        names cnms;

        std::string rname, cname, line;
        row r;
        int rownumber = 0;
        std::string scell;
        int icell;

        // open a file in read mode
        std::ifstream infile;
        infile.open(filename);

        IntMatrix tmt;
        while (std::getline(infile, line))
        {
            rownumber += 1;
            std::istringstream iss(line);
            if (rownumber == 1)
            {
                iss >> scell; // skip rownames entry
                while (iss >> scell)
                {
                    cnms.push_back(scell);
                }
            }
            else
            {
                iss >> scell;
                r.clear();
                rnms.push_back(scell);
                while ((iss >> icell))
                {
                    r.push_back(icell);
                }
                tmt.push_back(r);
            }
        }
    }
    infile.close();
    // return;
    this->rnames = rnms;
    this->cnames = cnms;
    this->mt = tmt;
    std::cout << "done" << std::endl;
}
*/
/*
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class IMatrix
{
public:
    using names = std::vector<std::string>;
    using row = std::vector<int>;
    using IntMatrix = std::vector<row>;

    void readTabfile(const std::string &filename);

    // For testing/printing
    void printMatrix() const;

private:
    names rnames;
    names cnames;
    IntMatrix mt;
};

void IMatrix::readTabfile(const std::string &filename)
{
    names rnms;
    names cnms;
    IntMatrix tmt;

    std::ifstream infile(filename);
    if (!infile)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    int rownumber = 0;

    while (std::getline(infile, line))
    {
        rownumber += 1;
        std::istringstream iss(line);
        std::string scell;
        row r;

        if (rownumber == 1)
        {
            iss >> scell; // skip top-left cell (usually a label)
            while (iss >> scell)
            {
                cnms.push_back(scell);
            }
        }
        else
        {
            iss >> scell;
            rnms.push_back(scell);
            int icell;
            while (iss >> icell)
            {
                r.push_back(icell);
            }
            tmt.push_back(r);
        }
    }

    infile.close();
    this->rnames = rnms;
    this->cnames = cnms;
    this->mt = tmt;
    std::cout << "done" << std::endl;
}

// Optional: print the loaded matrix
void IMatrix::printMatrix() const
{
    std::cout << "   ";
    for (const auto &c : cnames)
        std::cout << c << " ";
    std::cout << "\n";

    for (size_t i = 0; i < mt.size(); ++i)
    {
        std::cout << rnames[i] << " ";
        for (int val : mt[i])
            std::cout << val << " ";
        std::cout << "\n";
    }
}
*/
/*
Usage Example (in main()):
int main() {
    IMatrix matrix;
    matrix.readTabfile("data.tab");  // use your real tab file
    matrix.printMatrix();
    return 0;
}
*/
/*

// IMatrix example - Tabfile writing
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class IMatrix
{
public:
    using names = std::vector<std::string>;
    using row = std::vector<int>;
    using IntMatrix = std::vector<row>;

    void writeTabfile(const std::string &filename);

    // Mocked helpers — these should be implemented in your class
    int nrow() const { return mt.size(); }
    int ncol() const { return mt.empty() ? 0 : mt[0].size(); }

    names autonames(int count, const std::string &prefix) const
    {
        names autoNames;
        for (int i = 0; i < count; ++i)
        {
            autoNames.push_back(prefix + std::to_string(i + 1));
        }
        return autoNames;
    }

private:
    names rnames;
    names cnames;
    IntMatrix mt;
};

void IMatrix::writeTabfile(const std::string &filename)
{
    names rnms, cnms;

    // Use given column names or generate default ones
    if (cnames.empty())
    {
        cnms = autonames(ncol(), "C");
    }
    else
    {
        cnms = cnames;
    }

    // Use given row names or generate default ones
    if (rnames.empty())
    {
        rnms = autonames(nrow(), "R");
    }
    else
    {
        rnms = rnames;
    }

    std::ofstream outfile(filename, std::ios::out | std::ios::trunc);
    if (!outfile)
    {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Write header
    outfile << "RowNames";
    for (const auto &col : cnms)
    {
        outfile << "\t" << col;
    }
    outfile << "\n";

    // Write matrix
    for (size_t i = 0; i < mt.size(); ++i)
    {
        outfile << rnms[i];
        for (const auto &val : mt[i])
        {
            outfile << "\t" << val;
        }
        outfile << "\n";
    }

    outfile.close();
    std::cout << "Tabfile written to " << filename << std::endl;
}
*/

// span
/*
✅ Summary
Feature	Benefit
Lightweight wrapper	 -  No copying, just views
Safer alternative to T*	-  Avoids separate pointer/size
Interop with C arrays	-  Great for modernizing C interfaces in C++
Part of C++20 -	Include <span>, compile with -std=c++20

*/
/*
#include <iostream>
#include <span>
using namespace std;

void printInts(std::span<int> nums) {
    for (int x : nums) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> vec = {6, 7, 8};

    printInts(arr);       // OK — wraps raw array
    printInts(vec);       // OK — wraps vector
    printInts({9, 10, 11}); // OK — initializer list

    return 0;
}
*/
/*
Important Notes
std::span does not own the data. If the underlying array goes out of scope, the span becomes invalid.

You can also use std::span<const T> to make it read-only.
*/
/*
std::span<T> (since C++20) is a lightweight, non-owning view over a contiguous sequence of elements —
like:

C-style arrays (int arr[10])

std::vector<T>

std::array<T, N>

It provides a safe and convenient way to work with raw arrays without copying the data or sacrificing
 type safety.

🧠 Why use std::span?
Benefit	Explanation
✅ Safer than raw pointers -	Includes size info (like bounds-checked access)
✅ Does not own memory -	You can "view" data without copying or reallocating
✅ Easy to pass to functions - Replaces passing T* and size separately
✅ Works with many containers -	Can wrap vector, array, raw arrays, etc.
*/

#include <iostream>
#include <string>
#include <regex>

int main()
{
    std::string s("remove me please Hello   World");
    // std::string s("string");
    // std::string s = "string";
    // std::string s {"string"}; // preferred way(?)
    std::regex r(".+(Hello) + (World)");
    std::cout << std::regex_replace(s, r, "$1 $2!\n");
    std::cout << std::regex_replace("hw!\n", std::regex("hw"), "Hello World II");

    if (std::regex_search(">id test\nMALF", std::regex("^>[^\\s]+")))
    {
        std::cout << "FASTA header searched\n"; // YES
    }
    if (std::regex_match(">id test\nMALF", std::regex("^>[^\\s]+")))
    {
        std::cout << "FASTA header matched I\n"; // NO
    }
    if (std::regex_match(">id test\nMALF", std::regex("^>[^\\s]+.+\n.+")))
    {
        std::cout << "FASTA header matched II\n"; // YES
    }
    return 0;
}