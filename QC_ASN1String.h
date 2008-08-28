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

#ifndef _QORE_ASN1_QC_ASN1STRING_H
#define _QORE_ASN1_QC_ASN1STRING_H

DLLLOCAL extern qore_classid_t CID_ASN1STRING;
DLLLOCAL extern QoreClass *QC_ASN1STRING;

DLLLOCAL QoreClass *initASN1StringClass(QoreClass *parent);

class QoreAsn1String : public AbstractQoreAsn1Object
{
   protected:
      ASN1_STRING *str;

      DLLLOCAL virtual ~QoreAsn1String()
      {
	 if (str)
	    ASN1_STRING_free(str);
      }

   public:
      DLLLOCAL QoreAsn1String(int type, const void *ptr = 0, int len = 0)
      {
	 //printd(5, "QoreAsn1String::QoreAsn1String() this=%08p type=%d len=%d ptr=%08p\n", this, type, len, ptr);

	 str = ASN1_STRING_type_new(type);
	 if (!str || !len || !ptr)
	    return;

	 ASN1_STRING_set(str, ptr, len);
      }

      // takes over ownership of n_str
      DLLLOCAL QoreAsn1String(ASN1_STRING *n_str) : str(n_str) {}

      DLLLOCAL virtual AbstractQoreAsn1Object *copy() const {
	 return new QoreAsn1String(ASN1_STRING_dup(str));
      }

      DLLLOCAL operator bool() const { return (bool)str; }
      
      DLLLOCAL BinaryNode *getDerData() const
      {
	 int size = i2d_ASN1_bytes(str, 0, ASN1_STRING_type(str), V_ASN1_UNIVERSAL);
	 
	 unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * size);
	 unsigned char *tmp = data;

	 i2d_ASN1_bytes(str, &tmp, ASN1_STRING_type(str), V_ASN1_UNIVERSAL);

	 //printd(5, "QoreAsn1String::getDerData() returning type %d length %d\n", data[0], data[1]);
	 
	 return new BinaryNode(data, size);
      }

      DLLLOCAL virtual AbstractQoreNode *getQoreData() const {
	 assert(false);
	 return 0;
/*
	 SimpleRefHolder<BinaryNode> b(getDerData());
	 const unsigned char *p = ((const unsigned char *)b->getPtr()) + 1;
	 decodeLen(p);
	 int hlen = (char *)p - (char *)b->getPtr();

	 switch (ASN1_STRING_type(str)) {
	    case V_ASN1_UTF8STRING:
	       return new QoreStringNode((const char *)p, b->size() - hlen, QCS_UTF8);

	    case V_ASN1_NUMERICSTRING:
	    case V_ASN1_PRINTABLESTRING:
	    case V_ASN1_VISIBLESTRING:
	       return new QoreStringNode((const char *)p, b->size() - hlen);
	 }

	 BinaryNode *rv = new BinaryNode();
	 //printd(5, "getQoreData() new binarynode len=%d\n", b->size() - hlen);
	 rv->append(p, b->size() - hlen);
	 return rv;
*/
      }

      // this function should never be called
      DLLLOCAL virtual const QoreClass *getQoreClass() const {
	 assert(false);
	 return QC_ASN1STRING;
      }
};

#endif
