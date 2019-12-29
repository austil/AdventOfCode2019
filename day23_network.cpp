#include <iostream>
#include <cassert>
#include <optional>
#include "IntcodeComputer.cpp"

struct Packet {
  IntCode x {0};
  IntCode y {0};
};

ostream &operator<<(ostream &os, const Packet &p) {
  os << "(x=" << p.x << ", y=" << p.y << ")";
  return os;
}

int main(int argc, char const *argv[])
{
  // Part 1
  const auto nicProgram = parseIntcode(getPuzzleInput("inputs/aoc_day23_1.txt").front());
  ProgramState computers[50];
  for (int ip = 0; ip < 50; ip++){
    const queue<IntCode> networkAdress({ip});
    computers[ip] = runProgram(nicProgram, networkAdress);
  }

  queue<Packet> packetQueues[50];
  optional<Packet> lastNATPacket;
  size_t cycleCount = 0;
  while(true) {
    bool allIdle = true;

    // Collect packets
    size_t packetSent = 0;
    for (size_t ip = 0; ip < 50; ip++){
      auto &currentComputer = computers[ip];
      for (size_t n = 0; n < currentComputer.outputs.size(); n+=3) {
        const auto address = currentComputer.outputs.at(n);
        const Packet p {currentComputer.outputs.at(n+1), currentComputer.outputs.at(n+2)};
        if(address == 255) {
          lastNATPacket = p;
          // cerr << ip << " sent a NAT packet " << p << "\n";
        }
        else if(address < 50) {
          allIdle = false;
          packetQueues[address].push(p);
          // cout << ip << " sent a packet" << p << " to " << address << "\n";
        }
        else {
          cerr << ip << " tried to sent a packet " << p << " to an invalid adress " << address << "\n";
          throw;
        }
        packetSent++;
      }
      currentComputer.outputs.clear();
    }
    // Distribute packets and run
    size_t packetReceived = 0;
    for (size_t ip = 0; ip < 50; ip++){
      auto &currentComputer = computers[ip];
      auto &currentPacketQueue = packetQueues[ip];
      if(currentPacketQueue.empty()) {
        currentComputer.inputs.push(-1);
      }
      else {
        allIdle = false;
        while(!currentPacketQueue.empty()) {
          currentComputer.inputs.push(currentPacketQueue.front().x);
          currentComputer.inputs.push(currentPacketQueue.front().y);
          currentPacketQueue.pop();
          packetReceived++;
        }
      }

      computers[ip] = runProgram(currentComputer);
    }

    if(allIdle && lastNATPacket.has_value()) {
      packetQueues[0].push(lastNATPacket.value());
      cout << "The network is idle, sending last NAT packet " << lastNATPacket.value() << " to 0\n";
    }
    cycleCount++;
    cout << "Cycle " << cycleCount << ", sent " << packetSent << ", received " << packetReceived << "\n";
  }
  
  return 0;
}

