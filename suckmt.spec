#=========================================================================
#                   Copyright (C) 2000 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : suckmt.spec.in
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : RedHat RPM spec file.
#  $Date: 2000/10/22 22:31:33 $
#  $Revision: 1.10 $
#  $RCSfile: suckmt.spec.in,v $
#  $Author: niels $
#=========================================================================

Vendor: Niels Basjes
Summary: SuckMT, a multithreaded suck replacement
Name: suckmt
Version: 0.53
Release: 1
Source: http://www.wirehub.nl/~basjesn/suckmt/Files/suckmt-0.53.tar.gz
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
./configure --prefix=${RPM_BUILD_ROOT}

%build
make -j4

%install
make \
    rpminstall

%clean
if [ "${RPM_BUILD_ROOT}" != '/' ] ; then rm -rf ${RPM_BUILD_ROOT} ; fi

%files

%dir /var/spool/suckmt/in.coming/
%doc README ChangeLog INSTALL suckmt.lsm

%attr(- root root) /usr/bin/suckmt
%config /etc/suckmt.ini

%post
echo "Creating default configuration file..."
CONFIG_FILE=/etc/suckmt.ini

if [ -s ${CONFIG_FILE} ];
then
    NOW=`date`
    cp -f ${CONFIG_FILE} "${CONFIG_FILE}-backup-${NOW}"
    echo Your existing ${CONFIG_FILE} has been backed up as "${CONFIG_FILE}-backup-${NOW}"
else
    if [ -s ${CONFIG_FILE}.rpmorig ];
    then
        echo "It looks like you are upgrading (found ${CONFIG_FILE}.rpmorig)."
        echo "Automagically upgrading your original config file aswell:"
        cp ${CONFIG_FILE}.rpmorig ${CONFIG_FILE}
    fi
fi

/usr/bin/suckmt -init


# End of the file suckmt.spec
#=========================================================================
