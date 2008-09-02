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

qore_classid_t CID_ASN1STRING;
QoreClass *QC_ASN1STRING;

static void ASN1STRING_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   xsink->raiseException("ASN1STRING-CONSTUCTOR-ERROR", "ASN1String is an abstract class and cannot be directly instantiated");
}

QoreClass *initASN1StringClass(QoreClass *parent)
{
   QC_ASN1STRING = new QoreClass("ASN1String");
   CID_ASN1STRING = QC_ASN1STRING->getID();

   QC_ASN1STRING->addBuiltinVirtualBaseClass(parent);
   QC_ASN1STRING->setConstructor(ASN1STRING_constructor);

   return QC_ASN1STRING;
}
