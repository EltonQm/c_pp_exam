#include "task3_utils.hpp"
#include "argparse/include/argparse/argparse.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
  argparse::ArgumentParser program("GOoboParserApp");

  program.add_argument("--obsolete-stats")
      .help("Show obsolete stats for the file(s)")
      .nargs(argparse::nargs_pattern::any);

  program.add_argument("--consider-table")
      .help("Write consider table to output .tab file")
      .nargs(2);

  try
  {
    program.parse_args(argc, argv);
  }
  catch (const std::exception &err)
  {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  GOoboParserApp app;

  if (program.is_used("--obsolete-stats"))
  {
    auto args = program.get<std::vector<std::string>>("--obsolete-stats");
    if (args.size() == 1)
    {
      app.runObsoleteStats({args[0]});
    }
    else if (args.size() == 2)
    {
      app.runObsoleteStats({args[0]}, args[1]);
    }
    else
    {
      std::cerr << "Usage: --obsolete-stats <input.obo> [output.tab]"
                << std::endl;
      return 1;
    }
  }

  if (program.is_used("--consider-table"))
  {
    auto args = program.get<std::vector<std::string>>("--consider-table");
    app.writeConsiderTable({args[0]}, args[1]);
  }

  return 0;
}
