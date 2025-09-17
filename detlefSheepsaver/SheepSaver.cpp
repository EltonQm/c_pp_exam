#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<ctype.h>
#include <cmath>
using namespace std;

class SheepSaver{

    public:

    void save(int** board,int size,int win){
        ofstream file ("output.html");
        
        file << "<html>\n";
        file << "<head>\n";
        file << "<style>\n";
        file << "table { border-collapse: collapse; }\n";
        file << "td { width: 50px; height: 50px; text-align: center; }\n";
        file << ".white { background-color: white; }\n";
        file << ".black { background-color: gray; }\n";
        file <<".sheep {width:20px; height:20px; border-radius: 50%; background-color: yellow; position: relative; left: 50%; right:50%; top:20%; transform: translate(-50%, -50%);}\n";
        file <<".wolf {width:20px; height:20px; border-radius: 50%; background-color: green; position: relative;  left: 50%; right:50%; top:20%; transform: translate(-50%, -50%);}\n";
        file << "</style>\n";
        file << "</head>\n";
        file << "<body>\n";
        file << "<table>\n";

        for (int row = 0; row < size; ++row) {
            file << "<tr>\n";

            for (int col = 0; col < size; ++col) {
                file << "<td class=\"";
                if (board[row][col]==3){
                    file<<"black\">";
                     file<<"<div class=\"wolf\"></div>";
                     file << "</td>\n";
                }else if(board[row][col]==4){
                    file<<"black\">";
                    file<<"<div class=\"sheep\"></div>";
                    file << "</td>\n";
                    
                }else{
                file << ((row + col) % 2 == 0 ? "white" : "black");
                file << "\"></td>\n";
                }
                            
            }
            file << "<td class=\"white\">";
            file<< "<div>"<<(size-row)<<"</div>";
            file << "</td>\n";
            file << "</tr>\n";
        }

        file << "<tr>\n"; // Add a new row for the letters
        for (int i=0;i<size;++i){
            file << "<td class=\"white\">";
            char let = 'A' + i;
            file<< "<div>"<<let<<"</div>";
            file << "</td>\n";
        }




        file << "</table>\n";
        file << "</body>\n";
        file << "</html>\n";

    }
};