// DungeonRPG


#include "AbilitySystem/RPGAbilitySystemFunctionLibrary.h"

#include "Game/RPGGameModeBase.h"
#include "Game/RPGGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RPGPlayerState.h"
#include "UI/HUD/RPGHUD.h"
#include "UI/WidgetController/RPGWidgetController.h"

UOverlayWidgetController* URPGAbilitySystemFunctionLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController *PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(ARPGHUD *HUD = Cast<ARPGHUD>(PC->GetHUD()))
		{
			ARPGPlayerState *PS = PC->GetPlayerState<ARPGPlayerState>();
			UAbilitySystemComponent *ASC = PS->GetAbilitySystemComponent();
			UAttributeSet *AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC, PS, ASC, AS);
			return HUD->GetOverlayWidgetController(Params);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* URPGAbilitySystemFunctionLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController *PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(ARPGHUD *HUD = Cast<ARPGHUD>(PC->GetHUD()))
		{
			ARPGPlayerState *PS = PC->GetPlayerState<ARPGPlayerState>();
			UAbilitySystemComponent *ASC = PS->GetAbilitySystemComponent();
			UAttributeSet *AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC, PS, ASC, AS);
			return HUD->GetAttributeMenuWidgetController(Params);
		}
	}
	return nullptr;
}

USpellMenuWidgetController* URPGAbilitySystemFunctionLibrary::GetSpellMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController *PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(ARPGHUD *HUD = Cast<ARPGHUD>(PC->GetHUD()))
		{
			ARPGPlayerState *PS = PC->GetPlayerState<ARPGPlayerState>();
			UAbilitySystemComponent *ASC = PS->GetAbilitySystemComponent();
			UAttributeSet *AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC, PS, ASC, AS);
			return HUD->GetSpellMenuWidgetController(Params);
		}
	}
	return nullptr;
}

const UAbilityInfo* URPGAbilitySystemFunctionLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ARPGGameStateBase *GameState = Cast<ARPGGameStateBase>(UGameplayStatics::GetGameState(WorldContextObject));
	if (GameState) return GameState->AbilityInfoCopy;
	return nullptr;
}
