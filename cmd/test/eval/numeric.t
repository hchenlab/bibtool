#!/bin/perl -W
# =============================================================================
#  
#  This file is part of BibTool.
#  It is distributed under the GNU General Public License.
#  See the file COPYING for details.
#  
#  (c) 2015-2016 Gerd Neugebauer
#  
#  Net: gene@gerd-neugebauer.de
#  
#*=============================================================================

=head1 NAME

numeric.t - Test suite for BibTool numeric values.

=head1 SYNOPSIS

numeric.t

=head1 DESCRIPTION

This module contains some test cases. Running this module as program
will run all test cases and print a summary for each. Optionally files
*.out and *.err are left if the expected result does not match the
actual result.

=head1 OPTIONS

none

=head1 AUTHOR

Gerd Neugebauer

=cut

use strict;
use BUnit;

$BUnit::name_prefix = 'eval/';

#------------------------------------------------------------------------------
BUnit::run(name         => 'numeric-0',
	   args	        => '--eval',
	   resource     => '0;',
	   expected_err => '',
	   expected_out => "0\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'numeric-1',
	   args	        => '--eval',
	   resource	=> '123;',
	   expected_err => '',
	   expected_out => "123\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'numeric-2',
	   args	        => '--eval',
	   resource	=> '-123;',
	   expected_err => '',
	   expected_out => "-123\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'plus-1',
	   args	        => '--eval',
	   resource	=> '2+3;',
	   expected_err => '',
	   expected_out => "5\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'plus-2',
	   args	        => '--eval',
	   resource	=> '2+-1;',
	   expected_err => '',
	   expected_out => "1\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'plus-11',
	   args	        => '--eval',
	   resource	=> '"2"+3;',
	   expected_err => '',
	   expected_out => "5\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'plus-12',
	   args	        => '--eval',
	   resource	=> '2+"3";',
	   expected_err => '',
	   expected_out => "5\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'plus-13',
	   args	        => '--eval',
	   resource	=> '"abc 2"+3;',
	   expected_err => '',
	   expected_out => "5\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'plus-14',
	   args	        => '--eval',
	   resource	=> '2+"abc3";',
	   expected_err => '',
	   expected_out => "5\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'plus-21',
	   args		=> '--eval',
	   resource	=> '2+[777];',
	   expected_err	=> '',
	   expected_out	=> "3\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'minus-1',
	   args	        => '--eval',
	   resource	=> '-(123);',
	   expected_err => '',
	   expected_out => "-123\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'minus-2',
	   args	        => '--eval',
	   resource	=> '1-1;',
	   expected_err => '',
	   expected_out => "0\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'minus-3',
	   args	        => '--eval',
	   resource	=> '2-1;',
	   expected_err => '',
	   expected_out => "1\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'minus-4',
	   args	        => '--eval',
	   resource	=> '1-2;',
	   expected_err => '',
	   expected_out => "-1\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'times-1',
	   args	        => '--eval',
	   resource	=> '2*3;',
	   expected_err => '',
	   expected_out => "6\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'times-2',
	   args	        => '--eval',
	   resource	=> '2*-3;',
	   expected_err => '',
	   expected_out => "-6\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'times-3',
	   args	        => '--eval',
	   resource	=> '0*0;',
	   expected_err => '',
	   expected_out => "0\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'times-4',
	   args	        => '--eval',
	   resource	=> '-3*-2;',
	   expected_err => '',
	   expected_out => "6\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'div-1',
	   args	        => '--eval',
	   resource	=> '3/2;',
	   expected_err => '',
	   expected_out => "1\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'div-2',
	   args	        => '--eval',
	   resource	=> '6/-2;',
	   expected_err => '',
	   expected_out => "-3\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'div-3',
	   args	        => '--eval',
	   resource	=> '5/1;',
	   expected_err => '',
	   expected_out => "5\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'div-err-1',
	   args	        => '--eval',
	   resource	=> '3/0;',
	   expected_err => "\n*** BibTool ERROR: Divide by 0\n",
	   expected_out => '' );

#------------------------------------------------------------------------------
BUnit::run(name         => 'mod-1',
	   args	        => '--eval',
	   resource	=> '3 mod 2;',
	   expected_err => '',
	   expected_out => "1\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'mod-2',
	   args	        => '--eval',
	   resource	=> '3 mod 0;',
	   expected_err => "\n*** BibTool ERROR: Modulo by 0\n",
	   expected_out => "" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'number-as-string-1',
	   args	        => '--eval',
	   resource	=> '123:as.string()',
	   expected_err => '',
	   expected_out => "\"123\"\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'number-as-number-1',
	   args	        => '--eval',
	   resource	=> '123:as.number()',
	   expected_err => '',
	   expected_out => "123\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'number-as-boolean-1',
	   args	        => '--eval',
	   resource     => '1:as.boolean()',
	   expected_err => '',
	   expected_out => "true\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'number-as-boolean-2',
	   args	        => '--eval',
	   resource     => '(-1):as.boolean()',
	   expected_err => '',
	   expected_out => "true\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'boolean-as-boolean-0',
	   args	        => '--eval',
	   resource     => '0:as.boolean()',
	   expected_err => '',
	   expected_out => "false\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'boolean-equals-0',
	   args	        => '--eval',
	   resource     => '0:equals(0)',
	   expected_err => '',
	   expected_out => "true\n" );


#------------------------------------------------------------------------------
BUnit::run(name         => 'number-equals-1',
	   args	        => '--eval',
	   resource     => '0:equals(1)',
	   expected_err => '',
	   expected_out => "false\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'number-equals-2',
	   args	        => '--eval',
	   resource     => '0:equals([])',
	   expected_err => '',
	   expected_out => "false\n" );

#------------------------------------------------------------------------------
BUnit::run(name         => 'number-equals-3',
	   args	        => '--eval',
	   resource     => '0:equals("0")',
	   expected_err => '',
	   expected_out => "false\n" );

1;
#------------------------------------------------------------------------------
# Local Variables: 
# mode: perl
# End: 
