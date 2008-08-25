#!/usr/bin/env qore

# requires the asn1 module
%requires asn1

# execute the asn1_test class as the application class
%exec-class asn1_test

# require all variables to be declared before use
%require-our

# enable all warnings
%enable-all-warnings

class asn1_test {

    constructor() {

	my $msg = "hello";
	my $hash = SHA1_bin($msg);

	my $req = asn1_test::get_timestamp_req($hash);

	# save to file
	my $f = new File();
	$f.open2("asn1.bin", O_CREAT|O_TRUNC|O_WRONLY);
	$f.write($req.getDer());
    }

    static private get_timestamp_req($hash)
    {
    	my $seq = new ASN1Sequence();

	$seq.add(new ASN1Integer(1));
	$seq.add(asn1_test::get_message_imprint($hash));

	return $seq;
    }

    static private get_message_imprint($hash)
    {
	my $seq = new ASN1Sequence();

	$seq.add(asn1_test::get_algorithm_identifier());
	$seq.add(new ASN1String($hash, V_ASN1_OCTET_STRING));

	return $seq;
    }

    static private get_algorithm_identifier()
    {
	my $seq = new ASN1Sequence();

	$seq.add(new ASN1ObjectIdentifier(64));

	return $seq;
    }
}
