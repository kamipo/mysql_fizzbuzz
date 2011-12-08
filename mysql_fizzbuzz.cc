extern "C" {
#include <mysql/mysql.h>
}
#include <string>
#include <sstream>

extern "C" {
my_bool fizzbuzz_init(UDF_INIT* initid, UDF_ARGS* args, char* message);
void fizzbuzz_deinit(UDF_INIT* initid);
char* fizzbuzz(UDF_INIT* initid, UDF_ARGS* args, char* result, unsigned long* length, char* is_null, char* error);
}

my_bool fizzbuzz_init(UDF_INIT* initid, UDF_ARGS* args, char* message)
{
  if (args->arg_count != 1) {
    strcpy(message, "fizzbuzz: require only one argument");
    return 1;
  }
  if (args->arg_type[0] != INT_RESULT || *args->args[0] < 0) {
    strcpy(message, "fizzbuzz: argument should be a unsigned integer");
    return 1;
  }

  initid->ptr = (char*)(void*)new std::string();
  initid->const_item = 1;
  return 0;
}

void fizzbuzz_deinit(UDF_INIT* initid)
{
  delete (std::string*)(void*)initid->ptr;
}

char* fizzbuzz(UDF_INIT* initid, UDF_ARGS* args, char* result, unsigned long* length, char* is_null, char* error)
{
  unsigned long n = *args->args[0];
  std::string* out = (std::string*)(void*)initid->ptr;
  std::ostringstream ss;

  for (unsigned i = 1; i <= n; ++i) {
    if (i % 15 == 0) {
      ss << "FizzBuzz";
    } else if (i % 3 == 0) {
      ss << "Fizz";
    } else if (i % 5 == 0) {
      ss << "Buzz";
    } else {
      ss << i;
    }
    ss << std::endl;
  }

  *out = ss.str();
  *length = out->size();

  if (*length > 0) {
    return &(*out)[0];
  }

  *is_null = 1;
  return NULL;
}
