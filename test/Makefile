HSC=../src/hsc/hsc
HSC_PREFS=../hsc.prefs
HSC_ARGS=

# tests to fail (hsc exit code not 0)
TESTS_FAIL=illgnest.hsc unknattr.hsc
# tests to succeed
TESTS_SUCC=$(filter-out $(TESTS_FAIL), $(wildcard *.hsc))

.PHONY: clean test

test : $(patsubst %.hsc, %.html, $(TESTS_SUCC)) $(patsubst %.hsc, %.msg, $(TESTS_FAIL))

opt_getsize.html : HSC_ADD_ARGS := getsize
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
	rm -f $(patsubst %.hsc, %.html, $(TESTS_SUCC))
	rm -f $(patsubst %.hsc, %.msg, $(TESTS_SUCC) $(TESTS_FAIL))

