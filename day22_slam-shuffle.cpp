#include <iostream>
#include <cassert>
#include "utils.cpp"

using Card = unsigned long long;
using Deck = vector<Card>;

Deck getSpaceCardDeck(Card l = 10007) {
  Deck spaceCard(l);
  for (Card i = 0; i < l; i++)
  {
    spaceCard.push_back(i);
  }
  return spaceCard;
}

Deck dealIntoNewStack(const Deck &deck) {
  const Deck nextDeck(deck.crbegin(), deck.crend());
  return nextDeck;
}

Deck cutCard(const Deck &deck, int n) {
  const auto cutIndex = n > 0 ? n : deck.size() + n;
  Deck nextDeck(deck.cbegin() + cutIndex, deck.cend());
  nextDeck.insert(nextDeck.cend(), deck.cbegin(), deck.cbegin() + cutIndex);
  return nextDeck;
}

Deck dealWithIncrement(const Deck &deck, int incr) {
  Deck nextDeck(deck.size());
  for (size_t i = 0; i < deck.size(); i++)
  {
    const size_t destIndex = (i * incr) % deck.size();
    nextDeck.at(destIndex) = deck.at(i);
  }
  return nextDeck;
}

Deck suffle(const vector<string> &steps, Deck startingDeck) {
  Deck nextDeck = startingDeck;
  for(const auto step : steps) {
    const auto words = split(step, " ");
    if(words.size() == 2 && words.at(0) == "cut") {
      const int n = stoi(words.at(1));
      nextDeck = cutCard(nextDeck, n);
    }
    else if (words.size() == 4 && words.at(1) == "with") {
      const int n = stoi(words.at(3));
      nextDeck = dealWithIncrement(nextDeck, n);
    }
    else if (words.size() == 4 && words.at(1) == "into") {
      nextDeck = dealIntoNewStack(nextDeck);
    } else {
      cerr << "Unsuported step: " << step << "\n";
      throw;
    }
  }
  return nextDeck;
}

struct SuffleTest {
  vector<string> steps {};
  Deck expectedDeck {};
};

const vector<SuffleTest> suffleTests = {
  {
    {
      "deal with increment 7",
      "deal into new stack",
      "deal into new stack"
    },
    {0,3,6,9,2,5,8,1,4,7}
  },
  {
    {
      "cut 6",
      "deal with increment 7",
      "deal into new stack"
    },
    {3,0,7,4,1,8,5,2,9,6}
  },
  {
    {
      "deal with increment 7",
      "deal with increment 9",
      "cut -2"
    },
    {6,3,0,7,4,1,8,5,2,9}
  },
  {
    {
      "deal into new stack",
      "cut -2",
      "deal with increment 7",
      "cut 8",
      "cut -4",
      "deal with increment 7",
      "cut 3",
      "deal with increment 9",
      "deal with increment 3",
      "cut -1"
    },
    {9,2,5,8,1,4,7,0,3,6}
  }
};

int main(int argc, char const *argv[])
{
  // Part 1
  const auto d = id<Deck>;
  const Deck start10 = {0,1,2,3,4,5,6,7,8,9};
  assert(dealIntoNewStack(start10) == d({9,8,7,6,5,4,3,2,1,0}));
  assert(cutCard(start10, 3) == d({3,4,5,6,7,8,9,0,1,2}));
  assert(cutCard(start10, -4) == d({6,7,8,9,0,1,2,3,4,5}));
  assert(dealWithIncrement(start10, 3) == d({0,7,4,1,8,5,2,9,6,3}));
  for(const auto test : suffleTests) {
    assert(suffle(test.steps, start10) == test.expectedDeck);
  }

  const auto p1SuffleSteps = getPuzzleInput("inputs/aoc_day22_1.txt");
  const auto p1Suffled = suffle(p1SuffleSteps, getSpaceCardDeck());
  const auto thisYear = find(p1Suffled.cbegin(), p1Suffled.cend(), 2019);
  cout << "Part1, 2019 deck position: " << distance(p1Suffled.cbegin(), thisYear) << "\n";

  // Part 2
  // Deck startingDeck = getSpaceCardDeck(119315717514047);
  // for (Card i = 0; i < 101741582076661; i++)
  // {
  //   startingDeck = suffle(p1SuffleSteps, startingDeck);
  // }
  
  return 0;
}
