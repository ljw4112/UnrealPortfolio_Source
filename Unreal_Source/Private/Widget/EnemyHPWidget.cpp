#include "Widget/EnemyHPWidget.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyHPWidget::SetPercent(float Percent)
{
	if (!HPBar) return;
	HPBar->SetPercent(UKismetMathLibrary::FClamp(Percent, 0, 1));
}
