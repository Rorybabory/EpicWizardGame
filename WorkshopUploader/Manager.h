#pragma once
#include "steam/steam_api.h"

class CGameManager
{
public:
    void CreateItem();

    bool finished = false;

private:
    void onItemSubmitted(SubmitItemUpdateResult_t* pCallback, bool bIOFailure);

    CCallResult<CGameManager, CreateItemResult_t> m_CreateItemResult;
    CCallResult<CGameManager, SubmitItemUpdateResult_t> m_SubmitItemUpdateResult;
};
