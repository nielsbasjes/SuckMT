#=========================================================================
#                   Copyright (C) 2000 by Niels Basjes
#              SuckMT Website : http://oss.basjes.nl/SuckMT/
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : suckmt.spec.in
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : RedHat RPM spec file.
#  $Date: 2003/04/29 23:09:49 $
#  $Revision: 1.13 $
#  $RCSfile: suckmt.spec.in,v $
#  $Author: niels $
#=========================================================================

Vendor: Niels Basjes
Summary: SuckMT, a multithreaded suck replacement
Summary(pl):	SuckMT - wielow±tkowy zamiennik sucka
Name: suckmt
Version: 0.55
Release: 1
Source: http://oss.basjes.nl/SuckMT/Files/%{name}-%{version}.tar.gz
Packager: Niels Basjes <SuckMT@Basjes.nl>
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot-%(id -u -n)
BuildRequires:	autoconf
BuildRequires:	automake
Copyright: GPL
Group:		Applications/News
Provides: suckmt news-sucker

%description
This package contains software to download news from an NNTP server to
your local machine. This software was inspired by suck
(http://www.sucknews.org/) but has a major difference with
suck: SuckMT will open several NNTP connections simultaneously to
reduce the required connect time. To upload new messages back to the
NNTP server you will still need tools from the suck package.

%description -l pl
SuckMT to program do ¶ci±gania newsów z serwera NNTP na twój lokalny
komputer. Inspiracj± by³ program suck, od którego SuckMT do¶æ wyra¼nie
siê odró¿nia: SuckMT otwiera kilka równoleg³ych po³±czeñ z serwerem
NNTP aby ograniczyæ czas po³±czenia. Aby przes³aæ artyku³y z powrotem
do serwera NNTP potrzebujesz narzêdzi z pakietu suck.


%prep
%setup -q
%build
%{__make} configure
./configure --disable-dependency-tracking --disable-debug --prefix=/usr
%{__make} -j2

%install
rm -rf $RPM_BUILD_ROOT

%{__make} rpminstall DESTDIR=$RPM_BUILD_ROOT

%clean
if [ "${RPM_BUILD_ROOT}" != '/' ] ; then rm -rf ${RPM_BUILD_ROOT} ; fi

%files
%defattr(644,root,root,755)
%doc INSTALL README ChangeLog suckmt.ini.sample AUTHORS NEWS
%attr(755,root,root) %{_bindir}/suckmt
%config(noreplace,missingok) %verify(not md5 size mtime) %attr(660,news,news) %{_sysconfdir}/suckmt.ini
%attr(750,news,news) %dir %{_var}/spool/suckmt
%attr(750,news,news) %dir %{_var}/spool/suckmt/in.coming
%attr(750,news,news) %dir %{_var}/lib/suckmt
%attr(750,news,news) %dir %{_var}/log/suckmt
%attr(750,news,news) %{_var}/lib/suckmt/*

%post
echo "Creating default configuration file..."
CONFIG_FILE=%{_sysconfdir}/suckmt.ini

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

/usr/bin/suckmt -i ${CONFIG_FILE} -init


# End of the file suckmt.spec
#=========================================================================
