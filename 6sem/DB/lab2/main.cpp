#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <boost/program_options.hpp>

#include "area_container.h"

namespace po = boost::program_options;
using namespace std;

po::options_description desc("Allowed options");

void print_help_and_exit()
{
  cerr << desc << endl;
  exit(EXIT_FAILURE);
}

template<class Tk, class Tf>
AreaContainer<Tk, Tf> *load_sample(const string filename);

template<class Tk, class Tf>
void save_sample(const AreaContainer<Tk, Tf> *ac, const string filename);

#define SORT_RECORDS_COUNT 800000
#define UNSORT_RECORDS_COUNT 200000

#define AREA_SIZE 40

#if TYPE == 0
#define KEY_TYPE string
#else
#define KEY_TYPE long
#endif

template<class Tk, class Tf>
void save_sample(const AreaContainer<Tk, Tf> &ac);

int main(int argc, char **argv)
{
  desc.add_options()
    ("help,h",                                      "produce this help message")

    ("bin-source,b",         po::value<string>(),   "load binary base")
    ("text-source-base,t",   po::value<string>(),   "load text base (base part)")
    ("text-source-tail,r",   po::value<string>(),   "load text base (tail part)")

    ("interspace-size,s",    po::value<unsigned>(), "interspace size")

    ("find,f",               po::value<string>(),   "find sigle record")
    ("dump",                 po::value<string>(),   "dump base to binary file");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm);

  if (vm.count("help"))
    print_help_and_exit();

  if ((vm.count("bin-source") && (vm.count("text-source-base") || vm.count("text-source-tail"))) ||
      !(vm.count("bin-source") || !(vm.count("text-source") && vm.count("text-source-tail"))))
    print_help_and_exit();

  AreaContainer<KEY_TYPE, string> *ac = nullptr;
  if (vm.count("bin-source")) {
    cerr << "bin-source: " << vm["bin-source"].as<string>() << endl;
    ac = load_sample<KEY_TYPE, string>(vm["bin-source"].as<string>());
    cerr << "Loaded." << endl;
  } else {
    if (!vm.count("interspace-size"))
      print_help_and_exit();

    int interspace_length = vm["interspace-size"].as<unsigned>();
    int effective_interspace_length = round(0.9 * interspace_length);
    ifstream in(vm["text-source-base"].as<string>().c_str());
    if (!in.is_open())
    {
      cerr << "Can't open file " << vm["text-source-base"].as<string>() << endl;
      return 1;
    }

    char key[7];
    char val1[13];
    long val2;

    Item<KEY_TYPE, string> *pit = new Item<KEY_TYPE, string>;
    vector< Item<KEY_TYPE, string> >* items = new vector< Item<KEY_TYPE, string> >;
    vector< Interspace<KEY_TYPE, string> *>* interspaces = new vector< Interspace<KEY_TYPE, string> *>;
    list< Area<KEY_TYPE, string> >* areas = new list< Area<KEY_TYPE, string> >;
    int items_count = 0, interspaces_count = 0;

    for(int i = 0; i < SORT_RECORDS_COUNT; ++i) {
      in >> key >> val2 >> val1;

      #if TYPE == 0
        pit->key = val1;
      #else
        pit->key = val2;
      #endif
      pit->field = key;
      items->push_back(*pit);
      items_count++;

      if ( items_count == effective_interspace_length )
      {
        if ( interspaces_count % 10 == 9 )
        {
          interspaces->push_back(new Interspace<KEY_TYPE, string>(interspace_length));
          interspaces_count++;
        }

        if ( interspaces_count == AREA_SIZE )
        {
          areas->push_back(Area<KEY_TYPE, string>(AREA_SIZE, interspaces));
          interspaces = new vector< Interspace<KEY_TYPE, string> *>;
          interspaces_count = 0;
        }

        interspaces->push_back(new Interspace<KEY_TYPE, string>(interspace_length, items));
        interspaces_count++;
        items = new vector< Item<KEY_TYPE, string> >;
        items_count = 0;
      }
    }
    interspaces->push_back(new Interspace<KEY_TYPE, string>(interspace_length, items));
    areas->push_back(Area<KEY_TYPE, string>(AREA_SIZE, interspaces));
    ac = new AreaContainer<KEY_TYPE, string>(areas);
    in.close();
    cerr << "File " << vm["text-source-base"].as<string>() << " loaded." << endl;

    cerr << "Areas count before: " << ac->get_size() << endl;

    // TODO: move to params
    ofstream ofs("./before.json");
    ofs << ac->to_json();
    ofs.close();

    in.open(vm["text-source-tail"].as<string>().c_str());
    if (!in.is_open())
    {
      cerr << "Can't open file " << vm["text-source-tail"].as<string>() << endl;
      return 1;
    }

    for(int i = 0; i < UNSORT_RECORDS_COUNT; ++i) {
      in >> key >> val2 >> val1;

    #if TYPE == 0
      pit->key = val1;
    #else
      pit->key = val2;
    #endif
      pit->field = key;
      try {
        ac->add_item(*pit);
      } catch (invalid_argument* ex) {
        cerr << ex->what() << endl;
        throw;
      }
    }
    in.close();
    cerr << "File " << "base_tail.txt" << " loaded." << endl;
    cerr << "Areas count after: " << ac->get_size() << endl;
    delete pit;

    ofs.open("./after.json");
    ofs << ac->to_json();
    ofs.close();

  }

  ofstream ofs("./after.json");
  ofs << ac->to_json();
  ofs.close();

  if ( vm.count("find") )
  {
    vector< Item<KEY_TYPE, string> *> *res;
    #if TYPE == 0
      res = ac->find_item(vm["find"].as<string>().c_str());
    #else
      res = ac->find_item(atol(vm["find"].as<string>().c_str()));
    #endif
    if ( res == nullptr )
      cerr << "No records found" << endl;
    else
    {
      vector< Item<KEY_TYPE, string> *>::iterator it;
      for( it = res->begin(); it < res->end(); ++it )
      #if TYPE == 0
        printf("%s %s\n", (*it)->key.c_str(), (*it)->field.c_str());
      #else
        printf("%ld %s\n", (*it)->key, (*it)->field.c_str());
      #endif
    }
  }
  else if ( vm.count("dump") )
  {
    cerr << "Dumping into " << vm["dump"].as<string>() << endl;
    save_sample(ac, vm["dump"].as<string>());
    cerr << "Done" << endl;
  }

  return 0;
}

template<class Tk, class Tf>
AreaContainer<Tk, Tf> *load_sample(const string filename)
{
  AreaContainer<Tk, Tf> *ac;
  ifstream ifs(filename.c_str());
  boost::archive::binary_iarchive ia(ifs);
  ia.register_type< Item<Tk, Tf> >();
  ia.register_type< Interspace<Tk, Tf> >();
  ia.register_type< Area<Tk, Tf> >();
  ia.register_type< AreaContainer<Tk, Tf> >();
  ia >> ac;
  ifs.close();
  return ac;
}

template<class Tk, class Tf>
void save_sample(const AreaContainer<Tk, Tf> *ac, const string filename)
{
  ofstream ofs(filename.c_str());
  boost::archive::binary_oarchive oa(ofs);
  oa.register_type< Item<Tk, Tf> >();
  oa.register_type< Interspace<Tk, Tf> >();
  oa.register_type< Area<Tk, Tf> >();
  oa.register_type< AreaContainer<Tk, Tf> >();
  oa << ac;
  ofs.close();
}
