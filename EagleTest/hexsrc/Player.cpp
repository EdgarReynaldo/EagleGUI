



#include "Player.hpp"



void Player::CalcIncome() {
   our_turf.CalcIncome();
}



void Player::EndTurn() {
   CalcIncome();
   our_turf.CollectIncome();
}

