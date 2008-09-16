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

#ifndef _QORE_ASN1_H
#define _QORE_ASN1_H

#include "config.h"

#include <qore/Qore.h>

#include <openssl/asn1.h>
#include <openssl/objects.h>
#include <openssl/err.h>

#ifndef HAVE_OPENSSL_CONST
#define OPENSSL_CONST
#else
#define OPENSSL_CONST const
#endif

#include "AbstractQoreAsn1Object.h"
#include "QC_ASN1Sequence.h"
#include "QC_ASN1Integer.h"
#include "QC_ASN1ObjectIdentifier.h"
#include "QC_ASN1Boolean.h"
#include "QC_ASN1String.h"
#include "QC_ASN1OctetString.h"
#include "QC_ASN1UTF8String.h"
#include "QC_ASN1BitString.h"

DLLLOCAL extern qore_classid_t CID_ASN1OBJECT;

#endif
