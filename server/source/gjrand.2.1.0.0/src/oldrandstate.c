/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

struct gjrand gjrand_inte_oldrand_state=
{
    0, 2*123456789ul, 0, 1000001ul
};
