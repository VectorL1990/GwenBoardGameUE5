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

    float curTestOutputRuntimeInterval = 0.0;

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

    UPROPERTY(Transient)
    TObjectPtr<UMcts> mcts;


    int32 curActionEffectRound = 0;

    TArray<FRenderEffectRound> curActionRenderEffectRoundList;

    ActionType curActionType;


    void ReqLaunchCardSkill(bool simulationFlag, int32 launchX, int32 launchY, int32 targetX, int32 targetY);

    void TriggerRenderEffect();

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
    bool isTrain = true;

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



    AActor* testMotionCard;
    ECardVerticalMotionStage testCardMotionStage = ECardVerticalMotionStage::Default;

    ECardRotationStage testCardRotationStage = ECardRotationStage::Default;

    ECardHorizonMotionStage testCardHorizonXMotionStage = ECardHorizonMotionStage::Default;

    ECardHorizonMotionStage testCardHorizonYMotionStage = ECardHorizonMotionStage::Default;

    float testMotionCardRiseTop = 0.0;

    float testMotionCardXOffset = 0.0;

    float testMotionCardYOffset = 0.0;

    float testMotionCardXAccMotion = 0.0;

    float testMotionCardYAccMotion = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float testMotionAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float testDropAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float testRebounceAcc = 100;

    UPROPERTY(EditDefaultsOnly)
    float testRebounceSpeedLost = 0.5;

    float testMotionCardSpeed = 0.0;

    float testMotionCardHorizonXSpeed = 0.0;

    float testMotionCardHorizonYSpeed = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float maxMotionCardSpeed = 100.0;

    UPROPERTY(EditDefaultsOnly)
    float testMotionCardRotationAttenuation = 0.5;

    UPROPERTY(EditDefaultsOnly)
    float testMotionCardRotateSpeed = 0.4;

    UPROPERTY(EditDefaultsOnly)
    float testMotionCardMaxRotationPitch = 20.0;

    float testMotionCardCurRotationPitch = 0.0;

    float testMotionCardNextRotationPitch = 0.0;

    float testMotionCardHorizonXAcc = 0.0;

    float testMotionCardHorizonYAcc = 0.0;

    FVector2D testMotionCardHorizonTarget = FVector2D::Zero();

    UPROPERTY(EditDefaultsOnly)
    float testMotionHorizonT = 1.0;

    float testMotionCardCurXLoc = 0.0;

    float testMotionCardCurYLoc = 0.0;

    UPROPERTY(EditDefaultsOnly)
    float testMotionCardLerp = 0.2;

    float testMotionCurCountT = 0.0;

    float testMotionRiseT = 0.0;

    float testMotionTotalT = 0.0;

    UFUNCTION(BlueprintCallable)
    void TestTriggerCardMotion(FVector2D targetHorizonTarget);

    UFUNCTION(BlueprintCallable)
    void TestTriggerCardRotation();

    UFUNCTION(BlueprintCallable)
    void TestCardHorizonMotion(float dT);

    UFUNCTION(BlueprintCallable)
    void TestCardMotion(float dT);

    UFUNCTION(BlueprintCallable)
    void TestCardRotation(float dT);
};
