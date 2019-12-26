#include <iostream>
#include <cassert>
#include <algorithm>
#include "utils.cpp"

using Chemical = string;
using Quantity = unsigned long;

using ReagantMap = unordered_map<Chemical, Quantity>;

struct Reagent 
{ 
  Chemical chemical {"MISSING"};
  Quantity quantity {0};
};

struct Reaction
{
  vector<Reagent> inputs {};
  Reagent output {"", 0};
};

using ReactionOutputsMap = unordered_map<Chemical, Reaction>;

ostream &operator<<(ostream &os, const Reagent &r)
{
  os << r.quantity << " " << r.chemical;
  return os;
}

ReactionOutputsMap parseReactionList(const vector<string> list) {
  ReactionOutputsMap map;
  for(const auto entry : list) {
    Reaction reaction;
    const auto [inputs, output] = cut(entry, " => ");
    const auto splittedInputs = split(inputs, ", ");
    for(const auto input : splittedInputs) {
      const auto [inQuantity, inChemical] = cut(input, " ");
      reaction.inputs.push_back({inChemical, stoul(inQuantity)});
    }
    const auto [outQuantity, outChemical] = cut(output, " ");
    reaction.output = {outChemical, stoul(outQuantity)};
    map.insert({outChemical, reaction});
  }
  return map;
}

vector<Reagent> scaleInputs(const ReactionOutputsMap &reactionOutputsMap, Reagent reagent) {
  const auto reagentRecipe = reactionOutputsMap.at(reagent.chemical);
  vector<Reagent> scalledInputs;
  for(const auto input : reagentRecipe.inputs) {
    Reagent scaledInput = input;
    if(reagent.quantity > reagentRecipe.output.quantity) {
      const auto roundedUnit = reagent.quantity % reagentRecipe.output.quantity == 0 ? 0 : 1;
      scaledInput.quantity *= reagent.quantity / reagentRecipe.output.quantity + roundedUnit;
    }
    scalledInputs.push_back(scaledInput);
  }
  return scalledInputs;
}

ReagantMap mergeReagant(const vector<Reagent> &reagents) {
  ReagantMap mergedReagents;
  for(const auto r : reagents) {
    const auto entry = mergedReagents.find(r.chemical);
    if(entry == mergedReagents.cend()) {
      mergedReagents.insert({r.chemical, r.quantity});
    } else {
      mergedReagents.at(r.chemical) += r.quantity;
    }
  }
  return mergedReagents;
}

vector<Reagent> reagentMapToVector(const ReagantMap &map) {
  vector<Reagent> vector;
  for(const auto [chemical, quantity] : map) {
    vector.push_back({chemical, quantity});
  }
  return vector;
}

vector<Reagent> expandInputs(const ReactionOutputsMap &reactionOutputsMap, Reagent reagent) {
  const auto scaledInputs = scaleInputs(reactionOutputsMap, reagent);
  if(scaledInputs.size() == 1 /* && scaledInputs.front().chemical == "ORE" */) {
    return {reagent};
  }

  vector<Reagent> expanded;
  for(const auto r : scaledInputs) {
    assert(r.chemical != "ORE");
    const auto recurse = expandInputs(reactionOutputsMap, r);
    expanded.insert(expanded.cend(), recurse.cbegin(), recurse.cend());
  }

  return expanded;
}

size_t oreLookup(const ReactionOutputsMap &reactionOutputsMap, const ReagantMap &map) {
  size_t oreCount = 0;
  for(const auto [chemical, quantity] : map) {
    Reagent currentReagent = {chemical, quantity};
    while (currentReagent.chemical != "ORE")
    {
      currentReagent = scaleInputs(reactionOutputsMap, currentReagent).front();
    }
    oreCount += currentReagent.quantity;
  }
  return oreCount;
}

const vector<string> testNanofactory1 = {
  "10 ORE => 10 A",
  "1 ORE => 1 B",
  "7 A, 1 B => 1 C",
  "7 A, 1 C => 1 D",
  "7 A, 1 D => 1 E",
  "7 A, 1 E => 1 FUEL",
};

const vector<string> testNanofactory2 = {
  "9 ORE => 2 A",
  "8 ORE => 3 B",
  "7 ORE => 5 C",
  "3 A, 4 B => 1 AB",
  "5 B, 7 C => 1 BC",
  "4 C, 1 A => 1 CA",
  "2 AB, 3 BC, 4 CA => 1 FUEL"
};

const vector<string> testNanofactory3 = {
  "157 ORE => 5 NZVS",
  "165 ORE => 6 DCFZ",
  "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL",
  "12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ",
  "179 ORE => 7 PSHF",
  "177 ORE => 5 HKGWZ",
  "7 DCFZ, 7 PSHF => 2 XJWVT",
  "165 ORE => 2 GPVTF",
  "3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT",
};

const vector<string> testNanofactory4 = {
  "2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG",
  "17 NVRVD, 3 JNWZP => 8 VPVL",
  "53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL",
  "22 VJHF, 37 MNCFX => 5 FWMGM",
  "139 ORE => 4 NVRVD",
  "144 ORE => 7 JNWZP",
  "5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC",
  "5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV",
  "145 ORE => 6 MNCFX",
  "1 NVRVD => 8 CXFTF",
  "1 VJHF, 6 MNCFX => 4 RFSQX",
  "176 ORE => 6 VJHF"
};

const vector<string> testNanofactory5 = {
  "171 ORE => 8 CNZTR",
  "7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL",
  "114 ORE => 4 BHXH",
  "14 VRPVC => 6 BMBT",
  "6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL",
  "6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT",
  "15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW",
  "13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW",
  "5 BMBT => 4 WPTQ",
  "189 ORE => 9 KTJDG",
  "1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP",
  "12 VRPVC, 27 CNZTR => 2 XDBXC",
  "15 KTJDG, 12 BHXH => 5 XCVML",
  "3 BHXH, 2 VRPVC => 7 MZWV",
  "121 ORE => 7 VRPVC",
  "7 XCVML => 6 RJRHP",
  "5 BHXH, 4 VRPVC => 5 LTCX"
};

int main(int argc, char const *argv[])
{
  // Part 1 tests
  const auto testReactions1 = parseReactionList(testNanofactory1);
  assert(testReactions1.size() == 6);

  const auto sevenADetails = scaleInputs(testReactions1, {"A", 7});
  assert(sevenADetails.size() == 1);
  assert(sevenADetails.front().chemical == "ORE");
  assert(sevenADetails.front().quantity == 10);

  const auto twelveADetails = scaleInputs(testReactions1, {"A", 12});
  assert(twelveADetails.size() == 1);
  assert(twelveADetails.front().chemical == "ORE");
  assert(twelveADetails.front().quantity == 20);

  const auto oneCDetails = scaleInputs(testReactions1, {"C", 1});
  assert(oneCDetails.size() == 2);
  assert(oneCDetails.front().chemical == "A");
  assert(oneCDetails.front().quantity == 7);
  assert(oneCDetails.back().chemical == "B");
  assert(oneCDetails.back().quantity == 1);

  const auto expandOneC = mergeReagant(expandInputs(testReactions1, {"C", 1}));
  assert(expandOneC.size() == 2);
  assert(expandOneC.at("A") == 7);
  assert(expandOneC.at("B") == 1);

  const auto expandFuel = mergeReagant(expandInputs(testReactions1, {"FUEL", 1}));
  assert(expandFuel.size() == 2);
  assert(expandFuel.at("A") == 28);
  assert(expandFuel.at("B") == 1);
  assert(oreLookup(testReactions1, expandFuel) == 31);

  const auto testReactions2 = parseReactionList(testNanofactory2);
  const auto twoAB = scaleInputs(testReactions2, {"AB", 2});
  assert(twoAB.size() == 2);
  assert(twoAB.front().chemical == "A");
  assert(twoAB.front().quantity == 6);
  assert(twoAB.back().chemical == "B");
  assert(twoAB.back().quantity == 8);
  const auto t2ReagantMap =  mergeReagant(expandInputs(testReactions2, {"FUEL", 1}));
  assert(oreLookup(testReactions2, t2ReagantMap)  == 165);

  const auto testReactions3 = parseReactionList(testNanofactory3);
  const auto t3ReagantMap =  mergeReagant(expandInputs(testReactions3, {"FUEL", 1}));
  assert(oreLookup(testReactions3, t3ReagantMap) == 13312);

  const auto testReactions4 = parseReactionList(testNanofactory4);
  const auto t4ReagantMap =  mergeReagant(expandInputs(testReactions4, {"FUEL", 1}));
  assert(oreLookup(testReactions4, t4ReagantMap) == 180697);

  const auto testReactions5 = parseReactionList(testNanofactory5);
  const auto t5ReagantMap =  mergeReagant(expandInputs(testReactions5, {"FUEL", 1}));
  assert(oreLookup(testReactions5, t5ReagantMap) == 2210736);

  // Part 1
  const auto p1Input = getPuzzleInput("inputs/aoc_day14_1.txt");
  const auto p1Reactions = parseReactionList(p1Input);
  const auto p1ReagantMap =  mergeReagant(expandInputs(p1Reactions, {"FUEL", 1}));
  cout << "Part1, amount of ORE for one FUEL: " << oreLookup(p1Reactions, p1ReagantMap) << "\n";

  return 0;
}
