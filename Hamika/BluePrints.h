#pragma once

#include "Types.h"
#include "Array2D.h"

#include "Objects.h"

#include <string>
#include <fstream>
#include <vector>

#include "BluePrint.h"

#include <KIR/hash/KIR5_sha512.h>


/*

* -> FileIO -> TypeSelector
						-> BinaryEncoder -> HashChecker -> WorldSelector -> WouldIO -> BluePrintSelector -> HashChecker -> BluePrintIO -> *


*/
