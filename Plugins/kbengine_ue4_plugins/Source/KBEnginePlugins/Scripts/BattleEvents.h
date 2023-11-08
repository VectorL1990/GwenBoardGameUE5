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
struct FBATTLE_GRID_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
								int32 gridNb;

				UPROPERTY(EditAnywhere)
								int32 cardUid;

				UPROPERTY(EditAnywhere)
								int32 hp;

				UPROPERTY(EditAnywhere)
								int32 defence;

				UPROPERTY(EditAnywhere)
								uint8 agility;
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
class KBENGINEPLUGINS_API UKBEventData_ReqEnterRoom : public UKBEventData
{
				GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onReqTest : public UKBEventData
{
				GENERATED_BODY()

public:
				UPROPERTY(EditAnywhere)
								int32 testParam;
};

USTRUCT(BlueprintType)
struct FPERSIST_CARD_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
				FString cardName;
};

USTRUCT(BlueprintType)
struct FPERSIST_PLAYER_INFO
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
				TArray<FPERSIST_CARD_INFO> cardList;
};

class KBENGINEPLUGINS_API UKBEventData_syncPersistPlayerInfo : public UKBEventData
{
				GENERATED_BODY()
public:
				UPROPERTY(EditAnywhere)
				FPERSIST_PLAYER_INFO persistPlayerInfo;
};

class KBENGINEPLUGINS_API UKBEventData_syncInitializeAllCard : public UKBEventData
{
				GENERATED_BODY()
public:
				
};

