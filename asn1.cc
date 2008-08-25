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

#include <qore/Qore.h>

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
DLLEXPORT qore_license_t qore_module_license = QL_GPL;

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

QoreStringNode *asn1_module_init()
{
   QoreClass *QC_ASN1OBJECT = new QoreClass("ASN1Object");
   CID_ASN1OBJECT = QC_ASN1OBJECT->getID();

   QC_ASN1OBJECT->setConstructor(ASN1OBJECT_constructor);

   QC_ASN1OBJECT->addMethod("getDer",    (q_method_t)ASN1OBJECT_getDer);

   ASN1_NS.addSystemClass(initASN1SequenceClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1IntegerClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1ObjectIdentifierClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1BooleanClass(QC_ASN1OBJECT));
   ASN1_NS.addSystemClass(initASN1StringClass(QC_ASN1OBJECT));

   // add constants
   ASN1_NS.addConstant("V_ASN1_BIT_STRING",       new QoreBigIntNode(V_ASN1_BIT_STRING));
   ASN1_NS.addConstant("V_ASN1_OCTET_STRING",     new QoreBigIntNode(V_ASN1_OCTET_STRING));
   ASN1_NS.addConstant("V_ASN1_UTF8STRING",       new QoreBigIntNode(V_ASN1_UTF8STRING));
   ASN1_NS.addConstant("V_ASN1_NUMERICSTRING",    new QoreBigIntNode(V_ASN1_NUMERICSTRING));
   ASN1_NS.addConstant("V_ASN1_PRINTABLESTRING",  new QoreBigIntNode(V_ASN1_PRINTABLESTRING));
   ASN1_NS.addConstant("V_ASN1_T61STRING",        new QoreBigIntNode(V_ASN1_T61STRING));
   ASN1_NS.addConstant("V_ASN1_TELETEXSTRING",    new QoreBigIntNode(V_ASN1_TELETEXSTRING));
   ASN1_NS.addConstant("V_ASN1_IA5STRING",        new QoreBigIntNode(V_ASN1_IA5STRING));
   ASN1_NS.addConstant("V_ASN1_GRAPHICSTRING",    new QoreBigIntNode(V_ASN1_GRAPHICSTRING));
   ASN1_NS.addConstant("V_ASN1_ISO64STRING",      new QoreBigIntNode(V_ASN1_ISO64STRING));
   ASN1_NS.addConstant("V_ASN1_VISIBLESTRING",    new QoreBigIntNode(V_ASN1_VISIBLESTRING));
   ASN1_NS.addConstant("V_ASN1_GENERALSTRING",    new QoreBigIntNode(V_ASN1_GENERALSTRING));
   ASN1_NS.addConstant("V_ASN1_UNIVERSALSTRING",  new QoreBigIntNode(V_ASN1_UNIVERSALSTRING));
   ASN1_NS.addConstant("V_ASN1_BMPSTRING",        new QoreBigIntNode(V_ASN1_BMPSTRING));

   return 0;
}

void asn1_module_ns_init(QoreNamespace *rns, QoreNamespace *qns)
{
   qns->addNamespace(ASN1_NS.copy());
}

void asn1_module_delete()
{
}
