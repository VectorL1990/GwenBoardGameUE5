#pragma once
#include "Engine/KBECommon.h"
#include "Engine/KBEvent.h"
#include "BattleEvents.generated.h"

USTRUCT(BlueprintType)
struct FPLAY_CARD_KBE_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								int32 playCardId;

				UPROPERTY(EditAnywhere)
								int32 targetGridNb;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_reqSendAction : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								FString actionType;

				UPROPERTY(EditAnywhere)
								FPLAY_CARD_KBE_INFO playCardInfo;
};

USTRUCT(BlueprintType)
struct FUPDATE_GRID_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								int32 gridNb;

				UPROPERTY(EditAnywhere)
								int32 cardUid;

				UPROPERTY(EditAnywhere)
								FString updateType;

				UPROPERTY(EditAnywhere)
								int32 updateValue;
};

USTRUCT(BlueprintType)
struct FUPDATE_GRID_INFO_LIST
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								int32 updateId;

				UPROPERTY(EditAnywhere)
								TArray<FUPDATE_GRID_INFO> updateGridList;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onUpdateGridInfoList : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								FUPDATE_GRID_INFO_LIST updateInfoList;
};

USTRUCT(BlueprintType)
struct FSTATE_TAG_INFO
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FString stateName;

	UPROPERTY(EditAnywhere)
		uint8 stipulation;

	UPROPERTY(EditAnywhere)
		uint8 curCount;
};

USTRUCT(BlueprintType)
struct FSYNC_EFFECT_INFO
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FString effectName;

	UPROPERTY(EditAnywhere)
		uint8 availableTimes;

	UPROPERTY(EditAnywhere)
		uint8 countDown;
};

USTRUCT(BlueprintType)
struct FBATTLE_GRID_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								int32 gridNb;

				UPROPERTY(EditAnywhere)
								FString cardUid;

				UPROPERTY(EditAnywhere)
				FString cardName;

				UPROPERTY(EditAnywhere)
				uint8 hp;

				UPROPERTY(EditAnywhere)
				uint8 defence;

				UPROPERTY(EditAnywhere)
				uint8 agility;

				UPROPERTY(EditAnywhere)
				TArray<FString> tags;

				UPROPERTY(EditAnywhere)
				TArray<FSTATE_TAG_INFO> stateTags;

				UPROPERTY(EditAnywhere)
				TArray<FSYNC_EFFECT_INFO> effectInfos;

				UPROPERTY(EditAnywhere)
								FString avatarId;
};

USTRUCT(BlueprintType)
struct FCORE_UPDATE_BATLLE_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								TArray<FBATTLE_GRID_INFO> updateInfos;
};



UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onUpdateBattle : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								FCORE_UPDATE_BATLLE_INFO updateInfo;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_ReqTest : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								int32 testParam;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onReqTest : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								int32 testParam;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_ReqModifyCardGroup : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								int32 groupNb;

				UPROPERTY(EditAnywhere)
				TArray<FString> cardList;
};

USTRUCT(BlueprintType)
struct FCardGroup
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								TArray<FString> cardList;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_ReceivePlayerPersistInfo : public UKBEventData
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
				TArray<FCardGroup> cardGroups;
};







USTRUCT(BlueprintType)
struct FSYNC_CARD_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								FString cardKey;

				UPROPERTY(EditAnywhere)
								FString cardName;

				UPROPERTY(EditAnywhere)
								uint8 hp;

				UPROPERTY(EditAnywhere)
								uint8 defence;

				UPROPERTY(EditAnywhere)
								uint8 agility;

				UPROPERTY(EditAnywhere)
								TArray<FString> tags;

				UPROPERTY(EditAnywhere)
								TArray<FSTATE_TAG_INFO> stateTags;

				UPROPERTY(EditAnywhere)
								TArray<FSYNC_EFFECT_INFO> effectInfos;
};

USTRUCT(BlueprintType)
struct FPLAY_CARD_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								FString cardName;

				UPROPERTY(EditAnywhere)
								int32 hp;

				UPROPERTY(EditAnywhere)
								int32 defence;

				UPROPERTY(EditAnywhere)
								int32 agility;

				UPROPERTY(EditAnywhere)
								TArray<FString> tags;

				UPROPERTY(EditAnywhere)
								FString cardDescriptionCn;

				UPROPERTY(EditAnywhere)
								FString cardDescriptionEn;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncPlayerBattleInfo : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								TArray<FSYNC_CARD_INFO> cardList;

				UPROPERTY()
								TArray<FString> handCardList;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncReceiveEnterRoom : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								uint8 result;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncRoomCreated : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								FString sRoomKey;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncChangeHandCardSuccess : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								uint8 changeSelectCardNb;

				UPROPERTY(EditAnywhere)
								FString changeHandCardKey;

				UPROPERTY(EditAnywhere)
								FString changePileCardKey;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_reqChangeSelectCard : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								uint8 changeNb;

				UPROPERTY(EditAnywhere)
								FString changeCardKey;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncUpdateSelectedCards : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								uint8 changeNb;

				UPROPERTY(EditAnywhere)
								TArray<FSYNC_CARD_INFO> cardList;

				UPROPERTY(EditAnywhere)
								TArray<FString> handCardList;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncLaunchSkillFailed : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
				int32 curActionSequence;

				UPROPERTY(EditAnywhere)
				int32 clientActionSequence;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncBattleResult : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
				TArray<FString> losePlayerList;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncHeartBeat : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
				int32 curBattleTick;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncLatestBattleState : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
				int32 curSwitchControllerSequence;

				UPROPERTY(EditAnywhere)
				uint8 curControllerNb;

				UPROPERTY(EditAnywhere)
				FString curControllerAvatarId;

				UPROPERTY(EditAnywhere)
				int32 curActionSequence;

				UPROPERTY(EditAnywhere)
				TArray<FBATTLE_GRID_INFO> updateGridInfos;

				UPROPERTY(EditAnywhere)
				TArray<FSYNC_CARD_INFO> cardList;

				UPROPERTY(EditAnywhere)
				TArray<FString> handCardList;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncResumeBattle : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
				int32 controllerNb;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncSelectCardInterlude : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								TArray<FSYNC_CARD_INFO> cardList;

				UPROPERTY(EditAnywhere)
								TArray<FString> handCardList;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncSwitchController : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
				int32 controllerNb;

				UPROPERTY(EditAnywhere)
								FString avatarId;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncTimeInterval : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								uint8 battleState;

				UPROPERTY(EditAnywhere)
								int32 curTime;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSyncUpdateActionInfo : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
				int32 actionSequence;

				UPROPERTY(EditAnywhere)
				TArray<FBATTLE_GRID_INFO> gridInfos;

				UPROPERTY(EditAnywhere)
				TArray<FSYNC_CARD_INFO> updateCardInfos;
				
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_reqPlayCardAction : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								int32 actionSequence;

				UPROPERTY(EditAnywhere)
								FString cardUid;

				UPROPERTY(EditAnywhere)
								int32 gridNb;

};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_reqLaunchCardSkill : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								int32 actionSequence;

				UPROPERTY(EditAnywhere)
								FString cardUid;

				UPROPERTY(EditAnywhere)
								FString skillName;

				UPROPERTY(EditAnywhere)
								int32 launchGridNb;

				UPROPERTY(EditAnywhere)
								int32 targetGridNb;

};
