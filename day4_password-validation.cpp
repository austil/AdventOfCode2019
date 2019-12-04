#include <iostream>
#include <cassert>
#include <cmath>
#include "utils.cpp"

bool isValidPassword(string password) {
  const bool isSixDigit = password.size() == 6;
  bool twoIdenticalAdjacentDigit = false;
  bool neverDecrease = true;
  for (int i = 0; i < 6; i++)
  {
    const int digit = password.at(i) - '0';
    if(i > 0) {
      const int previousDigit = password.at(i-1) - '0';
      if(previousDigit == digit) {
        twoIdenticalAdjacentDigit = true;
      }
      if(previousDigit > digit) {
        neverDecrease = false;
      }
    }
  }
  return isSixDigit && twoIdenticalAdjacentDigit && neverDecrease;
}

bool isValidPasswordFixed(string password) {
  const bool isSixDigit = password.size() == 6;
  bool twoIdenticalAdjacentDigit = false;
  bool neverDecrease = true;
  int identicalAdjacentCounter = 1;
  for (int i = 0; i < 6; i++)
  {
    const int digit = password.at(i) - '0';
    if(i > 0) {
      const int previousDigit = password.at(i-1) - '0';
      if(previousDigit == digit) {
        identicalAdjacentCounter++;
        if(i < 5) {
          const int nextDigit = password.at(i+1) - '0';
          if(identicalAdjacentCounter == 2 && digit != nextDigit) {
            twoIdenticalAdjacentDigit = true;
          }
        } else if (identicalAdjacentCounter == 2) {
          twoIdenticalAdjacentDigit = true;
        }
      } else {
        identicalAdjacentCounter = 1;
      }
      if(previousDigit > digit) {
        neverDecrease = false;
      }
    }
  }
  return isSixDigit && twoIdenticalAdjacentDigit && neverDecrease;
}

int main(int argc, char const *argv[])
{
  // Part 1
  assert(isValidPassword("111111") == true);
  assert(isValidPassword("122345") == true);
  assert(isValidPassword("111123") == true);
  assert(isValidPassword("135679") == false);
  assert(isValidPassword("223450") == false);
  assert(isValidPassword("123789") == false);

  vector<int> validPassword;
  for (int i = 264360; i < 746325; i++)
  {
    if(isValidPassword(to_string(i))) {
      validPassword.push_back(i);
    }
  }
  cout << "Part 1, valid password in range: " << validPassword.size() << "\n";
  
  // Part 2
  assert(isValidPasswordFixed("112233") == true);
  assert(isValidPasswordFixed("123444") == false);
  assert(isValidPasswordFixed("111122") == true);

  vector<int> validPasswordFixed;
  for (int i = 264360; i < 746325; i++)
  {
    if(isValidPasswordFixed(to_string(i))) {
      validPasswordFixed.push_back(i);
    }
  }
  cout << "Part 2, valid password in range: " << validPasswordFixed.size() << "\n";

  return 0;
}

