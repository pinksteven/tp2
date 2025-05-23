#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
//cklion is stupid
// C macro using compiler flags to clear terminal
void clear() {
#if defined(_WIN32) || defined(_WIN64)
  std::system("cls");
#elif defined(__linux__) || defined(__unix__)
  std::system("clear");
#endif
};

template <typename T>
std::queue<T> merge_queues(std::queue<T> q1, std::queue<T> q2) {
  for (; !q2.empty(); q2.pop())
    q1.push(q2.front());
  return q1;
}

std::string merge_multiline(std::string front, std::string back) {
  std::vector<std::string> front_lines, back_lines;
  std::stringstream front_stream(front), back_stream(back);
  std::string line;
  while (getline(front_stream, line, '\n')) {
    front_lines.push_back(line);
  }
  while (getline(back_stream, line, '\n')) {
    back_lines.push_back(line);
  }
  std::string output = "";
  for (unsigned i = 0; i < front_lines.size(); i++)
    output += front_lines[i] + back_lines[i] + "\n";
  return output;
}

std::tuple<std::queue<int>, std::queue<int>> deal_cards() {
  // Represent cards as numbers will make value comparing easier
  std::vector<int> deck{2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,
                        5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,
                        8,  8,  9,  9,  9,  9,  10, 10, 10, 10, 11, 11, 11,
                        11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14};
  // easiest way to shuffle the deck i think. mersenne_twister_engine bc it's
  // used as expamle on cppreference
  std::shuffle(deck.begin(), deck.end(), std::mt19937(std::random_device()()));
  // well i gotta push one by one anyway (i think) so we going back and forth
  std::queue<int> player1, player2;
  for (int i = 0; i < 52; i++) {
    if (i % 2 == 0)
      player1.push(deck[i]);
    else
      player2.push(deck[i]);
  }
  // easier to return one thing so we tuple both hands
  return std::make_tuple(player1, player2);
}

void print_table(std::queue<int> &player1, std::queue<int> &player2,
                 std::queue<int> war1, std::queue<int> war2,
                 std::string message) {
  const std::string full_card = ".------.\n"
                                "|      |\n"
                                "|      |\n"
                                "|      |\n"
                                "|      |\n"
                                "`------'";
  const std::string half_card = "--.\n"
                                "  |\n"
                                "  |\n"
                                "  |\n"
                                "  |\n"
                                "--'";
  std::string deck1 = full_card;
  deck1.replace(28, 5, "CARDS");
  deck1.replace(21, 2, std::to_string(player1.size()));
  std::string deck2 = full_card;
  deck2.replace(28, 5, "CARDS");
  deck2.replace(21, 2, std::to_string(player2.size()));
  // just to reserve 5 lines already
  std::string table1 = " \n \n \n \n \n ";
  std::string table2 = " \n \n \n \n \n ";
  std::string card; // I need this available after exiting the loop
  for (; !war1.empty(); war1.pop()) {
    switch (war1.front()) {
    case 11:
      card = "J";
      break;
    case 12:
      card = "Q";
      break;
    case 13:
      card = "K";
      break;
    case 14:
      card = "A";
      break;
    default:
      card = std::to_string(war1.front());
      break;
    }
    std::string add = half_card;
    if (war1.size() % 2 == 1)
      add.replace(6 - card.length(), card.length(), card);
    table1 = merge_multiline(add, table1);
  }
  std::string front = ".----\n"
                      "|    \n"
                      "|    \n"
                      "|    \n"
                      "|    \n"
                      "`----";
  front.replace(25, card.length(), card);
  if (table1 != " \n \n \n \n \n ")
    table1 = merge_multiline(front, table1);
  for (; !war2.empty(); war2.pop()) {
    switch (war2.front()) {
    case 11:
      card = "J";
      break;
    case 12:
      card = "Q";
      break;
    case 13:
      card = "K";
      break;
    case 14:
      card = "A";
      break;
    default:
      card = std::to_string(war2.front());
      break;
    }
    std::string add = half_card;
    if (war2.size() % 2 == 1)
      add.replace(6 - card.length(), card.length(), card);
    table2 = merge_multiline(add, table2);
  }
  front = ".----\n"
          "|    \n"
          "|    \n"
          "|    \n"
          "|    \n"
          "`----";
  front.replace(25, card.length(), card);
  if (table2 != " \n \n \n \n \n ")
    table2 = merge_multiline(front, table2);
  std::cout << "PLAYER 1" << std::endl
            << deck1 << std::endl
            << std::endl
            << table1 << std::endl
            << std::endl
            << message << std::endl
            << std::endl
            << table2 << std::endl
            << std::endl
            << deck2 << std::endl
            << "PLAYER 2" << std::endl;
}

int main() {
  std::queue<int> player1, player2;
  std::queue<int> war1, war2;
  std::tie(player1, player2) = deal_cards();
  print_table(player1, player2, war1, war2, "Game of war");
  std::cout << "Press enter to continue..." << std::endl;
  std::cin.ignore();
  do {
    war1.push(player1.front());
    player1.pop();
    war2.push(player2.front());
    player2.pop();
    if (war1.back() > war2.back()) {
      print_table(player1, player2, war1, war2, "Player 1 wins this round!");
      player1 = merge_queues(player1, war1);
      player1 = merge_queues(player1, war2);
      war1 = {};
      war2 = {};
    } else if (war1.back() < war2.back()) {
      print_table(player1, player2, war1, war2, "Player 2 wins this round!");
      player2 = merge_queues(player2, war2);
      player2 = merge_queues(player2, war1);
      war1 = {};
      war2 = {};
    } else {
      print_table(player1, player2, war1, war2, "War has started!");
      if (player1.empty() || player2.empty())
        break;
      war1.push(player1.front());
      player1.pop();
      war2.push(player2.front());
      player2.pop();
    }
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
  } while (!player1.empty() && !player2.empty());
  if (player1.empty())
    print_table(player1, player2, war1, war2, "Player 2 wins!");
  else if (player2.empty())
    print_table(player1, player2, war1, war2, "Player 1 wins!");
  return 0;
}
