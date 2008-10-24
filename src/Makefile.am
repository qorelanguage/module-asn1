
ASN1_SOURCES = asn1.cc QC_ASN1Sequence.cc QC_ASN1Integer.cc QC_ASN1ObjectIdentifier.cc QC_ASN1Boolean.cc QC_ASN1String.cc QC_ASN1OctetString.cc QC_ASN1UTF8String.cc QC_ASN1BitString.cc

lib_LTLIBRARIES = asn1.la
asn1_la_SOURCES = $(ASN1_SOURCES)
asn1_la_LDFLAGS = -module -avoid-version $(OPENSSL_LDFLAGS)

AM_CPPFLAGS = $(ASN1_CPPFLAGS) $(QORE_CPPFLAGS) $(OPENSSL_CPPFLAGS)

if COND_PROFILE
AM_CXXFLAGS = -g -pg
else
if COND_DEBUG
AM_CXXFLAGS = -g
endif
endif

install-exec-hook:
	mv $(DESTDIR)$(libdir)/asn1.$(MODULE_SUFFIX) $(DESTDIR)$(libdir)/asn1.qmod
	rm -f $(DESTDIR)$(libdir)/asn1.la $(DESTDIR)$(libdir)/asn1.a
