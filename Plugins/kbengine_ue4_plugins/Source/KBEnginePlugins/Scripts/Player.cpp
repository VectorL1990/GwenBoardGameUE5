#include "Player.h"
#include "Engine/Entity.h"
#include "Engine/KBEngine.h"
#include "Engine/KBDebug.h"

namespace KBEngine
{

    Player::Player() : PlayerBase()
    {
    }

    Player::~Player()
    {
    }

    void Player::onUpdateBattle(const CORE_UPDATE_BATLLE_INFO& arg1)
    {}

    void Player::onUpdateGridInfoList(const UPDATE_GRID_INFO_LIST& arg1)
    {}
}