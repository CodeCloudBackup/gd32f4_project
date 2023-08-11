#ifndef __MOS_SW_H
#define __MOS_SW_H

#include "systick.h"

#define OUT_DRIVE_CTR1 PEout(10)	// DS0
#define OUT_DRIVE_CTR2 PEout(11)	// DS0
#define OUT_DRIVE_CTR3 PEout(12)	// DS0

#define E3FDS_SW PGout(5)	// DS0
#define LOCK_SW PGout(8)	// DS0

void OutDirveInit(void);

#endif

