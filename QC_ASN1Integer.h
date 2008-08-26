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

#ifndef _QORE_ASN1_QC_ASN1INTEGER_H
#define _QORE_ASN1_QC_ASN1INTEGER_H

DLLLOCAL extern qore_classid_t CID_ASN1INTEGER;
DLLLOCAL extern QoreClass *QC_ASN1INTEGER;

DLLLOCAL QoreClass *initASN1IntegerClass(QoreClass *parent);

class QoreAsn1Integer : public AbstractQoreAsn1Object
{
   protected:
      ASN1_STRING *i;

      DLLLOCAL virtual ~QoreAsn1Integer() {
	 if (i)
	    ASN1_STRING_free(i);
      }

   public:
      DLLLOCAL QoreAsn1Integer(int64 v) {
	 QoreString str;
	 str.sprintf("%lld", v);
	 BIGNUM *bn = 0;
	 if (!BN_dec2bn(&bn, str.getBuffer()))
	    return;

	 i = BN_to_ASN1_INTEGER(bn, 0);
	 BN_free(bn);
      }

      // takes over ownership of n_i
      DLLLOCAL QoreAsn1Integer(ASN1_STRING *n_i) : i(n_i) {}
      
      DLLLOCAL AbstractQoreAsn1Object *copy() const {
	 return new QoreAsn1Integer(ASN1_STRING_dup(i));
      }

      DLLLOCAL operator bool() const {
	 return (bool)i;
      }

      DLLLOCAL BinaryNode *getDerData() const {
	 int size = i2c_ASN1_INTEGER((ASN1_INTEGER *)i, 0);
	 unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * size);
	 unsigned char *tmp = data;
	 i2c_ASN1_INTEGER((ASN1_INTEGER *)i, &tmp);
	 
	 BinaryNode *rv = encodeDer(V_ASN1_INTEGER, size, data);
	 free(data);
	 return rv;
      }

      DLLLOCAL virtual AbstractQoreNode *getQoreData() const {
	 BIGNUM *bn = ASN1_INTEGER_to_BN(i, 0);
	 if (!bn)
	    return 0;

	 char *bnc = BN_bn2dec(bn);
	 QoreStringNode *str = bnc ? new QoreStringNode(bnc) : 0;
	 OPENSSL_free(bnc);
	 return str;
      }
};

#endif
