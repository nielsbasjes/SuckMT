#=========================================================================
#                   Copyright (C) 2000 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : suckmt.spec.in
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : RedHat RPM spec file.
#  $Date: 2000/03/14 18:55:21 $
#  $Revision: 1.6 $
#  $RCSfile: suckmt.spec.in,v $
#  $Author: niels $
#=========================================================================

Vendor: Niels Basjes
Summary: SuckMT, a multithreaded suck replacement
Name: suckmt
Version: 0.52
Release: 1
Source: http://www.wirehub.nl/~basjesn/Files/suckmt-0.52.tar.gz
Packager: Niels Basjes <SuckMT@Basjes.nl>
BuildRoot: /tmp/suck-%{PACKAGE_VERSION}-%{PACKAGE_RELEASE}
Copyright: GPL
Group: News
Provides: suckmt

%description
SuckMT is intended to be a multithreaded replacement for suck (http://home.att.net/~bobyetman/) i.e. use client NNTP to download news messages from a news server. SuckMT automatically balances the download of all NEWS items over several NNTP socket connections thus actually using a lot more of the available bandwidth. To upload new messages back to the NNTP server you will still need tools from the suck package.

%prep
%setup
make configure
./configure --prefix=${RPM_BUILD_ROOT}/usr

%build
make

%install
make \
    install

%clean
if [ "${RPM_BUILD_ROOT}" != '/' ] ; then rm -rf ${RPM_BUILD_ROOT} ; fi

%files

%doc README ChangeLog INSTALL suckmt.lsm

%attr(- root root) /usr/bin/suckmt

# End of the file suckmt.spec
#=========================================================================
