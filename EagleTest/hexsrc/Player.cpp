



#include "Player.hpp"



void Player::CalcIncome() {
   our_turf.CalcIncome();
}



void Player::EndTurn() {
   CalcIncome();
   points += our_turf.TotalIncome();
}

