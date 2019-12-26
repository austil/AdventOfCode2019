#include <iostream>
#include <cassert>
#include "utils.cpp"

using Card = unsigned long long;
using CardPosition = Card;
using Deck = vector<Card>;

Deck getSpaceCardDeck(Card l = 10007) {
  Deck spaceCard;
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

// Quicker solution

CardPosition dealIntoNewStackIdx(CardPosition deckLength, CardPosition cardIndex) {
  return deckLength - cardIndex - 1;
}

CardPosition cutCardIdx(CardPosition deckLength, int cutN, CardPosition cardIndex) {
  const auto cutIndex = cutN > 0 ? cutN : deckLength + cutN;
  return cardIndex < cutIndex 
    ? cardIndex + deckLength - cutIndex
    : cardIndex - cutIndex;
}

CardPosition dealWithIncrementIdx(CardPosition deckLength, int incr, CardPosition cardIndex) {
  return cardIndex * incr % deckLength;
}

CardPosition suffleIdx(const vector<string> &steps, CardPosition deckLength, CardPosition cardIndex) {
  CardPosition currentPosition = cardIndex;
  for(const auto step : steps) {
    const auto words = split(step, " ");
    if(words.size() == 2 && words.at(0) == "cut") {
      const int n = stoi(words.at(1));
      currentPosition = cutCardIdx(deckLength, n, currentPosition);
    }
    else if (words.size() == 4 && words.at(1) == "with") {
      const int n = stoi(words.at(3));
      currentPosition = dealWithIncrementIdx(deckLength, n, currentPosition);
    }
    else if (words.size() == 4 && words.at(1) == "into") {
      currentPosition = dealIntoNewStackIdx(deckLength, currentPosition);
    } else {
      cerr << "Unsuported step: " << step << "\n";
      throw;
    }
  }
  return currentPosition;
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
  if(thisYear == p1Suffled.cend()) {
    cout << "Part1, 2019 card not found...\n";
  }
  else {
    cout << "Part1, 2019 card position: " << distance(p1Suffled.cbegin(), thisYear) << "\n";
  }

  // Clever solution
  assert(dealIntoNewStackIdx(10, 3) == 6);
  assert(cutCardIdx(10, 3, 3) == 0);
  assert(cutCardIdx(10, 3, 2) == 9);
  assert(cutCardIdx(10, 3, 0) == 7);
  assert(cutCardIdx(10, 3, 9) == 6);
  assert(cutCardIdx(10, -4, 6) == 0);
  assert(cutCardIdx(10, -4, 5) == 9);
  assert(cutCardIdx(10, -4, 0) == 4);
  assert(dealWithIncrementIdx(10, 3, 0) == 0);
  assert(dealWithIncrementIdx(10, 3, 7) == 1);
  assert(dealWithIncrementIdx(10, 3, 4) == 2);
  assert(dealWithIncrementIdx(10, 3, 1) == 3);

  assert(suffleIdx(p1SuffleSteps, 10007, 2019) == 8191);
  
  // Part 2
  const unsigned long long suffleCount = 101741582076661;
  const Card hugeDeckLength = 119315717514047;
  // On ne peut pas boucler autant de fois
  // On ne peut pas mélanger un tel jeu, ne serait ce qu'une seule fois

  // Par contre le suffle appliqué une fois à lui même nous donne un double suffle
  // le double suffle appliqué à lui même nous donne un quadruple suffle
  // le quadruple fois 2 => fois 8
  // etc.
  // ainsi on arrive en 47 opérations à plus des 101 trillions de suffles répétitifs attendus !

  // Aussi pour savoir ce qui se trouve à l'emplacement 2020 un fois le suffle appliqué revient à
  // demander ou se trouve la carte 2020 à la suite du suffle inverse !
  
  return 0;
}
