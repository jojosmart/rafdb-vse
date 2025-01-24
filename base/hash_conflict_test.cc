// Author:Zhouxiaoliang

#include <string>

#include "base/yr.h"
#include "base/callback.h"
#include "base/hash.h"
#include "base/hash_tables.h"
#include "file/simple_line_reader.h"

DEFINE_string(urls_file, "" , "");

bool PushURL(const string& line, hash_set<uint32>* id_set, int* conflict) {
  uint32 docid = Fingerprint32(line);
  if (id_set->find(docid) != id_set->end()) {
    (*conflict)++;
  } else {
    id_set->insert(docid);
  }
  return true;
}

int main(int argc, char **argv) {
  AtExitManager exit_manager;
  ParseCommandLineFlags(&argc, &argv, true);

  hash_set<uint32> id_set;
  int conflict = 0;
  file::SimpleLineReader reader(FLAGS_urls_file, true);
  ResultCallback1<bool, const string&>* call_back =
      NewPermanentCallback(&PushURL, &id_set, &conflict);
  reader.ProcessLines(call_back);
  delete call_back;

  printf("%d\n", conflict);

  return 0;
}
