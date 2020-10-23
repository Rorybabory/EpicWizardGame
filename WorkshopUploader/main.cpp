#include <iostream>
#include "steam/steam_api.h"

//The Game Manager class is needed to handle Steamworks callbacks
class CGameManager
{
public:
    void CreateItem();

    bool finished = false;
    AppId_t nConsumerAppID = (AppId_t)1441830;

private:
    void onItemCreated(CreateItemResult_t* pCallback, bool bIOFailure);
    void onItemSubmitted(SubmitItemUpdateResult_t* pCallback, bool bIOFailure);

    CCallResult<CGameManager, CreateItemResult_t> m_CreateItemResult;
    CCallResult<CGameManager, SubmitItemUpdateResult_t> m_SubmitItemUpdateResult;
};

//We create a new Item by providing the game ID and we specify we are creating a normal mod that can be subscribed to
void CGameManager::CreateItem()
{
    std::cout << "Creating item..." << std::endl;

    SteamAPICall_t hSteamAPICall = SteamUGC()->CreateItem(nConsumerAppID, k_EWorkshopFileTypeCommunity);
    m_CreateItemResult.Set(hSteamAPICall, this, &CGameManager::onItemCreated);
}

//Once the mod was created, we can grab the id from the callback params and then customize it with the UGCUpdateHandle_t before making another API call to SubmitItemUpdate
void CGameManager::onItemCreated(CreateItemResult_t* pCallback, bool bIOFailure)
{
    if (pCallback->m_eResult == k_EResultOK && !bIOFailure)
    {
        std::cout << "Item created!" << std::endl;

        if (pCallback->m_bUserNeedsToAcceptWorkshopLegalAgreement)
        {
            SteamFriends()->ActivateGameOverlayToWebPage("steam://url/CommunityFilePage/");
        }

        UGCUpdateHandle_t handle = SteamUGC()->StartItemUpdate(nConsumerAppID, pCallback->m_nPublishedFileId);

        SteamUGC()->SetItemTitle(handle, "Title test");
        SteamUGC()->SetItemDescription(handle, "Description test");
        SteamUGC()->SetItemUpdateLanguage(handle, "None");
        SteamUGC()->SetItemMetadata(handle, "Test metadata");
        SteamUGC()->SetItemVisibility(handle, k_ERemoteStoragePublishedFileVisibilityPublic);

        SteamParamStringArray_t* pTags = new SteamParamStringArray_t();
        pTags->m_ppStrings = new const char* [1];
        pTags->m_ppStrings[0] = "stage";
        pTags->m_nNumStrings = 1;
        SteamUGC()->SetItemTags(handle, pTags);
        SteamUGC()->AddItemKeyValueTag(handle, "test_key", "test_value");
        std::string mod_directory = "./mods/testMod";
        SteamUGC()->SetItemContent(handle, mod_directory.c_str());
        std::string preview_image = "TestImage.png";
        SteamUGC()->SetItemPreview(handle, preview_image.c_str());

        std::string pchChangeNote = "This is a changelog";

        SteamAPICall_t submit_item_call = SteamUGC()->SubmitItemUpdate(handle, pchChangeNote.c_str());
        m_SubmitItemUpdateResult.Set(submit_item_call, this, &CGameManager::onItemSubmitted);
    }
    else
    {
        finished = true;
    }
}

void CGameManager::onItemSubmitted(SubmitItemUpdateResult_t* pCallback, bool bIOFailure)
{
    if (pCallback->m_eResult == k_EResultOK && !bIOFailure)
    {
        std::cout << "Item update submitted created!" << std::endl;
    }
    finished = true;
}

int main()
{
    //First, let's check if the Steam client is running
    if (SteamAPI_Init())
    {
        CGameManager gameManager;
        //Now let's trigger the item creation process
        gameManager.CreateItem();

        //Dont forget to run the callbacks while we wait
        while (!gameManager.finished)
        {
            SteamAPI_RunCallbacks();
        }
    }

    std::cout << "Process finished" << std::endl;

    return 0;
}