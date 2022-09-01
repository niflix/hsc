# Makefile for HSC automated tests ("HTML Sucks Completely")
# Copyright (C) 2022 Ralf Herrmann
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


# While testing hsc outputs a message file "${test}.msg". This must
# match the expected messages in "${test}.mex" to succeed.
#
# If hsc does not fail (intentionally) it outputs also a "${test}.html"
# which needs to match "${test}.expected" for the test to succeed.

TESTS_FAIL = illgnest.hsc unknattr.hsc
TESTS_SUCC = $(filter-out $(TESTS_FAIL), $(wildcard *.hsc))

HSC = ../src/hsc/hsc
HSC_PREFS = ../hsc.prefs
HSC_ARGS =

.PHONY: clean check

test check: $(TESTS_SUCC:%.hsc=%.html) $(TESTS_FAIL:%.hsc=%.msg)

opt_getsize.html : HSC_ADD_ARGS := getsize lct
opt_rplcent.html : HSC_ADD_ARGS := rplcent
optim_tag.html: HSC_ADD_ARGS := compact

%.html: %.hsc
	$(HSC) prefsfile=$(HSC_PREFS) msgmode=pedantic msgfile=$*.msg status=quiet to=$@ $(HSC_ARGS) $(HSC_ADD_ARGS) $<
	diff --brief $@ $*.expected
	diff --brief $*.msg $*.mex

%.msg: %.hsc
	$(HSC) prefsfile=$(HSC_PREFS) msgmode=pedantic msgfile=$@ status=quiet to=$*.html $(HSC_ARGS) $(HSC_ADD_ARGS) $< && false || true
	diff --brief $*.msg $*.mex

clean:
	rm -f $(TESTS_SUCC:%.hsc=%.html)
	rm -f $(TESTS_SUCC:%.hsc=%.msg)
	rm -f $(TESTS_FAIL:%.hsc=%.msg)

distclean: clean
