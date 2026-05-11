#include "../testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  inf.readToken("[a-z]{1,1000}", "s");
  inf.readEoln();

  int q = inf.readInt(1, 100000, "q");
  inf.readEoln();

  for (int i = 0; i < q; i++) {
    inf.readInt(1, 1000000000, "k_i");
    inf.readSpace();
    inf.readInt(1, 1000000000, "x_i");
    inf.readEoln();
  }

  inf.readEof();

  return 0;
}
