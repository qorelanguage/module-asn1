

ASN1_SOURCES = asn1.cc QC_ASN1Sequence.cc QC_ASN1Integer.cc QC_ASN1ObjectIdentifier.cc QC_ASN1Boolean.cc QC_ASN1String.cc QC_ASN1OctetString.cc QC_ASN1UTF8String.cc

lib_LTLIBRARIES = asn1.la
asn1_la_SOURCES = $(ASN1_SOURCES)
asn1_la_LDFLAGS = -module -avoid-version $(ASN1_LDFLAGS) $(LIBXML2_LDFLAGS) $(QORE_LDFLAGS)

AM_CPPFLAGS = $(ASN1_CPPFLAGS) $(QORE_CPPFLAGS) $(LIBXML2_CPPFLAGS)

if COND_PROFILE
AM_CXXFLAGS = -g -pg
else
if COND_DEBUG
AM_CXXFLAGS = -g
endif
endif

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

install-exec-hook:
	mv $(libdir)/asn1.$(MODULE_SUFFIX) $(libdir)/asn1.qmod
	rm -f $(libdir)/asn1.la $(libdir)/asn1.a
