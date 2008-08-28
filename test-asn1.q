#!/usr/bin/env qore

# requires the asn1 module
%requires asn1

# execute the asn1_test class as the application class
%exec-class asn1_test

# require all variables to be declared before use
%require-our

# enable all warnings
%enable-all-warnings

namespace asn1_test {
    const defaults = (
        "iters"         : 1,                # test loop iterations per thread
        "threads"       : 1                 # number of concurrent threads
        );

    const opts = 
        ( "verbose" : "verbose,v:i+",
          "help"    : "help,h",
          "iters"   : "iters,i=i",
          "threads" : "threads,t=i" );

    const SHA1_ID = 64;
}

class asn1_test::asn1_test {

    constructor() {
        $.command_line();
        
        printf("QORE ans1 module v%s Test Script (%d thread%s, %d iteration%s per thread)\n", ASN1::ModuleVersion, 
               $.threads, $.threads == 1 ? "" : "s", 
               $.iters, $.iters == 1 ? "" : "s");

	# seed the random number generator
	srand(clock_getmicros());

        $.counter = new Counter();
        my $t = $.threads;
        while ($t--) {
            $.counter.inc();
            background $.run_tests();
        }

        $.counter.waitForZero();

        my $ntests = elements $.thash;
        printf("%d error%s encountered in %d unique test%s in %d iteration%s (%d thread%s).\n",
               $.errors, $.errors == 1 ? "" : "s", 
               $ntests, $ntests == 1 ? "" : "s",
               $.ip, $.ip == 1 ? "" : "s",
               $.threads, $.threads == 1 ? "" : "s");
    }
    
    run_tests() {
	on_error ++$.errors;
        on_exit $.counter.dec();

        for (my $i = 0; $i < $.iters; $i++) {
	    my $msg = "hello";
	    my $hash = SHA1_bin($msg);

	    my $req = $.get_timestamp_req($hash, rand());

	    # save to file
	    #my $f = new File();
	    #$f.open2("asn1.bin", O_CREAT|O_TRUNC|O_WRONLY);
	    #$f.write($req.getDer());

	    my $req2 = ASN1Object::parse($req.getDer());

	    #printf("%N\n", $req2.getQoreData());

	    $.test_value($req.getDer(), $req2.getDer(), "ASN1 generation and parsing");

	    my $bin = binary("hi");
	    my $bs = new ASN1BitString($bin);

	    $.test_value($bs.getBinary(), $bin, "ASN1BitString::getQoreData()");

	    my $l = (False, True, True, False, True, False, False, False,
		     False, True, True, False, True, False, False, True );
	    $.test_value($bs.getQoreData(), $l, "ASN1BitString::getQoreData()");

	    $.test_value($bs.getBitString(), "0110100001101001", "ASN1BitSTring::getBitString()");

            $.ip += 1;
	}
    }

    static usage()
    {
        # get script name
        my $pn = split("/", $ENV."_");
        $pn = pop $pn; #$l[elements $l - 1];

        printf(
            "usage: %s [options] <iterations>
  -h,--help         shows this help text
  -i,--iters=ARG    runs ARG iterations of the tests in each thread
  -t,--threads=ARG  runs tests in ARG threads
  -v,--verbose=ARG  sets verbosity level to ARG
", 
            $pn);
        exit(1);
    }

    private command_line()
    {
        my $g = new GetOpt(opts);
        my $o = $g.parse(\$ARGV);
        if (exists $o."_ERRORS_") {
            printf("%s\n", $o."_ERRORS_"[0]);
            exit(1);
        }
        if ($o.help)
            asn1_test::usage();

        $.verbose = $o.verbose;

        # number of iterations
        $.iters = exists $o.iters ? $o.iters : defaults.iters;

        # number of threads
        $.threads = exists $o.threads ? $o.threads : defaults.threads;
    }

    test_value($v1, $v2, $msg) {
        if ($v1 === $v2) {
            if ($.verbose)
                printf("OK: %s test\n", $msg);
        }
        else {
            printf("ERROR: %s test failed! (%N != %N)\n", $msg, $v1, $v2);
            $.errors++;
        }
        $.thash.$msg = True;
    }

    private get_timestamp_req($hash, $nonce) {
    	my $seq = new ASN1Sequence();

	$seq.add(new ASN1Integer(1));
	$seq.add(asn1_test::get_message_imprint($hash));
	$seq.add(new ASN1Integer($nonce));

	$.test_value(3, $seq.size(), "ASN1Sequence::size()");

	return $seq;
    }

    static private get_message_imprint($hash) {
	my $seq = new ASN1Sequence();

	$seq.add(asn1_test::get_algorithm_identifier());
	$seq.add(new ASN1OctetString($hash));

	return $seq;
    }

    static private get_algorithm_identifier() {
	my $seq = new ASN1Sequence();

	$seq.add(new ASN1ObjectIdentifier(SHA1_ID));

	return $seq;
    }
}
