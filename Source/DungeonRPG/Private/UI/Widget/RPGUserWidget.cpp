// DungeonRPG


#include "UI/Widget/RPGUserWidget.h"

void URPGUserWidget::SetWidgetController(UObject *InWidgetController)
{
	this->WidgetController = InWidgetController;
	AfterSetWidgetController();
}
