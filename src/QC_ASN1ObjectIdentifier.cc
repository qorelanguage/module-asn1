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

qore_classid_t CID_ASN1OBJECTIDENTIFIER;
QoreClass *QC_ASN1OBJECTIDENTIFIER;

static void ASN1OBJECTIDENTIFIER_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   const AbstractQoreNode *p = get_param(params, 0);

   SimpleRefHolder<QoreAsn1ObjectIdentifier> obj;

   if (p && p->getType() == NT_STRING) {
      const QoreStringNode *str = reinterpret_cast<const QoreStringNode *>(p);
      p = get_param(params, 1);

      obj = new QoreAsn1ObjectIdentifier(str->getBuffer(), p ? p->getAsInt() : 0);
   }
   else
      obj = new QoreAsn1ObjectIdentifier(p ? p->getAsBigInt() : 0);

   if (!obj) {
      xsink->raiseException("ASN1OBJECTIDENTIFIER-COSNTRUCTOR-ERROR", "failure to create ASN1ObjectIdentifier object");
      return;
   }

   self->setPrivate(CID_ASN1OBJECTIDENTIFIER, obj.release());
}

static void ASN1OBJECTIDENTIFIER_copy(QoreObject *self, QoreObject *old, QoreAsn1ObjectIdentifier *obj, ExceptionSink *xsink)
{
   xsink->raiseException("ASN1OBJECTIDENTIFIER-COPY-ERROR", "ASN1ObjectIdentifier objects cannot be copied");
}

static AbstractQoreNode *ASN1OBJECTIDENTIFIER_getName(QoreObject *self, QoreAsn1ObjectIdentifier *obj, const QoreListNode *params, ExceptionSink *xsink)
{
   return obj->getName();
}

static AbstractQoreNode *ASN1OBJECTIDENTIFIER_getNumericalForm(QoreObject *self, QoreAsn1ObjectIdentifier *obj, const QoreListNode *params, ExceptionSink *xsink)
{
   return obj->getNumericalForm();
}

QoreClass *initASN1ObjectIdentifierClass(QoreClass *parent)
{
   QC_ASN1OBJECTIDENTIFIER = new QoreClass("ASN1ObjectIdentifier");
   CID_ASN1OBJECTIDENTIFIER = QC_ASN1OBJECTIDENTIFIER->getID();

   QC_ASN1OBJECTIDENTIFIER->addBuiltinVirtualBaseClass(parent);

   QC_ASN1OBJECTIDENTIFIER->setConstructor(ASN1OBJECTIDENTIFIER_constructor);
   QC_ASN1OBJECTIDENTIFIER->setCopy((q_copy_t)ASN1OBJECTIDENTIFIER_copy);

   QC_ASN1OBJECTIDENTIFIER->addMethod("getName",          (q_method_t)ASN1OBJECTIDENTIFIER_getName);
   QC_ASN1OBJECTIDENTIFIER->addMethod("getNumericalForm", (q_method_t)ASN1OBJECTIDENTIFIER_getNumericalForm);

   return QC_ASN1OBJECTIDENTIFIER;
}
