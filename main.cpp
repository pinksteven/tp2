#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
#include <tuple>
#include <vector>

std::tuple<std::queue<int>, std::queue<int>> deal_cards() {
  std::vector<int> deck{2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,
                        5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,
                        8,  8,  9,  9,  9,  9,  10, 10, 10, 10, 11, 11, 11,
                        11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14};
  std::shuffle(deck.begin(), deck.end(), std::mt19937(std::random_device()()));
  std::queue<int> player1, player2;
  for (int i = 0; i < 52; i++) {
    if (i % 2 == 0)
      player1.push(deck[i]);
    else
      player2.push(deck[i]);
  }
  return std::make_tuple(player1, player2);
}

int main() {
  std::queue<int> player1, player2;
  std::tie(player1, player2) = deal_cards();
  for (; !player1.empty(); player1.pop())
    std::cout << player1.front() << ' ';
  return 0;
}
