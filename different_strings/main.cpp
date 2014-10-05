#include <iostream>
#include "cow_string.hpp"

using namespace std;

int main(int argc, char **argv) 
{
  cow_string str1{"Test string."};
  cow_string str2 = str1;
  cow_string str3 = str2;
  
  cout << str1 << endl;
  cout << str2 << endl;
  str3.append('.');
  str3.append('.');
  cout << str3 << endl;
  cout << "Str1 and str2 share buffer: " << boolalpha << str1.share_buffer(str2) << endl;
  cout << "Input new cow_string: " << endl;
  cin >> str1;
  cout << "New str1 = " << str1 << endl;
  cout << "Str1 and str2 share buffer: " << boolalpha << str1.share_buffer(str2) << endl;
  return 0;
}
