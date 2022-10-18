#pragma once

#include <KIR/hash/KIR5_sha512.h>

struct Account
{
	KIR5::sha512digest hash;
};