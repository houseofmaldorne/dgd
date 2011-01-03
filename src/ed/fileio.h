/*
 * This file is part of DGD, http://dgd-osr.sourceforge.net/
 * Copyright (C) 1993-2010 Dworkin B.V.
 * Copyright (C) 2010 DGD Authors (see the file Changelog for details)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

typedef struct {
    Int lines;	/* number of lines */
    Int chars;	/* number of characters */
    Int zero;	/* number of zeroes discarded */
    Int split;	/* number of splits of too long lines */
    bool ill;	/* incomplete last line */
} io;

extern bool io_load (editbuf*, char*, Int, io*);
extern bool io_save (editbuf*, char*, Int, Int, int, io*);
