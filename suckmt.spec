#=========================================================================
#                   Copyright (C) 2000 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : suckmt.spec.in
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : RedHat RPM spec file.
#  $Date: 2000/01/08 22:25:15 $
#  $Revision: 1.3 $
#  $RCSfile: suckmt.spec.in,v $
#  $Author: niels $
#=========================================================================

Vendor: Niels Basjes
Summary: SuckMT, a multithreaded suck replacement
Name: suckmt
Version: 0.41
Release: 1
Source: http://www.wirehub.nl/~basjesn/Files/suckmt-0.41.tar.gz
Packager: Niels Basjes <SuckMT@Basjes.nl>
BuildRoot: /tmp/suck-%{PACKAGE_VERSION}-%{PACKAGE_RELEASE}
Copyright: GPL
Group: News
Provides: suckmt

%description
This package contains software to download news from an NNTP server to your
local machine. This software was inspired by suck (http://home.att.net/~bobyetman/)
but has a major difference with suck: SuckMT will open several NNTP connections 
simultaneously to reduce the required connect time. To upload new messages back
to the NNTP server you will still need tools from the suck package.

%prep
%setup


%build
make

%install
make \
    install

%clean
if [ "${RPM_BUILD_ROOT}" != '/' ] ; then rm -rf ${RPM_BUILD_ROOT} ; fi

%files

%doc README ChangeLog

%attr(- root root) /usr/bin/suckmt

# End of the file suckmt.spec
#=========================================================================
