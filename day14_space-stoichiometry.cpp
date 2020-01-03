#include <iostream>
#include <cassert>
#include <algorithm>
#include "utils.cpp"

using Chemical = string;
using Quantity = unsigned long;

using ReagentMap = unordered_map<Chemical, Quantity>;

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

Reaction scaleReaction(const ReactionOutputsMap &reactionOutputsMap, Reagent reagent) {
  assert(reagent.quantity > 0);
  
  vector<Reagent> scalledInputs;
  Reagent scalledOutputs = reagent;

  const auto reagentRecipe = reactionOutputsMap.at(reagent.chemical);
  const auto roundUnit = reagent.quantity % reagentRecipe.output.quantity == 0 ? 0 : 1;
  const auto scaleFactor = reagent.quantity / reagentRecipe.output.quantity + roundUnit;

  for(const auto input : reagentRecipe.inputs) {
    Reagent scaledInput = input;
    if(reagent.quantity > reagentRecipe.output.quantity) {
      scaledInput.quantity *= scaleFactor;
    }
    scalledInputs.push_back(scaledInput);
  }

  scalledOutputs.quantity = reagentRecipe.output.quantity * scaleFactor;

  return {scalledInputs, scalledOutputs};
}

pair<ReagentMap, ReagentMap> expandReaction(
  const ReactionOutputsMap &reactionOutputsMap,
  Reagent desiredReagent,
  ReagentMap waste = {}
) {
  if(desiredReagent.chemical == "ORE" || desiredReagent.quantity == 0) {
    return {
      {{desiredReagent.chemical, desiredReagent.quantity}},
      waste
    };
  }

  ReagentMap expanded;
  auto currentReaction = scaleReaction(reactionOutputsMap, desiredReagent);

  if(currentReaction.output.quantity > desiredReagent.quantity) {
    waste[desiredReagent.chemical] += currentReaction.output.quantity - desiredReagent.quantity;
  }

  for(auto &input : currentReaction.inputs) {
    int toProduce = input.quantity - waste[input.chemical];
    if(toProduce > 0) {
      input.quantity = toProduce;
      waste[input.chemical] = 0;
      const auto [e, w] = expandReaction(reactionOutputsMap, input, waste);
      for(const auto r : e) {
        expanded[r.first] += r.second;
      }
      for(const auto r : w) {
        waste[r.first] = r.second;
      }
    }
    else {
      input.quantity = 0;
      waste[input.chemical] = -toProduce;
    }
  }

  return {expanded, waste};
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

  const auto sevenADetails = scaleReaction(testReactions1, {"A", 7});
  assert(sevenADetails.inputs.size() == 1);
  assert(sevenADetails.inputs.front().chemical == "ORE");
  assert(sevenADetails.inputs.front().quantity == 10);
  assert(sevenADetails.output.quantity == 10);

  const auto twelveADetails = scaleReaction(testReactions1, {"A", 12});
  assert(twelveADetails.inputs.size() == 1);
  assert(twelveADetails.inputs.front().chemical == "ORE");
  assert(twelveADetails.inputs.front().quantity == 20);
  assert(twelveADetails.output.quantity == 20);

  const auto oneCDetails = scaleReaction(testReactions1, {"C", 1});
  assert(oneCDetails.inputs.size() == 2);
  assert(oneCDetails.inputs.front().chemical == "A");
  assert(oneCDetails.inputs.front().quantity == 7);
  assert(oneCDetails.inputs.back().chemical == "B");
  assert(oneCDetails.inputs.back().quantity == 1);
  assert(oneCDetails.output.quantity == 1);

  assert(expandReaction(testReactions1, {"C", 1}).first.at("ORE") = 11);
  assert(expandReaction(testReactions1, {"C", 1}, {{"A", 7}}).first.at("ORE") = 1);
  assert(expandReaction(testReactions1, {"FUEL", 1}).first.at("ORE") == 31);

  assert(expandReaction(parseReactionList(testNanofactory2), {"FUEL", 1}).first.at("ORE") == 165);
  assert(expandReaction(parseReactionList(testNanofactory3), {"FUEL", 1}).first.at("ORE") == 13312);
  assert(expandReaction(parseReactionList(testNanofactory4), {"FUEL", 1}).first.at("ORE") == 180697);
  assert(expandReaction(parseReactionList(testNanofactory5), {"FUEL", 1}).first.at("ORE") == 2210736);

  const auto input = parseReactionList(getPuzzleInput("inputs/aoc_day14_1.txt"));
  cout << "Part1, FUEL required : " << expandReaction(input, {"FUEL", 1}).first.at("ORE") << "\n";
  
  return 0;
}
