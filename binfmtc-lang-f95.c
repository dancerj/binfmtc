/*
 *  binfmt_misc C Interpreter
 *  Copyright (C) 2005 Junichi Uekawa
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Fortran bindings.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "binfmtc.h"

const char* gcc_x = " -x f95 ";
const char* language_type = "Fortran 95";
const char* binfmtc_magic="! BINFMTF95:";

const char* compiler_name(void)
{
  return  getenv("GFORTRAN")?:
    "gfortran";
}

const char* default_options(void)
{
  return getenv("BINFMTC_GFORTRAN_OPTS")?:
    " -O2 -Wall ";
}

