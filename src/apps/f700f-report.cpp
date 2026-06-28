#include <f700f/report_artifact_loader.hpp>

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

namespace {

void print_help(std::ostream &out) {
  out << "Usage: f700f-report --input <artifact.json|artifact.csv> [--output <report.md>]\n"
      << "\n"
      << "Loads a sweep aggregate artifact and writes an M2 baseline comparison "
         "Markdown report.\n";
}

} // namespace

int main(int argc, char **argv) {
  std::string input_path;
  std::string output_path;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      print_help(std::cout);
      return 0;
    }
    if (arg == "--input" || arg == "-i") {
      if (i + 1 >= argc) {
        std::cerr << "error: --input requires a path\n";
        return 2;
      }
      input_path = argv[++i];
      continue;
    }
    if (arg == "--output" || arg == "-o") {
      if (i + 1 >= argc) {
        std::cerr << "error: --output requires a path\n";
        return 2;
      }
      output_path = argv[++i];
      continue;
    }
    std::cerr << "error: unknown argument: " << arg << "\n";
    print_help(std::cerr);
    return 2;
  }

  if (input_path.empty()) {
    std::cerr << "error: missing required --input <artifact.json|artifact.csv>\n";
    return 2;
  }

  try {
    const auto loaded = f700f::reporting::load_report_input_file(input_path);
    const auto markdown = f700f::reporting::render_report_from_loaded_input(loaded);
    if (output_path.empty()) {
      std::cout << markdown;
    } else {
      std::ofstream out(output_path);
      if (!out) {
        std::cerr << "error: unable to write output file: " << output_path << "\n";
        return 1;
      }
      out << markdown;
    }
  } catch (const std::exception &ex) {
    std::cerr << "error: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
