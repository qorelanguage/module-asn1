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

qore_classid_t CID_ASN1INTEGER;
QoreClass *QC_ASN1INTEGER;

static void ASN1INTEGER_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   const AbstractQoreNode *p = get_param(params, 0);

   QoreAsn1Integer *i = 0;

   qore_type_t t = p ? p->getType() : NT_NOTHING;
   if (t == NT_STRING) {
      const QoreStringNode *str = reinterpret_cast<const QoreStringNode *>(p);
      i = new QoreAsn1Integer(str);
   }
   else
      i = new QoreAsn1Integer(p ? p->getAsBigInt() : 0);

   if (!i) {
      xsink->raiseException("ASN1INTEGER-CONSTRUCTOR-ERROR", "failure to create ASN1Integer object");      
      return;
   }
   self->setPrivate(CID_ASN1INTEGER, i);
}

static void ASN1INTEGER_copy(QoreObject *self, QoreObject *old, QoreAsn1Integer *i, ExceptionSink *xsink)
{
   QoreAsn1Integer *ni = (QoreAsn1Integer *)i->copy();
   if (!ni) {
      xsink->raiseException("ASN1INTEGER-COPY-ERROR", "failure copying ASN1Integer object");
      return;
   }

   self->setPrivate(CID_ASN1INTEGER, ni);   
}

QoreClass *initASN1IntegerClass(QoreClass *parent)
{
   QC_ASN1INTEGER = new QoreClass("ASN1Integer");
   CID_ASN1INTEGER = QC_ASN1INTEGER->getID();

   QC_ASN1INTEGER->addBuiltinVirtualBaseClass(parent);

   QC_ASN1INTEGER->setConstructor(ASN1INTEGER_constructor);
   QC_ASN1INTEGER->setCopy((q_copy_t)ASN1INTEGER_copy);

   return QC_ASN1INTEGER;
}
