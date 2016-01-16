#!/bin/perl -W
# =============================================================================
#  
#  This file is part of BibTool.
#  It is distributed under the GNU General Public License.
#  See the file COPYING for details.
#  
#  (c) 2011-2016 Gerd Neugebauer
#  
#  Net: gene@gerd-neugebauer.de
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#*=============================================================================

=head1 NAME

main.t - Test suite for BibTool main.

=head1 SYNOPSIS

main.t 

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

use constant OUT_FILE => '_output.bib';


#------------------------------------------------------------------------------
BUnit::run(name => 'output_file_0',
    args         => "--output.file",
    expected_err => <<__EOF__);

output.file
___________^
*** BibTool ERROR: Symbol expected.
__EOF__

#------------------------------------------------------------------------------
BUnit::run(name => 'output_file_1',
    args         => "-q --output.file=".OUT_FILE." bib/x1",
    expected_out => '',
    expected_err => '',
    prepare      => sub { unlink(OUT_FILE) if -e OUT_FILE; },
    check        => sub { if (-e OUT_FILE)  {
                            unlink(OUT_FILE);
			    return 0;
			  }
			  print STDERR "missing ".OUT_FILE."\t";
			  return 1});


#------------------------------------------------------------------------------
BUnit::run(name => '_o_0',
    args         => "-o",
    expected_err => <<__EOF__);

*** BibTool WARNING: Missing output file name
__EOF__

#------------------------------------------------------------------------------
BUnit::run(name => '_o_1',
    args         => "-q -o ".OUT_FILE." bib/x1",
    expected_out => '',
    expected_err => '',
    prepare      => sub { unlink(OUT_FILE) if -e OUT_FILE; },
    check        => sub { if (-e OUT_FILE)  {
                            unlink(OUT_FILE);
			    return 0;
			  }
			  print STDERR "missing ".OUT_FILE."\t";
			  return 1});

1;
#------------------------------------------------------------------------------
# Local Variables: 
# mode: perl
# End: 
