// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPGHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class URPGUserWidget;
/**
 * 
 */
UCLASS()
class DUNGEONRPG_API ARPGHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController *GetOverlayWidgetController(const FWidgetControllerParams &WCParams);

	void InitOverlay(APlayerController *PC, APlayerState *PS, UAbilitySystemComponent *ASC, UAttributeSet *AS);

	UAttributeMenuWidgetController *GetAttributeMenuWidgetController(const FWidgetControllerParams &WCParams);

	USpellMenuWidgetController *GetSpellMenuWidgetController(const FWidgetControllerParams &WCParams);

private:
	UPROPERTY()
	TObjectPtr<URPGUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URPGUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
