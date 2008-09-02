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

qore_classid_t CID_ASN1BOOLEAN;
QoreClass *QC_ASN1BOOLEAN;

static void ASN1BOOLEAN_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   const AbstractQoreNode *p = get_param(params, 0);

   SimpleRefHolder<QoreAsn1Boolean> i(new QoreAsn1Boolean(p ? p->getAsBigInt() : 0));
   if (!i) {
      xsink->raiseException("ASN1BOOLEAN-COSNTRUCTOR-ERROR", "failure to create ASN1Boolean object");      
      return;
   }
   self->setPrivate(CID_ASN1BOOLEAN, i.release());
}

static void ASN1BOOLEAN_copy(QoreObject *self, QoreObject *old, QoreAsn1Boolean *b, ExceptionSink *xsink)
{
   xsink->raiseException("ASN1BOOLEAN-COPY-ERROR", "ASN1Boolean objects cannot be copied");
}

static AbstractQoreNode *ASN1BOOLEAN_value(QoreObject *self, QoreAsn1Boolean *b, const QoreListNode *params, ExceptionSink *xsink)
{
   return get_bool_node(b->value());
}

QoreClass *initASN1BooleanClass(QoreClass *parent)
{
   QC_ASN1BOOLEAN = new QoreClass("ASN1Boolean");
   CID_ASN1BOOLEAN = QC_ASN1BOOLEAN->getID();

   QC_ASN1BOOLEAN->addBuiltinVirtualBaseClass(parent);

   QC_ASN1BOOLEAN->setConstructor(ASN1BOOLEAN_constructor);
   QC_ASN1BOOLEAN->setCopy((q_copy_t)ASN1BOOLEAN_copy);

   QC_ASN1BOOLEAN->addMethod("value",      (q_method_t)ASN1BOOLEAN_value);

   return QC_ASN1BOOLEAN;
}
