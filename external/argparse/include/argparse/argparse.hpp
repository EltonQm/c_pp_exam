// Simplified argparse.hpp for this project
// This is a minimal implementation to get the project building

#pragma once
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace argparse
{

    enum class nargs_pattern
    {
        at_least_one
    };

    class ArgumentParser
    {
    private:
        std::string program_name;
        std::string description;
        std::map<std::string, std::string> defaults;
        std::vector<std::string> positional_args;
        std::vector<std::string> mutually_exclusive_groups;

    public:
        std::map<std::string, std::vector<std::string>> arguments;
        std::map<std::string, bool> used_args;

    public:
        ArgumentParser(const std::string &name) : program_name(name) {}

        void add_description(const std::string &desc)
        {
            description = desc;
        }

        class Argument
        {
        private:
            ArgumentParser *parser;
            std::string name;
            std::string help_text;
            std::string default_val;
            nargs_pattern nargs_pattern_val;
            bool is_mutually_exclusive = false;

        public:
            Argument(ArgumentParser *p, const std::string &n) : parser(p), name(n) {}

            Argument &help(const std::string &h)
            {
                help_text = h;
                return *this;
            }

            Argument &nargs(nargs_pattern pattern)
            {
                nargs_pattern_val = pattern;
                return *this;
            }

            Argument &default_value(const std::string &val)
            {
                default_val = val;
                parser->defaults[name] = val;
                return *this;
            }

            void set_mutually_exclusive(bool exclusive)
            {
                is_mutually_exclusive = exclusive;
            }
        };

        class MutuallyExclusiveGroup
        {
        private:
            ArgumentParser *parser;

        public:
            MutuallyExclusiveGroup(ArgumentParser *p) : parser(p) {}

            Argument add_argument(const std::string &name)
            {
                Argument arg(parser, name);
                arg.set_mutually_exclusive(true);
                parser->mutually_exclusive_groups.push_back(name);
                // Register the argument in the parser
                parser->arguments[name] = {};
                return arg;
            }
        };

        MutuallyExclusiveGroup add_mutually_exclusive_group(bool required = false)
        {
            return MutuallyExclusiveGroup(this);
        }

        Argument add_argument(const std::string &name)
        {
            return Argument(this, name);
        }

        void parse_args(int argc, char **argv)
        {
            // Simple argument parsing
            for (int i = 1; i < argc; i++)
            {
                std::string arg = argv[i];
                if (arg.substr(0, 2) == "--")
                {
                    std::string key = arg.substr(2);
                    std::vector<std::string> values;

                    // Collect values until next option or end
                    int j = i + 1;
                    while (j < argc && argv[j][0] != '-')
                    {
                        values.push_back(argv[j]);
                        j++;
                    }

                    if (values.empty())
                    {
                        values.push_back("true"); // Flag argument
                    }

                    arguments[key] = values;
                    used_args[key] = true;
                    i = j - 1;
                }
                else
                {
                    positional_args.push_back(arg);
                }
            }

            // Handle help flag
            if (arguments.find("help") != arguments.end())
            {
                throw std::runtime_error("Help requested");
            }
        }

        bool is_used(const std::string &name) const
        {
            auto it = used_args.find(name);
            return it != used_args.end() && it->second;
        }

        template <typename T>
        T get(const std::string &name) const
        {
            auto it = arguments.find(name);
            if (it != arguments.end() && !it->second.empty())
            {
                if constexpr (std::is_same_v<T, std::string>)
                {
                    return it->second[0];
                }
                else if constexpr (std::is_same_v<T, std::vector<std::string>>)
                {
                    return it->second;
                }
            }

            auto default_it = defaults.find(name);
            if (default_it != defaults.end())
            {
                if constexpr (std::is_same_v<T, std::string>)
                {
                    return default_it->second;
                }
                else if constexpr (std::is_same_v<T, std::vector<std::string>>)
                {
                    return {default_it->second};
                }
            }

            if constexpr (std::is_same_v<T, std::string>)
            {
                return "";
            }
            else if constexpr (std::is_same_v<T, std::vector<std::string>>)
            {
                return {};
            }
        }

        friend std::ostream &operator<<(std::ostream &os, const ArgumentParser &parser)
        {
            os << "Usage: " << parser.program_name << " [options]\n\n";
            if (!parser.description.empty())
            {
                os << parser.description << "\n\n";
            }
            os << "Options:\n";
            os << "  --help    Show this help message\n";
            return os;
        }
    };

} // namespace argparse
