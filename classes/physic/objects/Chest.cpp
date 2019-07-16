#include "../../../stdafx.h"
#include "Chest.h"

MChest::MChest():MLevelObject() {
	Closed = true;
	Type = LOT_CHEST;
}

bool MChest::GetClosed() {
	return Closed;
}

void MChest::SetClosed(bool Value){
	Closed = Value;
}
