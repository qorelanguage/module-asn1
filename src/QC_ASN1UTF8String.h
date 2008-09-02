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

#ifndef _QORE_ASN1_QC_ASN1UTF8STRING_H
#define _QORE_ASN1_QC_ASN1UTF8STRING_H

DLLLOCAL extern qore_classid_t CID_ASN1UTF8STRING;
DLLLOCAL extern QoreClass *QC_ASN1UTF8STRING;

DLLLOCAL QoreClass *initASN1UTF8StringClass(QoreClass *parent);

class QoreAsn1UTF8String : public QoreAsn1String
{
   protected:

   public:
      DLLLOCAL QoreAsn1UTF8String(const QoreString *str) : QoreAsn1String(V_ASN1_UTF8STRING, str->getBuffer(), str->strlen())
      {
      }

      // takes over ownership of n_str
      DLLLOCAL QoreAsn1UTF8String(ASN1_STRING *n_str) : QoreAsn1String(n_str) 
      {
	 assert(ASN1_STRING_type(str) == V_ASN1_UTF8STRING);
      }

      DLLLOCAL virtual AbstractQoreNode *getQoreData() const {
	 SimpleRefHolder<BinaryNode> b(getDerData());
	 const unsigned char *p = ((const unsigned char *)b->getPtr()) + 1;
	 decodeLen(p);
	 int hlen = (char *)p - (char *)b->getPtr();

	 return new QoreStringNode((const char *)p, b->size() - hlen, QCS_UTF8);
      }

      DLLLOCAL virtual const QoreClass *getQoreClass() const {
	 return QC_ASN1UTF8STRING;
      }
};

#endif
