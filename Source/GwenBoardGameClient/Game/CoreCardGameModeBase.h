// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

#include "../BasicGameMode.h"
#include "CoreGameBlueprintFunctionLibrary.h"
#include "BoardGrid.h"
#include "BattleCamera.h"
#include "AI/MctsPlayer.h"
#include "CoreCardGameModeBase.generated.h"

/**
 *
 */



UCLASS()
class GWENBOARDGAMECLIENT_API ACoreCardGameModeBase : public ABasicGameMode
{
    GENERATED_BODY()
public:

    virtual void BeginPlay() override;

    virtual void InitEvents() override;

    virtual void Tick(float deltaTime) override;

    UFUNCTION(BlueprintNativeEvent)
    void InitDone();

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACard> cardBPClass;

    UPROPERTY(EditDefaultsOnly)
    FVector initSpawnCardLoc;

    UPROPERTY(EditDefaultsOnly)
    FVector selectCardDemoLoc;

    UPROPERTY(EditDefaultsOnly)
    float selectCardInterval;

    UPROPERTY(EditDefaultsOnly)
    int32 maxSelectCardNb;

    UPROPERTY(EditDefaultsOnly)
    int32 availableSwitchCardNb;

    UPROPERTY(EditAnywhere)
    TArray<FVector> selectCardSpawnPts;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> battleCameraBPClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> boardGridBPClass;

    UPROPERTY(EditDefaultsOnly)
    float gridSpawnCardOffset = 0.1;

    UPROPERTY(EditDefaultsOnly)
    float cardSelfRotInterval;

    UPROPERTY(EditDefaultsOnly)
    float spreadCardRotInterval;

    UPROPERTY(EditDefaultsOnly)
    float spreadCardRotRadius;

    UPROPERTY(EditDefaultsOnly)
    float spreadCardHeight;

    UPROPERTY(EditDefaultsOnly)
    float spreadCardStepHeight;

    UPROPERTY(EditDefaultsOnly)
    FVector spreadCardOffset;

    UPROPERTY(EditDefaultsOnly)
    float hoverMoveRightCardsOffset;

    UPROPERTY(EditDefaultsOnly)
    float hoverCardUpOffset;

    UPROPERTY(EditDefaultsOnly)
    float hoverMoveCardInterpDeltaTime;

    UPROPERTY(EditDefaultsOnly)
    float hoverMoveCardInterpSpeed;


    TMap<CameraType, ABattleCamera*> camerasMap;

    UPROPERTY(EditAnywhere)
    TMap<FString, TSubclassOf<AActor>> effectParticleActorMap;

    UPROPERTY()
    ABattleBoard* battleBoard;

    UPROPERTY(EditDefaultsOnly)
    EGamingType gamingType = EGamingType::Training;
public:
    // --- Select card logic
    UFUNCTION(BlueprintNativeEvent)
    void TriggerReadCardInfo();

    void RandSelectCards();

    void FinishCardSelection();

    UPROPERTY(EditDefaultsOnly)
    int32 startingSelectCardNb = 10;

    // --- Main game logic
    int32 curActionEffectRound = 0;

    TArray<FRenderEffectRound> curActionRenderEffectRoundList;

    ActionType curActionType;

    void ReqPlayCard(bool simulationFlag, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    void ReqLaunchCardSkill(bool simulationFlag, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    void TriggerRenderEffect();

    //void LaunchSkill();

    UPROPERTY()
    TArray<ACard*> testCards;

    TArray<FRotator> testCardRots;
    TArray<FVector> testCardLocations;

    TArray<FVector> testCardTempLocations;

    ACard* testMoveCard;

    void SpawnTestCards();

    void RearrangeCardLocations(int32 hoverCardNb);

    void RecoverCardLocations();

    void CalculateCardSpread();

    void MoveRearrangeCards();

    UFUNCTION(BlueprintCallable)
    void MoveCard(FVector originLoc, FVector targetLoc, float midHeight);

    // --- Training logic
    void SimulateTrainAction(float dT);

    void TrainPlayGameLoop(float dT);

    AMctsPlayer* mctsPlayer;

    UPROPERTY(EditDefaultsOnly)
    float aiTrainPlayerActionInterval;

    float aiTrainPlayerActionCount = 0.0;

    // --- Single game logic

    void SinglePlayerGameLoop(float dT);

    void GetLegalLaunchSkillAction(TMap<int32, FBoardRow>& boardCardInfo, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY);

    // --- Local logic functions
    void CheckEntitiesCreated();

    void GetAllPresetObjects();

    void InitPreBattle();

    //void SetupBattleBoardAndCards();

    //void TriggerBattlePreparation();

    void CalibrateGridInfos(TArray<FBATTLE_GRID_INFO> gridInfos);

    void CalibratePlayerCardInfos(TArray<FSYNC_CARD_INFO> allCardInfoList, TArray<FString> handCardUidList);

    void CalibrateCurrentGlobalInfo(int32 curActionSequence, int32 curSwitchControllerSequence, uint8 curControllerNb);

    // --- Account sync functions
    void onEnterWorld(const UKBEventData* eventData);

    // --- Avatar req functions
    void ReqChangeSelectCard(FString changeCardKey);

    void ReqFinishSelectCards();

    void ReqUpdateSelectedCard();

    void ReqSyncHeartBeat();

    void ReqLatestBattleInfo();

    void MultiPlayerReqPlayCardAction(int32 actionSequence, FString cardUid, int32 gridNb);

    void MultiPlayerReqLaunchCardSkill(int32 actionSequence, FString cardUid, FString skillName, int32 launchGridNb, int32 targetGridNb);

    // --- Avatar sync functions

    void onReceiveUpdateCoreGame(const UKBEventData* eventData);

    void onUpdateGridInfoList(const UKBEventData* eventData);

    void onSyncBattleResult(const UKBEventData* eventData);

    void onSyncChangeHandCardSuccess(const UKBEventData* eventData);

    void onSyncExhaustCardReplacement(const UKBEventData* eventData);

    void onSyncHeartBeat(const UKBEventData* eventData);

    void onSyncLatestBattleState(const UKBEventData* eventData);

    void onSyncLaunchSkillFailed(const UKBEventData* eventData);

    virtual void onSyncPlayerBattleInfo(const UKBEventData* eventData) override;

    void onSyncReceiveEnterRoom(const UKBEventData* eventData);

    void onSyncReceiveFinishCardSelection(const UKBEventData* eventData);

    void onSyncResumeBattle(const UKBEventData* eventData);

    void onSyncRoomStartBattle(const UKBEventData* eventData);

    void onSyncSelectCardInterlude(const UKBEventData* eventData);

    void onSyncSwitchController(const UKBEventData* eventData);

    void onSyncTimeInterval(const UKBEventData* eventData);

    void onSyncUpdateSelectedCards(const UKBEventData* eventData);


    virtual void SpawnSelectCard() override;

    bool isSinglePlay = true;

    bool isHumanTurn = true;

    bool hasReqEnterRoom = false;

    SingleBattleState singleBattleState = SingleBattleState::Default;

    InterludeState interludeState = InterludeState::Default;

    NetworkStatus networkStatus = NetworkStatus::Default;

    ClientBattleState clientBattleState = ClientBattleState::Default;

    TMap<FString, float> interludeStateTicksMap;

    UPROPERTY(EditDefaultsOnly)
    TMap<FString, float> battleStateTicksMap;

    float curBattleStateTick = 0.0;

    int32 receiveActionSequence = 0;

    float curCountingTick = 0.0;

    float curReqEnterRoomTick = 0.0;

    uint8 receiveControllerNb = 0;

    int32 receiveSwitchControllerSequence = 0;

    UPROPERTY()
        TMap<int32, ABoardGrid*> boardGrids;

    UPROPERTY()
        TMap<int32, ACard*> occupiedGridCardMap;

    UPROPERTY()
        TMap<FString, FSYNC_CARD_INFO> allCardInfoMap;

    UPROPERTY()
    TMap<FString, ACard*> allCardMap;

    uint8 maxChangeSelectCardNb = 3;
    uint8 curChangeSelectCardNb = 0;
};
