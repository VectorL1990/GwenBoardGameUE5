// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "AI/AIRunnable.h"
#include "../BasicGameMode.h"
#include "CoreGameBlueprintFunctionLibrary.h"
#include "BoardGrid.h"
#include "BattleCamera.h"
#include "AI/MctsPlayer.h"
#include "ReplayBoard.h"
#include "ReplayCard.h"
#include "Card.h"
#include "NiagaraActor.h"
#include "Render/PSGuideActor.h"
#include "Render/SlashPSActor.h"
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

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintNativeEvent)
    void InitDone();

    EGameModeRenderState gameModeRenderState = EGameModeRenderState::Default;

    int32 curRenderingRound = 0;

    TArray<FRenderActionNode> curRenderingActionNodes;

    float curRenderEndRoundTime = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float endRoundRenderInterval = 1.0;



    


    void OperateCountDown(float dT);

    void AIMovementCountDown(float dT);

    void TriggerAIAskAction(uint8 inTriggerSection);

    UPROPERTY(EditDefaultsOnly)
    float playerOperateTime = 90.0;

    float curPlayerOperateLeftTime;

    bool isTrain = false;

    bool testManualTriggerAIAsk = false;

    float curAIMovementWaitTime = 0.0;

    float aiMovementWaitTime = 1.0;

    float curAIWaitResponseTime = 0.0;

    float aiWaitResponseTime = 3.0;



    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACard> cardBPClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AReplayCard> replayCardBPClass;

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
    TSubclassOf<UMcts> mctsBPClass;

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
    FVector sectionZeroSpreadCardOffset;

    UPROPERTY(EditDefaultsOnly)
    FVector sectionOneSpreadCardOffset;

    UPROPERTY(EditDefaultsOnly)
    float hoverMoveRightCardsOffset;

    UPROPERTY(EditDefaultsOnly)
    float hoverMoveCardsHorizontalOffset;

    UPROPERTY(EditDefaultsOnly)
    float hoverCardUpOffset;

    UPROPERTY(EditDefaultsOnly)
    float hoverMoveCardInterpDeltaTime;

    UPROPERTY(EditDefaultsOnly)
    float hoverMoveCardInterpSpeed;

    UPROPERTY(EditDefaultsOnly)
    FVector sectionZeroGraveAreaLocation;

    UPROPERTY(EditDefaultsOnly)
    FVector sectionOneGraveAreaLocation;

    UPROPERTY(EditDefaultsOnly)
    FVector gridCardVerticalOffset;

    UPROPERTY(EditDefaultsOnly)
    float gridGuidePSOffset = 50.0;

    UPROPERTY(EditDefaultsOnly)
    FVector replayGridCardVerticalOffset;

    ACard* selectPlayCard;

    ACard* selectBoardCard;

    TMap<CameraType, ABattleCamera*> camerasMap;

    UPROPERTY(EditAnywhere)
    TMap<FString, TSubclassOf<AActor>> effectParticleActorMap;

    UPROPERTY()
    ABattleBoard* battleBoard;

    UPROPERTY(EditDefaultsOnly)
    EGamingType gamingType = EGamingType::Training;

    UPROPERTY(EditDefaultsOnly)
    FVector replaySectionZeroFirstHandCardLoc;

    UPROPERTY(EditDefaultsOnly)
    FVector replaySectionOneFirstHandCardLoc;

    UPROPERTY(EditDefaultsOnly)
    float replayHandCardOffset;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<APSGuideActor> cardConnectPSBPClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ASlashPSActor> redSlashNiagaraBPClass;

    
public:
    // --- Select card logic
    UFUNCTION(BlueprintNativeEvent)
    void TriggerReadCardInfo();

    void FinishCardSelection();

    // --- Main game logic
    class FAIRunnable* aiRunnable;

    void TestTriggerSimulation();

    void TestTriggerTritonInference();

    void TestTriggerAction(uint8 campNb, int32 launchX, int32 launchY, int32 targetX, int32 targetY, ActionType actionType);

    void DemonstrateMctsTreeNode(UMctsTreeNode* node);

    void TryTriggerEndRound();

    void TimeoutTriggerPlayCardAction(uint8 campNb);

    void UpdateEndRoundButtonState(EEndRoundButtonState state);

    UPROPERTY(Transient)
    TObjectPtr<UMcts> mcts;


    int32 curActionEffectRound = 0;


    ActionType curActionType;


    void ReqLaunchCardSkill(bool simulationFlag, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    void TriggerSkillHurtRender(FVector startPt, const TArray<FVector>& targetPts);

    //void LaunchSkill();

    UPROPERTY()
    TArray<ACard*> sectionZeroHandBattleCards;

    UPROPERTY()
    TArray<ACard*> sectionOneHandBattleCards;

    UPROPERTY()
    TMap<int32, ACard*> allBattleCards;

    UPROPERTY()
    TMap<int32, AReplayCard*> allReplayCards;

    ACard* curHighlightCard;

    TArray<FRotator> sectionZeroCardRots;
    TArray<FRotator> sectionOneCardRots;
    TArray<FVector> sectionZeroOriginLocations;
    TArray<FVector> sectionOneOriginLocations;
    TArray<FVector> sectionZeroCardLocations;
    TArray<FVector> sectionOneCardLocations;

    void SpawnHandCard(FString cardName, uint8 sectionNb, int32 cardUid, int32 inCurHp, int32 inCurDefence, int32 inCurCd, int32 inCurAvailable, int32 handCardNb);

    void DeleteHandAllCards();

    void CalculateHoverCardLocations(uint8 campNb, int32 hoverCardNb);

    void RecoverHoverCardLocations();

    void SetSelectPlayCard(uint8 campNb, ACard* inSelectCard);

    void RecoverSelectPlayCard();

    void CalculateCardSpread();

    void MoveRearrangeCards();

    // --- Training logic

    AReplayBoard* replayBoard;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AMctsPlayer> mctsPlayerBPClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ABattleBoard> battleBoardBPClass;

    AMctsPlayer* sectionZeroMctsPlayer;

    AMctsPlayer* sectionOneMctsPlayer;

    UPROPERTY(EditDefaultsOnly)
    float aiTrainPlayerActionInterval;

    float aiTrainPlayerActionCount = 0.0;

    // --- Single game logic

    uint8 curSectionNb = 0;

    void GetLegalLaunchSkillAction(TMap<int32, FBoardRow>& boardCardInfo, TMap<int32, FInstanceCardInfo>& allInstanceCardInfo, FEffectInfo& effectInfo, int32 launchX, int32 launchY);

    // --- Local logic functions
    void CheckEntitiesCreated();

    void GetAllPresetObjects();

    void InitPreBattle();

    //void SetupBattleBoardAndCards();

    //void TriggerBattlePreparation();


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

    uint8 maxChangeSelectCardNb = 3;
    uint8 curChangeSelectCardNb = 0;



};
