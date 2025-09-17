typedef std::vector<std::vector<std::string>> Matrix;
class EltonOboParser {
public:
    Matrix get_Slims(std::string filename, std::string subset){
        auto t1 = std::chrono::high_resolution_clock::now();
        
        std::ifstream fin = std::ifstream(filename);
        //define regex for slim
        std::regex r("subsetdef: (\\S+) \"(.+?)\"");
        std::string line = "", id = "ID", slimName;
        bool add_true = false, term_true = false;
        Matrix output = {};
        
        while (getline(fin, line)) {
            std::regex r;
            if (std::regex_match(line, r)){
                if (line.find("slim")!=std::string::npos){
                    auto end = line.find(" ");
                    slimName = line.substr(end, line.size());
                    term_true = true;
                } else {term_true=false;}
            }
            else if (line.find("id:") == 0) {
                if (add_true && term_true) {
                    output.push_back({id, slimName});
                }
                auto end = line.find(" ");
                id = line.substr(end, line.size());
            }
            else if (line.find("pir") != std::string::npos) {
                add_true = true;
            } else {
                add_true = false;
            }
            
        }
        fin.close();
        return (output);
    }
};
        
        //here is the help function
        void help(char *argv[]) {
            std::cout << "Gene Ontology, Ugobgu Elton\n";
            usage(argv);
            std::cout << "--get-slims\n";
            std::cout << "\t The number of GO identifiers belonging to all  slim "
            "GO ids\n";
            std::cout << "--get-slim-stats\n";
            std::cout << "\t he number of GO identifiers belonging to a given slim\n";
            std::cout << "--help\n";
            std::cout << "\t shows the help menu\n";
            //          std::cout << "?namespace?\n";
            //          std::cout << "\t one of three options: molecular_function, "
            //                       "cellular_component, biological_process";
            //          std::cout << "?outfile?\n";
            std::cout << "\t name of outfile, must be .obo-file";
        }
        
        
void getslim_stats(Matrix stats) {
    int count_cons = 0;
    int count_no = 0;
    int i;
    for (i = 0; i < stats.size(); i++) {
      if (stats[i][1] == "N.A.") {
        count_no = count_no + 1;
      } else {
        count_cons = count_cons + 1;
      }
    }
    std::cout << "consider \t" << count_cons << "\n";
    std::cout << "no-consider \t" << count_no << "\n";
  }

int main(int argc, const char* argv[]) {
    // Use regular expression to check for a valid file name
    std::regex r(".{1,}\\.obo");

    if (argc < 2) {
        std::cout << "Error: Missing command line arguments." << std::endl;
        return 1; // Exit with an error code
    }
    else{
        if (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0) {
            // Call the help function
             help(argv);
        }
        
        if (argc >= 2 && strcmp(argv[3], "--get-slims") == 0) {
            // Call the get-slims function
        }
        if (argc >= 3 && strcmp(argv[4], "--get-slim-stata") == 0) {
            // Call the get-slim-stata function
        }
        if ((argc >= 4 && strcmp(argv[5], "--get-slim-subset")) || (strcmp(argv[5], "drosophila") == 0 || strcmp(argv[5], "generic") == 0)) {
            // Check for slim subset
        }
        if (argc >= 5 && !std::regex_match(argv[6], r)) {
            std::cout << "Error: File '" << argv[6] << "' does not match the expected format." << std::endl;
            // Check for file name
        }
    }
    return 0;
}
