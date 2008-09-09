/*
  asn1 Qore module

  Qore Programming Language

  Copyright (C) 2008 David Nichols, all rights reserved

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "qore-asn1.h"

QoreStringNode *asn1_module_init();
void asn1_module_ns_init(QoreNamespace *rns, QoreNamespace *qns);
void asn1_module_delete();

#define QORE_ASN1_VERSION "0.1"

// qore module symbols
DLLEXPORT char qore_module_name[] = "asn1";
DLLEXPORT char qore_module_version[] = QORE_ASN1_VERSION;
DLLEXPORT char qore_module_description[] = "asn1 module";
DLLEXPORT char qore_module_author[] = "David Nichols";
DLLEXPORT char qore_module_url[] = "http://qoretechnologies.com/qore";
DLLEXPORT int qore_module_api_major = QORE_MODULE_API_MAJOR;
DLLEXPORT int qore_module_api_minor = QORE_MODULE_API_MINOR;
DLLEXPORT qore_module_init_t qore_module_init = asn1_module_init;
DLLEXPORT qore_module_ns_init_t qore_module_ns_init = asn1_module_ns_init;
DLLEXPORT qore_module_delete_t qore_module_delete = asn1_module_delete;
DLLEXPORT qore_license_t qore_module_license = QL_LGPL;

QoreNamespace ASN1_NS("ASN1");

qore_classid_t CID_ASN1OBJECT;

static void ASN1OBJECT_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   xsink->raiseException("ASN1OBJECT-COSNTRUCTOR-ERROR", "ASN1Object is an abstract base class and cannot be explicitly constructed");
}

static AbstractQoreNode *ASN1OBJECT_getDer(QoreObject *self, AbstractQoreAsn1Object *obj, const QoreListNode *params, ExceptionSink *xsink)
{
   return obj->getDerData();
}

static AbstractQoreNode *ASN1OBJECT_getQoreData(QoreObject *self, AbstractQoreAsn1Object *obj, const QoreListNode *params, ExceptionSink *xsink)
{
   return obj->getQoreData();
}

static AbstractQoreAsn1Object *parseAsn1String(int type, const unsigned char *&p, const QoreClass *&qc, ExceptionSink *xsink)
{
   const unsigned char *t = p + 1;
   int len = AbstractQoreAsn1Object::decodeLen(t);
   // the cast on the following line is in case of older openssl versions
   ASN1_STRING *str = d2i_ASN1_type_bytes(0, (OPENSSL_CONST unsigned char **)&p, len + (t - p), type);
   if (!str) {
      long e = ERR_get_error();
      char buf[121];
      ERR_error_string(e, buf);
      xsink->raiseException("ASN1OBJECT-PARSE-ERROR", "failed to parse ASN1 string (0x%x) data: %s", *p, buf);
      return 0;
   }

   int str_type = ASN1_STRING_type(str);
   switch (str_type) {
      case V_ASN1_OCTET_STRING:
	 qc = QC_ASN1OCTETSTRING;
	 return new QoreAsn1OctetString(str);
	 break;

      case V_ASN1_BIT_STRING:
	 qc = QC_ASN1BITSTRING;
	 return new QoreAsn1BitString(str);
	 break;

      case V_ASN1_UTF8STRING:
	 qc = QC_ASN1UTF8STRING;
	 return new QoreAsn1UTF8String(str);
	 break;
   }

   ASN1_STRING_free(str);
   xsink->raiseException("ASN1OBJECT-PARSE-ERROR", "unsupported ASN1 string type 0x%x", str_type);
   return 0;
}

static AbstractQoreAsn1Object *parseAsn1Object(const unsigned char *&p, const QoreClass *&qc, ExceptionSink *xsink)
{
   int type = *p;
   switch (type) {
      case V_ASN1_BOOLEAN: {
	 qc = QC_ASN1BOOLEAN;
	 QoreAsn1Boolean *obj = new QoreAsn1Boolean(p[2]);
	 p += 3;
	 return obj;
      }

      case V_ASN1_INTEGER: {
	 ++p;
	 int len = AbstractQoreAsn1Object::decodeLen(p);
	 // the cast on the following line is for older openssl versions
	 ASN1_INTEGER *i = c2i_ASN1_INTEGER(0, (OPENSSL_CONST unsigned char **)&p, len);
	 if (!i) {
	    xsink->raiseException("ASN1OBJECT-PARSE-ERROR", "failed to parse ASN1 integer data");
	    return 0;
	 }
	 qc = QC_ASN1INTEGER;
	 QoreAsn1Integer *obj = new QoreAsn1Integer(i);
	 return obj;
      }

      case V_ASN1_BIT_STRING:
	 return parseAsn1String(B_ASN1_BIT_STRING, p, qc, xsink);

      case V_ASN1_OCTET_STRING:
	 return parseAsn1String(B_ASN1_OCTET_STRING, p, qc, xsink);

      case V_ASN1_UTF8STRING:
	 return parseAsn1String(B_ASN1_UTF8STRING, p, qc, xsink);

      case V_ASN1_NUMERICSTRING:
	 return parseAsn1String(B_ASN1_NUMERICSTRING, p, qc, xsink);

      case V_ASN1_PRINTABLESTRING:
	 return parseAsn1String(B_ASN1_PRINTABLESTRING, p, qc, xsink);

      case V_ASN1_T61STRING:
	 return parseAsn1String(B_ASN1_T61STRING, p, qc, xsink);

      case V_ASN1_VIDEOTEXSTRING:
	 return parseAsn1String(B_ASN1_VIDEOTEXSTRING, p, qc, xsink);

      case V_ASN1_IA5STRING:
	 return parseAsn1String(B_ASN1_IA5STRING, p, qc, xsink);

      case V_ASN1_GRAPHICSTRING:
	 return parseAsn1String(B_ASN1_GRAPHICSTRING, p, qc, xsink);

      case V_ASN1_ISO64STRING:
	 return parseAsn1String(B_ASN1_ISO64STRING, p, qc, xsink);

      case V_ASN1_GENERALSTRING:
	 return parseAsn1String(B_ASN1_GENERALSTRING, p, qc, xsink);

      case V_ASN1_UNIVERSALSTRING:
	 return parseAsn1String(B_ASN1_UNIVERSALSTRING, p, qc, xsink);

      case V_ASN1_BMPSTRING:
	 return parseAsn1String(B_ASN1_BMPSTRING, p, qc, xsink);

      // object identifier
      case V_ASN1_OBJECT: {
	 const unsigned char *t = p + 1;
	 int len = AbstractQoreAsn1Object::decodeLen(t);
	 //printd(5, "parseAsn1Object() type=%d, len=%d, 1st byte=0x%x\n", type, len, *t);
	 // the cast on the following line is for older openssl versions
	 ASN1_OBJECT *o = d2i_ASN1_OBJECT(0, (OPENSSL_CONST unsigned char **)&p, len + (t - p));
	 if (!o) {
	    long e = ERR_get_error();
	    char buf[121];
	    ERR_error_string(e, buf);
	    xsink->raiseException("ASN1OBJECT-PARSE-ERROR", "error decoding ASN1 object identifier: %s", buf);
	    return 0;
	 }
	 qc = QC_ASN1OBJECTIDENTIFIER;
	 return new QoreAsn1ObjectIdentifier(o);
      }

      case V_ASN1_SEQUENCE: {
	 ++p;
	 int len = AbstractQoreAsn1Object::decodeLen(p);
	 SimpleRefHolder<QoreAsn1Sequence> seq(new QoreAsn1Sequence());
	 const unsigned char *end = p + len;

	 while (p < end) {
	    const QoreClass *qc;
	    AbstractQoreAsn1Object *obj = parseAsn1Object(p, qc, xsink);
	    if (!obj)
	       return 0;
	    seq->add(obj);
	 }

	 qc = QC_ASN1SEQUENCE;
	 return seq.release();
      }
   }

   xsink->raiseException("ASN1OBJECT-PARSE-ERROR", "don't know how to decode ASN1 identifier octet 0x%x", *p);
   return 0;
}

static AbstractQoreNode *f_parse(const QoreListNode *params, ExceptionSink *xsink)
{
   const BinaryNode *b = test_binary_param(params, 0);
   if (!b) {
      xsink->raiseException("ASN1OBJECT-PARSE-ERROR", "expecting a sole binary object argument in DER format to ASN1Object::parse()");
      return 0;
   }

   const unsigned char *p = (const unsigned char *)b->getPtr();
   if (!p)
      return 0;

   const QoreClass *qc;
   AbstractQoreAsn1Object *obj = parseAsn1Object(p, qc, xsink);
   if (!obj)
      return 0;

   QoreObject *qo = new QoreObject(qc, getProgram());
   qo->setPrivate(qc->getID(), obj);
   return qo;
}

QoreStringNode *asn1_module_init()
{
   QoreClass *QC_ASN1OBJECT = new QoreClass("ASN1Object");
   CID_ASN1OBJECT = QC_ASN1OBJECT->getID();

   QC_ASN1OBJECT->setConstructor(ASN1OBJECT_constructor);

   QC_ASN1OBJECT->addMethod("getDer",      (q_method_t)ASN1OBJECT_getDer);
   QC_ASN1OBJECT->addMethod("getQoreData", (q_method_t)ASN1OBJECT_getQoreData);

   // static methods
   QC_ASN1OBJECT->addStaticMethod("parse", f_parse);

   ASN1_NS.addSystemClass(QC_ASN1OBJECT);
   ASN1_NS.addSystemClass(initASN1SequenceClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1IntegerClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1ObjectIdentifierClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1BooleanClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1StringClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1OctetStringClass(QC_ASN1STRING));
   ASN1_NS.addSystemClass(initASN1UTF8StringClass(QC_ASN1STRING));
   ASN1_NS.addSystemClass(initASN1BitStringClass(QC_ASN1STRING));

   // add constants
   ASN1_NS.addConstant("ModuleVersion",            new QoreStringNode(QORE_ASN1_VERSION));

   ASN1_NS.addConstant("V_ASN1_BIT_STRING",        new QoreBigIntNode(V_ASN1_BIT_STRING));
   ASN1_NS.addConstant("V_ASN1_OCTET_STRING",      new QoreBigIntNode(V_ASN1_OCTET_STRING));
   ASN1_NS.addConstant("V_ASN1_UTF8STRING",        new QoreBigIntNode(V_ASN1_UTF8STRING));
   ASN1_NS.addConstant("V_ASN1_NUMERICSTRING",     new QoreBigIntNode(V_ASN1_NUMERICSTRING));
   ASN1_NS.addConstant("V_ASN1_PRINTABLESTRING",   new QoreBigIntNode(V_ASN1_PRINTABLESTRING));
   ASN1_NS.addConstant("V_ASN1_T61STRING",         new QoreBigIntNode(V_ASN1_T61STRING));
   ASN1_NS.addConstant("V_ASN1_TELETEXSTRING",     new QoreBigIntNode(V_ASN1_TELETEXSTRING));
   ASN1_NS.addConstant("V_ASN1_IA5STRING",         new QoreBigIntNode(V_ASN1_IA5STRING));
   ASN1_NS.addConstant("V_ASN1_GRAPHICSTRING",     new QoreBigIntNode(V_ASN1_GRAPHICSTRING));
   ASN1_NS.addConstant("V_ASN1_ISO64STRING",       new QoreBigIntNode(V_ASN1_ISO64STRING));
   ASN1_NS.addConstant("V_ASN1_VISIBLESTRING",     new QoreBigIntNode(V_ASN1_VISIBLESTRING));
   ASN1_NS.addConstant("V_ASN1_GENERALSTRING",     new QoreBigIntNode(V_ASN1_GENERALSTRING));
   ASN1_NS.addConstant("V_ASN1_UNIVERSALSTRING",   new QoreBigIntNode(V_ASN1_UNIVERSALSTRING));
   ASN1_NS.addConstant("V_ASN1_BMPSTRING",         new QoreBigIntNode(V_ASN1_BMPSTRING));
   ASN1_NS.addConstant("V_ASN1_BOOLEAN",           new QoreBigIntNode(V_ASN1_BOOLEAN));
   ASN1_NS.addConstant("V_ASN1_INTEGER",           new QoreBigIntNode(V_ASN1_INTEGER));
   ASN1_NS.addConstant("V_ASN1_NULL",              new QoreBigIntNode(V_ASN1_NULL));
   ASN1_NS.addConstant("V_ASN1_OBJECT_DESCRIPTOR", new QoreBigIntNode(V_ASN1_OBJECT_DESCRIPTOR));
   ASN1_NS.addConstant("V_ASN1_EXTERNAL",          new QoreBigIntNode(V_ASN1_EXTERNAL));
   ASN1_NS.addConstant("V_ASN1_REAL",              new QoreBigIntNode(V_ASN1_REAL));
   ASN1_NS.addConstant("V_ASN1_ENUMERATED",        new QoreBigIntNode(V_ASN1_ENUMERATED));
   ASN1_NS.addConstant("V_ASN1_SET",               new QoreBigIntNode(V_ASN1_SET));
   ASN1_NS.addConstant("V_ASN1_UTCTIME",           new QoreBigIntNode(V_ASN1_UTCTIME));
   ASN1_NS.addConstant("V_ASN1_GENERALIZEDTIME",   new QoreBigIntNode(V_ASN1_GENERALIZEDTIME));

   return 0;
}

void asn1_module_ns_init(QoreNamespace *rns, QoreNamespace *qns)
{
   qns->addNamespace(ASN1_NS.copy());
}

void asn1_module_delete()
{
}
