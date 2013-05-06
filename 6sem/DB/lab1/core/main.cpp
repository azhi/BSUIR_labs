#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "digit_analyze_hash_table.h"
#include "shift_hash_table.h"

namespace po = boost::program_options;
using namespace std;

po::options_description desc("Allowed options");

void print_help_and_exit()
{
  cerr << desc << endl;
  exit(EXIT_FAILURE);
}

void batch(VirtualHashTable *ht, const string &in_filename, const string &type);
void load_text(VirtualHashTable *ht, const string &in_filename);
void find_mode(VirtualHashTable *ht, const string &k);
void interactive_mode(VirtualHashTable *ht);

#define RECORDS_COUNT 1000000
#define SPACE_COUNT 1200000
#define KEYS_COUNT 1000200

int main(int argc, char const *argv[])
{
  // Declare the supported options.
  desc.add_options()
    ("help,h",                                 "produce this help message")

    ("bin-source,b",    po::value<string>(),   "load binary base")
    ("text-source,t",   po::value<string>(),   "load text base")

    ("type,k",          po::value<string>(),   "select table type")
    ("package-count,p", po::value<unsigned>(), "package count")

    ("batch,i",         po::value<string>(),   "run batch mode")
    ("find,f",          po::value<string>(),   "find sigle record")
    ("interactive",                            "run interactive mode")
    ("dump",            po::value<string>(),   "dump base to binary file");


  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm);

  if (vm.count("help"))
    print_help_and_exit();

  if ((vm.count("bin-source") && vm.count("text-source")) ||
      !(vm.count("bin-source") || vm.count("text-source")))
    print_help_and_exit();

  VirtualHashTable *ht = nullptr;

  if (vm.count("bin-source")) {
    cerr << "bin-source: " << vm["bin-source"].as<string>() << endl;
    ifstream ifs(vm["bin-source"].as<string>());

    boost::archive::binary_iarchive ia(ifs);
    ia >> ht;

    ifs.close();
    cerr << "Loaded." << endl;
  } else {
    if (!vm.count("type") || !vm.count("package-count"))
      print_help_and_exit();

    unsigned package_count = vm["package-count"].as<unsigned>();

    if (vm["type"].as<string>() == string("shift")) {
      cerr << "text-source(shift): "
           << vm["text-source"].as<string>() << endl;
      ht = new ShiftHashTable(package_count, SPACE_COUNT / package_count);
    } else if (vm["type"].as<string>() == string("analyze")) {
      cerr << "text-source(analyze): "
           << vm["text-source"].as<string>() << endl;
      ht = new DigitAnalyzeHashTable(package_count,
                                     SPACE_COUNT / package_count);
    } else
      print_help_and_exit();

    load_text(ht, vm["text-source"].as<string>());
  }

  if (vm.count("batch")) {
    cerr << "Batch: " << vm["batch"].as<string>() << endl;
    batch(ht, vm["batch"].as<string>(), vm["type"].as<string>());
  } else if (vm.count("dump")) {
    cerr << "Dumping to " << vm["dump"].as<string>() << endl;
    ofstream ofs(vm["dump"].as<string>());
    ht->dump(ofs);
    ofs.close();
    cerr << "Done." << endl;
  } else if (vm.count("find")) {
    find_mode(ht, vm["find"].as<string>());
  } else if (vm.count("interactive")) {
    interactive_mode(ht);
  } else
    print_help_and_exit();

  if (ht)
    delete ht;
  return 0;
}

void find_mode(VirtualHashTable *ht, const string &k)
{
  Item *pit = nullptr;
  if ( !VirtualHashTable::is_key(const_cast<char *>(k.c_str())) )
    cerr << "It's not a valid key." << endl;
  else
  {
    pit = ht->find_record(const_cast<char *>(k.c_str()));

    if ( pit == nullptr )
      cerr << "No record found." << endl;
    else
      cout << pit->key << " "
        << pit->field1 << " "
        << pit->field2 << endl;
  }
}

void interactive_mode(VirtualHashTable *ht)
{
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  char k[256];
  Item *pit = nullptr;
  cout << "Welcome to interactive mode!\nType some key to find or 'q' to exit" << endl;
  cout << "Key: ";
  cin >> k;
  while ( strcmp(k, "q") != 0 )
  {
    if ( !VirtualHashTable::is_key(k) )
      cout << "It's not a valid key." << endl;
    else
    {
      start = std::chrono::high_resolution_clock::now();
      pit = ht->find_record(k);
      end = std::chrono::high_resolution_clock::now();

      ull elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>
        (end-start).count();
      cout << "Response in " << elapsed_microseconds / 1000.0f << "ms." << endl;
      if ( pit == nullptr )
        cout << "No record found." << endl;
      else
        cout << "Record: " << pit->key << " "
             << pit->field1 << " "
             << pit->field2 << endl;
    }
    cout << "Key: ";
    cin >> k;
  }
}

void load_text(VirtualHashTable *ht, const string &in_filename)
{
  ifstream in(in_filename);
  char key[7];
  char val1[13];
  long val2;
  Item *pit = new Item;

  for(int i = 0; i < RECORDS_COUNT; ++i) {
    in >> key >> val2 >> val1;

    strcpy(pit->key, key);
    strcpy(pit->field1, val1);
    pit->field2 = val2;

    ht->add_record(*pit);
  }
  delete pit;
  in.close();

  cerr << "File " << in_filename << " loaded." << endl;
}

void batch(VirtualHashTable *ht, const string &in_filename, const string &type)
{
  ifstream in(in_filename);
  char key[7];

  clock_t t1 = clock();
  for(int i = 0; i < KEYS_COUNT; ++i) {
    in >> key;
    ht->find_record(key);
  }
  clock_t t2 = clock();
  in.close();

  cout << ht->package_count << " "
       << type << " "
       << ht->count_in_packages / (double) SPACE_COUNT << " "
       << ht->count_in_overflow / (double) ht->count_in_packages << " "
       << (t2 - t1) / (double) CLOCKS_PER_SEC << endl;
}
